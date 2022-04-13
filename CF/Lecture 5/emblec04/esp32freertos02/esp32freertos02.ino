#define CONFIG_FREERTOS_HZ 1000
int taskNr[]={0,1,2,3};
void anotherTask( void * parameter )
{
  for (;;) 
  {
    Serial.println(*(int *)parameter);
  //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );// will never come here
}

void setup()
{
  Serial.begin(112500);
  delay(3000); 

  for (int i=0; i <4; i++)
  xTaskCreate(anotherTask, "another Task", 10000, (void *)(taskNr+i),  2, NULL);
}

void loop()
{
  Serial.println(uxTaskPriorityGet(NULL));
  Serial.println("loop");
 // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
