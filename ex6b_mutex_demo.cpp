#include <Arduino.h>

// You'll likely need this on vanilla FreeRTOS
// #include <semphr.h>

// use only 1 core for learning purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// globals
static int shared_var = 0;
static SemaphoreHandle_t mutex; // mutex and semaphore operates in a similar fashion, so they put them together


// task: increment shared var (the very inefficient way for demo purpose)
void incTask(void* parameter) {
    int local_var;

    while (1) {

        // take mutex prior to critical section
        if (xSemaphoreTake(mutex, 0) == pdTRUE) {
            local_var = shared_var; // read global var
            local_var++;
            vTaskDelay(random(100, 500) / portTICK_PERIOD_MS); // create inefficiency for local_var++ for demo purposes
            shared_var = local_var; // write to global var

            Serial.println(shared_var);

            // release mutex after critical section
            xSemaphoreGive(mutex);
        }
        else {
            // do something else if mutex is taken
        }

    }
}


void setup() {

    Serial.begin(115200);

    // create mutex before starting tasks
    mutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
        incTask,
        "Increment task 1",
        1024,
        NULL,
        1,
        NULL,
        app_cpu);

    xTaskCreatePinnedToCore(
        incTask,
        "Increment task 2",
        1024,
        NULL,
        1,
        NULL,
        app_cpu);

    vTaskDelete(NULL);
}

void loop() {

}