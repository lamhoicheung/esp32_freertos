#include <Arduino.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


static bool msgReady = false;
char* msgPtr = nullptr;
int msgByteSize;


void sender(void* parameter) {
  while (1) {

    msgByteSize = Serial.available();

    if (msgByteSize == 0) { // skip the following if there is nothing to read
      msgReady = false;
    } 
    else {
      msgPtr = (char*) pvPortMalloc(msgByteSize);

      for (int i = 0; i < msgByteSize; i++) {
        *(msgPtr + i) = Serial.read();
      }

      msgReady = true;
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); 
  }
}

void receiver(void* parameter) {
  while (1) {

    Serial.print("Heap (bytes): ");
    Serial.print(xPortGetFreeHeapSize());
    
    Serial.print("\tSENDER msgReady: ");
    Serial.print(msgReady);

    // Serial.println();

    if (msgReady) {
      char msg[msgByteSize];

      // read msg
      if (msgPtr != nullptr) {
        for (int i = 0; i < msgByteSize; i++) {
          msg[i] = *(msgPtr + i);
        }
      }

      // print msg
      Serial.print("\tSENDER msg: ");
      Serial.print(msg);

      // read memory
      vPortFree(msgPtr);
      msgPtr = nullptr;
      
      // reset flag
      msgReady = false;

    }

    Serial.println();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  // tasks that run forever
  xTaskCreatePinnedToCore(
    sender,
    "Sender",
    1500,
    NULL,
    1,
    NULL,
    app_cpu);

  xTaskCreatePinnedToCore(
    receiver,
    "Receiver",
    1500,
    NULL,
    1,
    NULL,
    app_cpu);

  vTaskDelete(NULL);           
}

void loop() {

  /*
  
  NOTES

  Note that while it is possible to have the second task wait 
  for our global pointer to that heap (msg_ptr) to be non-NULL, 
  this method is not thread-safe! As soon as the pvPortMalloc() returns, 
  msg_ptr will be set to a non-NULL value (assuming pvPortMalloc() did not return NULL, 
  indicating that we are out of heap). If the scheduler were to stop the first thread 
  and start running the second thread at this point, 
  there would be no message inside the heap memory! It would be random bytes, all 0s, or perhaps even an old message!

  
  To make this thread-safe, we need to use another global variable as a flag (msg_flag). 
  This flag only gets set to 1 after we are done copying the message over to the heap buffer. 
  We also check to make sure this flag is 0 before allocating new heap in the first task.

  In the second task, we wait for our message flag (msg_flag) to be 1 
  and then print out the message in heap when it is. 
  We then immediately free the heap memory, set it to NULL, and reset the message flag.
  
  */

}