#include <Arduino.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// pins
static const int led_pin = 2;


// task: blink LED
void toggleLED(void *parameter) {
  while (1) {
     digitalWrite(led_pin, HIGH);
     vTaskDelay(500 / portTICK_PERIOD_MS);
     digitalWrite(led_pin, LOW);
     vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  // configure pins
  pinMode(led_pin, OUTPUT);

  // tasks that run forever
  xTaskCreatePinnedToCore(  // use xTaskCreate() in vanilla FreeRTOS
    toggleLED,              // function to be called
    "Toggle LED",           // name of task
    1024,                   // stack size (bytes in ESP32, words in FreeRTOS)
    NULL,                   // parameter to pass to function
    1,                      // task priority (0 to configMAX_PRIORITIES - 1)
    NULL,                   // task handle
    app_cpu);               // run on 1 core for learning purposes (ESP32 only)

  // if this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler()
  // in main after setting up your tasks 
}

void loop() {

}