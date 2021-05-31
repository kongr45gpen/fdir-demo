#ifndef FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
#define FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP

#include <Services/OnBoardMonitoringService.hpp>
#include <ServicePool.hpp>

class SystemParameterMonitoring {

    OnBoardMonitoringService::ParameterMonitoringDefinition<float,OnBoardMonitoringService::LimitCheck<float>> defn2 {
        1, 0, 100, 1, {
            10,0,40,0
        }
    };

public:
    SystemParameterMonitoring() {
        OnBoardMonitoringService& service = Services.onBoardMonitoring;

        service.addParameterMonitoringDefinition(defn2);
    }
};

extern std::optional<SystemParameterMonitoring> systemParameterMonitoring;

#endif //FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
