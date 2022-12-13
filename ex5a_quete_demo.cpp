#include <Arduino.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// queue config
static const u_int8_t msg_queue_len = 5;


// globals
static QueueHandle_t msg_quete;


// task: print messages from queue
void printMsgs(void *parameter) {

    // A void pointer is a pointer that has no associated data type with it. 
    // A void pointer can hold address of any type and can be typecasted to any type. 

    int item;

    while (1) {
        if (xQueueReceive(msg_quete, (void*) &item, 0) == pdTRUE) {

            // If something is received (pdTRUE), print it out.
            Serial.println(item);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void setup() {

    Serial.begin(115200);

    msg_quete = xQueueCreate(msg_queue_len, sizeof(int));

    // tasks that run forever
    xTaskCreatePinnedToCore(
        printMsgs,
        "Print messages",
        1024,
        NULL,
        1,
        NULL,
        app_cpu);

}

void loop() {

    static int num = 0;

    if (xQueueSend(msg_quete, (void*) &num, 10) != pdTRUE) {
        Serial.println("Queue full.");
    }

    num++;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}