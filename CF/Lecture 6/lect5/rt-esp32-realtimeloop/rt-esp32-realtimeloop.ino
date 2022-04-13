#define CONFIG_FREERTOS_HZ 1000
SemaphoreHandle_t myFirstSem;
TimerHandle_t sampler1;

//https://w.freertos.org/a00110.html

// https://www.freertos.org/vtaskdelayuntil.html

#define configTIMER_SERVICE_TASK_NAME "Tmr Svc"
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2)
 
void t1( void * pvParameters )
{
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 10;
  int i = 0;
  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();

  for ( ;; )
  {
    // Wait for the next cycle.
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
    Serial.println(i);
    i++;
    // Perform action here.
  }
}

volatile  int cci = 0;

void setup()
{
  Serial.begin(115200);
  delay(3000);

  xTaskCreate(t1, "t1", 10000, NULL,  3, NULL);

}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
   vTaskDelay(1000 / portTICK_PERIOD_MS);
}


/*
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

     for( ;; )
     {
          vToggleLED();
         vTaskDelay( xDelay );
*/
