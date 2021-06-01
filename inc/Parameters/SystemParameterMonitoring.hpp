#ifndef FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
#define FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP

#include <Services/OnBoardMonitoringService.hpp>
#include <Services/OnBoardMonitoringChecks.hpp>
#include <ServicePool.hpp>

class SystemParameterMonitoring {
    OnBoardMonitoringService::ParameterMonitoringDefinition<float,LimitCheck<float>> sensor1check {
        1, 0, 100, 1, {
            10,0,40,0
        }
    };

    OnBoardMonitoringService::ParameterMonitoringDefinition<float,LimitCheck<float>> sensor2check {
            1, 1, 100, 1, {
                    10,0,40,0
            }
    };

public:
    SystemParameterMonitoring() {
        OnBoardMonitoringService& service = Services.onBoardMonitoring;

        service.addParameterMonitoringDefinition(sensor1check);
        service.addParameterMonitoringDefinition(sensor2check);
    }
};

extern std::optional<SystemParameterMonitoring> systemParameterMonitoring;

#endif //FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
