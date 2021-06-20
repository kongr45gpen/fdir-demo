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

    for (;;) {
        pinval = PIO_PinRead(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(1));
        Services.onBoardMonitoring.checkAll(xTaskGetTickCount());
        Services.housekeeping.checkAndSendHousekeepingReports(xTaskGetTickCount());
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

ECSSTask::ECSSTask() {
    auto &Function = Services.functionManagement;

    Services.functionManagement.include("restart", functionRestart);
    Services.functionManagement.include("restart_sensor", functionRestartSensor);

    Services.eventAction.addEventActionDefinition(
            {0, 100, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart_sensor", "1"))});
    Services.eventAction.addEventActionDefinition(
            {0, 101, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart_sensor", "1"))});
    Services.eventAction.addEventActionDefinition(
            {0, 104, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart_sensor", "1"))});
    Services.eventAction.addEventActionDefinition(
            {0, 105, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart_sensor", "2"))});
    Services.eventAction.addEventActionDefinition(
            {0, 106, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart_sensor", "3"))});
    Services.eventAction.addEventActionDefinition(
            {0, 110, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart"))});
    Services.eventAction.addEventActionDefinition(
            {0, 111, 0, MessageParser::composeArbitraryECSS<64>(Function.callFromGround("restart"))});

}

void ECSSTask::functionRestart(String<16> args) {
    NVIC_SystemReset();
}

void ECSSTask::functionRestartSensor(String<16> args) {
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
}

#pragma clang diagnostic pop
