#include <iostream>
#include <iomanip>
#include <Logger.hpp>
#include <Tasks/UARTTask.hpp>
#include <COBS.hpp>
#include <MessageParser.hpp>
#include "FreeRTOS.h"
#include "Service.hpp"

void Service::storeMessage(Message& message) {
    // appends the remaining bits to complete a byte
    message.finalize();

    // Generate a report and log it away
    LOG_TRACE << "New " << ((message.packetType == Message::TM) ? "TM" : "TC")
              << " [" << message.serviceType << "," << message.messageType << "]";

    if (uartTask) {
        etl::string<256> encodedData = COBSencode<256>(MessageParser::composeECSS(message));

        uartTask->sendUARTMessage(encodedData);
    }
}
