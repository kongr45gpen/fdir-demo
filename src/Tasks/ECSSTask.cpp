#include <optional>
#include <FreeRTOS.h>
#include <definitions.h>
#include <ServicePool.hpp>
#include <task.h>
#include <main.hpp>
#include "Tasks/ECSSTask.h"

std::optional<ECSSTask> ecssTask;

void ECSSTask::operator()() {
    Services.eventReport.informativeEventReport(EventReportService::MCUStart, "");

    for(;;) {
        pinval = PIO_PinRead(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(1));
        Services.onBoardMonitoring.checkAll(xTaskGetTickCount());
        Services.housekeeping.checkAndSendHousekeepingReports(xTaskGetTickCount());
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
ECSSTask::ECSSTask() {
    Services.functionManagement.include("restart", [](String<ECSS_FUNCTION_MAX_ARG_LENGTH>){
        NVIC_SystemReset();
    });

    Services.functionManagement.include("restart_sensor", [](String<ECSS_FUNCTION_MAX_ARG_LENGTH> args) {
        char sensor = args[0];

        if (sensor == '1' || sensor == '3') {
            if (temp1task) {
                temp1task->restart();
            }
        }

        if (sensor == '2' || sensor == '3') {
            if (temp2task) {
                temp2task->restart();
            }
        }
    });
}
#pragma clang diagnostic pop
