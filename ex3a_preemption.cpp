#include <Arduino.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// LED
static const int led_pin = 2;
static int blink_rate = 500;


// task: blink LED
void toggleLED(void *parameter) {
  while (1) {
     digitalWrite(led_pin, HIGH);
     vTaskDelay(blink_rate / portTICK_PERIOD_MS);
     digitalWrite(led_pin, LOW);
     vTaskDelay(blink_rate / portTICK_PERIOD_MS);
  }
}

void parseInput(void *parameter) {
  // in PlatformIO, just enter the number without return
  // the monitor will read the return, such that
  // Serial.parseInt() return 0 and set the rate to 0
  while(1) {
    if (Serial.available()) {
      blink_rate = Serial.parseInt();
      Serial.print("\tUpdated blink rate to ");
      Serial.println(blink_rate);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  vTaskDelay(500 / portTICK_PERIOD_MS);
  Serial.println("Enter positive integer to update blink rate.");

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
    app_cpu
  );   
  
  xTaskCreatePinnedToCore(  // use xTaskCreate() in vanilla FreeRTOS
    parseInput,              // function to be called
    "Parse input",           // name of task
    1024,                   // stack size (bytes in ESP32, words in FreeRTOS)
    NULL,                   // parameter to pass to function
    2,                      // task priority (0 to configMAX_PRIORITIES - 1)
    NULL,                   // task handle
    app_cpu
  );     

  // we delete the task containing the setup() and loop() functions. 
  // This will prevent loop() from running!
  vTaskDelete(NULL);

}

void loop() {
  // Finally, we leave loop() blank. 
  // In fact, execution should never reach there, as we deleted its associated task.

  /*

  Try playing around with the priorities of the tasks. 
  What happens if you make the “Toggle LED” task priority 2? 
  Why does everything still work? 

  What happens if you make the “Read Serial” task priority 2 
  instead (and leave the toggle task at priority 1)? 
  Why does the LED task stop blinking?

  Note: I don't know why but the behavior is still the same on both cases.
  
  */
}