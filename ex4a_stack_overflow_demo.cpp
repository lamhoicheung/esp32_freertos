/*

  Stack overflow demo

  sizeof(int) is 4 bytes, b[100] takes around 400 bytes,
  The overhead of a task, for ESP32 is around 768 bytes,
  since we only allocate 1024 bytes, stack is overflowed

  The program runs when we increase the allocated stack size to let say 1500

*/


#include <Arduino.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// test task
void test(void *parameter) {
  while (1) {
    int a = 1;
    int b[100]; // sizeof(int) is 4 bytes

    // do something with the array so it is not optimized out by the compiler
    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }

    Serial.println(b[0]);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  // tasks that run forever
  xTaskCreatePinnedToCore(
    test,
    "Test task",
    1500,
    NULL,
    1,
    NULL,
    app_cpu);   

  vTaskDelete(NULL);             
}

void loop() {

}