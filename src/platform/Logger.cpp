#include <Logger.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <definitions.h>
#include <Tasks/UARTTask.hpp>
#include <Message.hpp>
#include <MessageParser.hpp>
#include <COBS.hpp>

void Logger::log(Logger::LogLevel level, etl::istring & message) {
    char uartMessage[254];
    char uartOutput[256];
    char name[10];

    if (level <= Logger::trace) {
        strcpy(name, "trace");
    } else if (level <= Logger::debug) {
        strcpy(name, "debug");
    } else if (level <= Logger::info) {
        strcpy(name, "info");
    } else if (level <= Logger::notice) {
        strcpy(name, "notice");
    } else if (level <= Logger::warning) {
        strcpy(name, "warning");
    } else if (level <= Logger::error) {
        strcpy(name, "error");
    } else {
        strcpy(name, "emergency");
    }

    size_t count = snprintf(uartMessage, 254, "%-7lu [%-7s] %s\r\n", xTaskGetTickCount(), name, message.c_str());

    if (uartTask) {
        Message logMessage(255, 255, Message::TC, 0);
        std::copy(uartMessage, uartMessage + count, logMessage.data);
        logMessage.dataSize = count;

        etl::string<256> encodedData = COBSencode<256>(MessageParser::composeECSS(logMessage));

        uartTask->sendUARTMessage(encodedData);
    }
}
