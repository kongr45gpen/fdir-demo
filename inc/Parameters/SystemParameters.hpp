#ifndef FDIR_DEMO_SYSTEMPARAMETERS_HPP
#define FDIR_DEMO_SYSTEMPARAMETERS_HPP

#include <cstdint>
#include <Services/Parameter.hpp>

class SystemParameters {
public:
    Parameter<float> temperature1Value{0.0};
    Parameter<float> temperature2Value{0.0};

    Parameter<uint8_t> temperature1CommunicationFailureCount{0};
    Parameter<uint8_t> temperature2CommunicationFailureCount{0};

    /**
     * The key of the array is the ID of the parameter as specified in PUS
     */
    etl::array<std::reference_wrapper<ParameterBase>, ECSS_PARAMETER_COUNT> parametersArray = {
        std::ref<ParameterBase>(temperature1Value),
        std::ref<ParameterBase>(temperature2Value),
        std::ref<ParameterBase>(temperature1CommunicationFailureCount),
        std::ref<ParameterBase>(temperature2CommunicationFailureCount),
    };

    SystemParameters() = default;
};

extern SystemParameters systemParameters;

#endif //FDIR_DEMO_SYSTEMPARAMETERS_HPP
