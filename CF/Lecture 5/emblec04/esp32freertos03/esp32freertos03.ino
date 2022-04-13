#define CONFIG_FREERTOS_HZ 1000
int taskNr[] = {0, 1, 2, 3};
//Serial protected ?
void t1( void * parameter )
{
  for (;;)
  {
    Serial.print("12345678901234567890\n");
    //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void t2( void * parameter )
{
  for (;;)
  {
    Serial.print("abcdefghijklmnopqrst\n");
    //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void t3( void * parameter )
{
  for (;;)
  {
    Serial.print("ABCDEFGHIJKLMNOPQRST\n");
    //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(112500);
  delay(3000);
  xTaskCreate(t1, "t1", 10000,NULL,  2, NULL);
  xTaskCreate(t2, "t1", 10000, NULL,  2, NULL);
  xTaskCreate(t3, "t1", 10000, NULL,  2, NULL);
}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
