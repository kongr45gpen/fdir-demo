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
    auto &Parameter = Services.parameterManagement;

    auto compose = [](auto && PH1) { return MessageParser::composeArbitraryECSS<64>(std::forward<decltype(PH1)>(PH1), 0); };

    Services.functionManagement.include("restart", functionRestart);
    Services.functionManagement.include("restart_sensor", functionRestartSensor);

    Services.eventAction.addEventActionDefinition(
            {0, 100, 0, compose(Function.callFromGround("restart_sensor", "1"))});
    Services.eventAction.addEventActionDefinition(
            {0, 101, 1, compose(Function.callFromGround("restart_sensor", "2"))});
    Services.eventAction.addEventActionDefinition(
            {0, 102, 2, compose(Parameter.setParameterFromGround(systemParameters.temperature1Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 103, 3, compose(Parameter.setParameterFromGround(systemParameters.temperature2Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 104, 4, compose(Function.callFromGround("restart_sensor", "1"))});
    Services.eventAction.addEventActionDefinition(
            {0, 105, 5, compose(Function.callFromGround("restart_sensor", "2"))});
    Services.eventAction.addEventActionDefinition(
            {0, 106, 6, compose(Function.callFromGround("restart_sensor", "3"))});
    Services.eventAction.addEventActionDefinition(
            {0, 107, 7, compose(Parameter.setParameterFromGround(systemParameters.temperature1Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 108, 8, compose(Parameter.setParameterFromGround(systemParameters.temperature2Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 109, 9, compose(Parameter.setParameterFromGround(systemParameters.temperature1Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 109, 10, compose(Parameter.setParameterFromGround(systemParameters.temperature2Status, SystemParameters::TemperatureStatus::Disabled))});
    Services.eventAction.addEventActionDefinition(
            {0, 110, 11, compose(Function.callFromGround("restart"))});
    Services.eventAction.addEventActionDefinition(
            {0, 111, 12, compose(Function.callFromGround("restart"))});

}

void ECSSTask::functionRestart(String<16> args) {
    vTaskDelay(100);
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
