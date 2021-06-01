/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <memory.h>
#include <Logger.hpp>
#include <Parameters/SystemParameters.hpp>
#include <ServicePool.hpp>
#include <Parameters/SystemParameterMonitoring.hpp>
#include <Tasks/UARTTask.hpp>
#include "definitions.h"                // SYS function prototypes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

volatile uint8_t pinval = 0;
volatile int xTask1 = 1;

_Noreturn void xTask1Code(void *pvParameters){

    for(;;){
        PIO_PinToggle(PIO_PIN_PA23);
        //pinval = PIO_PinRead(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(500));
        LOG_DEBUG << "kalispera";
    }

};

_Noreturn void xTask2Code(void *pvParameters){

    for(;;){
//        PIO_PinToggle(PIO_PIN_PA23);
        pinval = PIO_PinRead(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(500));
        Services.onBoardMonitoring.checkAll();
    }

};

/**
 * Just calls the operator() function of a task
 * @param pvParameters Pointer to object of type Task
 */
template<class Task>
static void vClassTask(void *pvParameters) {
    (static_cast<Task *>(pvParameters))->operator()();
}


int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    systemParameterMonitoring.emplace();
    uartTask.emplace();

    xTaskCreate(xTask1Code, "Task1",1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(xTask2Code, "Task2",1000, NULL, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(vClassTask<UARTTask>, "UART", 1000, &*uartTask, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while ( true )
    {


        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }
#pragma clang diagnostic pop

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}






/*******************************************************************************
 End of File
*/
