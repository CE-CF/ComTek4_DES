 

//Jens s2021

// freertos considerations

struct rdWrLockTp {
  SemaphoreHandle_t rdSem, wrSem;
  int rdCount;
};


struct rdWrLockTp rdWrLock1;

volatile int rdInsideFortest = 0;

// RD WR LOCK BY SEMAPHORE

int rdWrLockInit(struct rdWrLockTp *lock)
{
  lock->rdSem = xSemaphoreCreateMutex();
  lock->wrSem = xSemaphoreCreateMutex();
  lock->rdCount = 0;
  return 0; // ok - no real chk
}

void rdEnter(struct rdWrLockTp *lock)
{
  while  ( xSemaphoreTake(lock->rdSem, (TickType_t)10)  != pdTRUE ) ;
  lock->rdCount++;
  if (lock->rdCount == 1) {
    while  ( xSemaphoreTake(lock->wrSem, (TickType_t)10)  != pdTRUE ) ;
  }
  rdInsideFortest++; // just for test
  xSemaphoreGive(lock->rdSem);
}

void wrEnter(struct rdWrLockTp *lock)
{
  while  ( xSemaphoreTake(lock->wrSem, (TickType_t)10)  != pdTRUE ) ;
}

void rdLeave(struct rdWrLockTp *lock)
{
  while  ( xSemaphoreTake(lock->rdSem, (TickType_t)10)  != pdTRUE ) ;
  lock->rdCount--;
  rdInsideFortest--;  // for test only
  if (lock->rdCount == 0) {
    xSemaphoreGive(lock->wrSem);  
  }
  xSemaphoreGive(lock->rdSem);
}

void wrLeave(struct rdWrLockTp *lock)
{
    xSemaphoreGive(lock->wrSem);
}

void wr(void *p)
{
  int i = 0;
  while (1) {
    delay( random(100, 1000));
    wrEnter(&rdWrLock1);
    Serial.println("wr in");
    delay( random(100, 2500));
    Serial.println("wr leaving");
    // just chekking
    if (0 < rdInsideFortest)
      Serial.println("problemmmmmss");
    wrLeave(&rdWrLock1);

  }
}

volatile int rdNr = 0;

void rd(void *p)
{
  int n;

  n = (int) p;
   while (1) {
    delay( random(100, 1000));
    rdEnter(&rdWrLock1);
    Serial.print("rd>> "); Serial.println(n);
    delay( random(100, 500));
    Serial.print("rd<<"); Serial.println(n);
    rdLeave(&rdWrLock1);

  }
}
void setup()
{
  Serial.begin(115200);  // for output from task 1
  delay(3000);
  rdWrLockInit(&rdWrLock1);
  xTaskCreate(wr, "wr", 2000, (void *)0,  2, NULL);
  xTaskCreate(rd, "wr", 2000, (void *)1,  2, NULL);
  xTaskCreate(rd, "wr", 2000, (void *)2,  2, NULL);
  xTaskCreate(rd, "wr", 2000, (void *)3,  2, NULL);
}

void loop() {
  /* loop will never be called */
}
