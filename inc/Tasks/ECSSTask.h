#ifndef FDIR_DEMO_ECSSTASK_H
#define FDIR_DEMO_ECSSTASK_H

#include <optional>

class ECSSTask {
public:
    void operator()();

    ECSSTask();
private:
    volatile uint8_t pinval = 0;
};

extern std::optional<ECSSTask> ecssTask;


#endif //FDIR_DEMO_ECSSTASK_H
