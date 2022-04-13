#define CONFIG_FREERTOS_HZ 1000
SemaphoreHandle_t mutSem;

static portMUX_TYPE my_mutex;
volatile int t2Missed = 0, t2Hit = 0;;

void t1( void * parameter )
{
  int lI,lII;
  for (;;)
  {
    portENTER_CRITICAL(&my_mutex);
    lI = t2Missed;
    lII = t2Hit;
    portEXIT_CRITICAL(&my_mutex);
    while ( xSemaphoreTake(mutSem, (TickType_t)1000)  != pdTRUE )   {}
    // yep got it - finally :-)
    Serial.print("1:yes  hit: ");
    Serial.print(lI);
    Serial.print(" nohit: ");
    Serial.println(lI);
    
    vTaskDelay(200);
    xSemaphoreGive(mutSem );
  }
}



void t2( void * parameter )
{
  while (1)
  {
    if ( xSemaphoreTake(mutSem, (TickType_t)10)  == pdTRUE )
    {
      // yep got it
      Serial.print("2:yes");
      portENTER_CRITICAL(&my_mutex);
      t2Hit++;
      portEXIT_CRITICAL(&my_mutex);

      xSemaphoreGive(mutSem);
    }
    else
    {
      portENTER_CRITICAL(&my_mutex);
      t2Missed++;
      portEXIT_CRITICAL(&my_mutex);
    }
    vTaskDelay(200);
  }
}

void setup()
{
  Serial.begin(112500);
  delay(3000);

  vPortCPUInitializeMutex(&my_mutex);
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
