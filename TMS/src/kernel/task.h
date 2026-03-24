#ifndef __TASK__
#define __TASK__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Task {

protected:
    int period;          
    TaskHandle_t handle;

    virtual void tick() = 0;

private:
    static void taskFunction(void* pvParameters) {
        Task* self = static_cast<Task*>(pvParameters);
        self->task();
        vTaskDelete(nullptr);
    }

    void task() {
        while (true) {
            this->tick();
            vTaskDelay(pdMS_TO_TICKS(this->period));
        }
    }

public:
    virtual void init(int period) {
        this->period = period;
        this->handle = nullptr;
    }

    void start(const char* name, int stackSize, int priority, int core) {
        xTaskCreatePinnedToCore(
            Task::taskFunction,
            name,
            stackSize,
            this,         
            priority,
            &handle,
            core
        );
    }

    void suspend() { if (this->handle) vTaskSuspend(this->handle); }
    void resume()  { if (this->handle) vTaskResume(this->handle);  }
    void stop()    { if (this->handle) vTaskDelete(this->handle);  }
};

#endif