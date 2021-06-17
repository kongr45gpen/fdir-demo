#ifndef FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
#define FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP

#include <Services/OnBoardMonitoringService.hpp>
#include <Services/OnBoardMonitoringChecks.hpp>
#include <ServicePool.hpp>

class SystemParameterMonitoring {
    OnBoardMonitoringService::ParameterMonitoringDefinition<float,LimitCheck<float>> sensor1check {
        1, 0, 200, 5, {
            20,4,30,4
        }
    };

    OnBoardMonitoringService::ParameterMonitoringDefinition<float,LimitCheck<float>> sensor2check {
            2, 1, 1000, 1, {
                    10,0,40,0
            }
    };

    OnBoardMonitoringService::ParameterMonitoringDefinition<float, LimitCheck<float>> sensorDeltaCheck {
        3, 2, 200, 5, {
            -5, 99, 5, 99
        }
    };

public:
    SystemParameterMonitoring() {
        OnBoardMonitoringService& service = Services.onBoardMonitoring;

        service.addParameterMonitoringDefinition(sensor1check);
        service.addParameterMonitoringDefinition(sensor2check);
        service.addParameterMonitoringDefinition(sensorDeltaCheck);
    }
};

extern std::optional<SystemParameterMonitoring> systemParameterMonitoring;

#endif //FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
