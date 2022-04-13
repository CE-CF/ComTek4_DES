#define CONFIG_FREERTOS_HZ 1000
SemaphoreHandle_t myFirstSem;
TimerHandle_t sampler1;

#define configTIMER_SERVICE_TASK_NAME "Tmr Svc"
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2)

volatile  int cci = 0;
void t1( void * parameter )
{
  for (;;)
  {
    if ( xSemaphoreTake(myFirstSem, (TickType_t)1000)  == pdTRUE )
    {
      // yep got it
      Serial.print("ABCDEFGHIJKLMNOPQRST - ");
      Serial.println(cci);
      xSemaphoreGive(myFirstSem );
    }

  }
}

void samplerCode(xTimerHandle pxTimer)
{
  


  cci++; // no blocking eq wait on sem etc in here
}

void setup()
{
  Serial.begin(115200);
  delay(3000);
  sampler1 = xTimerCreate(
               "sampler1",
               pdMS_TO_TICKS(1000), /* period/time */
               pdTRUE, /* auto reload */
               (void*)0, /* timer ID */
               samplerCode); /* callback */
  xTimerStart(sampler1, 0);

    myFirstSem = xSemaphoreCreateMutex(); // 1 key present
 // myFirstSem = xSemaphoreCreateBinary(); // 1 key present
  // crt sem before you start tasks
  xTaskCreate(t1, "t1", 10000, NULL,  1, NULL);

}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}


/*
 *  const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

     for( ;; )
     {
          vToggleLED();
         vTaskDelay( xDelay );
 */
