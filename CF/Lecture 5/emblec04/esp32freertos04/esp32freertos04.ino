#define CONFIG_FREERTOS_HZ 1000
SemaphoreHandle_t myFirstSem;

void t1( void * parameter )
{
  for (;;)
  {
    if ( xSemaphoreTake(myFirstSem, (TickType_t)1000)  == pdTRUE )
    {
      // yep got it
      Serial.print("ABCDEFGHIJKLMNOPQRST\n");
      xSemaphoreGive(myFirstSem );
    }

  }
}
void t2( void * parameter )
{
  for (;;)
  {
    if ( xSemaphoreTake(myFirstSem, (TickType_t)1000)  == pdTRUE )
    {
      // yep got it
      Serial.print("abcdefghijklmnopqrst\n");
      xSemaphoreGive(myFirstSem);
    }
  }
}

void setup()
{
  Serial.begin(112500);
  delay(3000);
  myFirstSem = xSemaphoreCreateMutex(); // 1 key present
  // crt sem before you start tasks
  xTaskCreate(t1, "t1", 10000, NULL,  3, NULL);
  xTaskCreate(t2, "t1", 10000, NULL,  2, NULL);
  Serial.println("just bef");
  delay(3000);

}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
