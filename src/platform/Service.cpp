#include <iostream>
#include <iomanip>
#include <Logger.hpp>
#include "FreeRTOS.h"
#include "Service.hpp"

void Service::storeMessage(Message& message) {
    // appends the remaining bits to complete a byte
    message.finalize();

    // Generate a report and log it away
    LOG_DEBUG << "New " << ((message.packetType == Message::TM) ? "TM" : "TC")
              << " [" << message.serviceType << "," << message.messageType << "]";
}
