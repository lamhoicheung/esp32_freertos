#include <Arduino.h>

// You'll likely need this on vanilla FreeRTOS
// #include <semphr.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// pins
static const int led_pin = 2;


// globals



void blinkLED(void* parameter) {
    int num = *((int*) parameter); // cast param to integer pointer, then de-reference it

    Serial.print("Received: ");
    Serial.println(num);

    pinMode(led_pin, OUTPUT);

    while (1) {
        digitalWrite(led_pin, HIGH);
        vTaskDelay(num / portTICK_PERIOD_MS);       
        digitalWrite(led_pin, LOW);
        vTaskDelay(num / portTICK_PERIOD_MS);
    }
}


void setup() {

    long int delay_arg;

    // start Serial
    Serial.begin(115200);

    // wait for a moment to start
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Enter a number for delay (in ms)");

    // wait for input from Serial
    while (Serial.available() <= 0); 

    // read integer value
    delay_arg = Serial.parseInt();
    Serial.print("Sending: ");
    Serial.println(delay_arg);


    xTaskCreatePinnedToCore(
        blinkLED,
        "Blink LED",
        1024,
        (void*) &delay_arg,
        1,
        NULL,
        app_cpu);


    Serial.println("Done.");

    /*
    
        Before blinkLED starts, setup() has already finished,
        so delay_arg goes out of scope.
    
    */
}

void loop() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}