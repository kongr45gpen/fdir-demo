#include <optional>
#include <FreeRTOS.h>
#include <definitions.h>
#include <ServicePool.hpp>
#include <task.h>
#include "Tasks/ECSSTask.h"

std::optional<ECSSTask> ecssTask;

void ECSSTask::operator()() {
    for(;;) {
        pinval = PIO_PinRead(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(1));
        Services.onBoardMonitoring.checkAll(xTaskGetTickCount());
        Services.housekeeping.checkAndSendHousekeepingReports(xTaskGetTickCount());
    }
}

ECSSTask::ECSSTask() {

}
