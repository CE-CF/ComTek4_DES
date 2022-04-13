//inspired from https://microcontrollerslab.com/arduino-freertos-queues-create-read-write-examples/
 
#define CONFIG_FREERTOS_HZ 1000
SemaphoreHandle_t mutSem;

QueueHandle_t  msgQ;

static portMUX_TYPE my_mutex;
volatile int t2Missed = 0, t2Hit = 0;;

void t1( void * parameter )
{
  int i = 0;
  BaseType_t  res;
  int lI, lII;
  for (;;)
  {
    i++;
    res = xQueueSend(msgQ,  &i , 0 );// Send if space - dont wait for space !

 
    while ( xSemaphoreTake(mutSem, (TickType_t)1000)  != pdTRUE )   {}
    
    if (res == pdTRUE)
    Serial.print("1: did send ");
    else
    Serial.print("1: no send ");
    Serial.println(i);
    xSemaphoreGive(mutSem );
 
    vTaskDelay(200);
  }
}



void t2( void * parameter )
{
  int iii;
  int res;
  while (1)
  {
  // int iiii = uxQueueMessagesWaiting(msgQ); if you want to know nr msg in Q
    if (pdTRUE == xQueueReceive(msgQ, &iii, 100)) {
      res = 0;
    }
    else {
      res = -1;
    }

    taskYIELD();

    while (xSemaphoreTake(mutSem, (TickType_t)10)  != pdTRUE ) {}

    Serial.print("2: ");
    if (res == 0) {
      Serial.print(" got msg "); Serial.println(iii);
    }
    else {
      Serial.println(" no msg");
    }

    xSemaphoreGive(mutSem);

    vTaskDelay(500);
  }
}

void setup()
{
  Serial.begin(112500);
  delay(3000);

  vPortCPUInitializeMutex(&my_mutex);
  msgQ = xQueueCreate(10, sizeof(int));

  mutSem = xSemaphoreCreateMutex(); // 1 key present
  // crt sem before you start tasks
  xTaskCreatePinnedToCore(t1, "t1", 2000, NULL,  2, NULL, 0);
  xTaskCreatePinnedToCore(t2, "t2", 2000, NULL,  2, NULL, 0);
  Serial.println("just bef");
  delay(3000);

}

void loop()
{

}
