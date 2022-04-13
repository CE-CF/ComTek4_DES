#define CONFIG_FREERTOS_HZ 1000

 
void anotherTask( void * parameter )
{
  for (;;) 
  {
    Serial.println(*(int *)parameter);
    Serial.println(uxTaskPriorityGet(NULL));

  //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );// will never come here
}

void setup()
{
  Serial.begin(112500);
  delay(3000); 

  xTaskCreate(anotherTask, "another Task", 10000, NULL,  1, NULL);
}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
 // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
