#pragma once

#include "apu_fwd.h"
#include "apu_length_counter.h"
#include "apu_enums.h"

class ApuEnvelope : public ApuLengthCounter {
public:
    ApuEnvelope(ApuChannelType channel_type, ApuContainer *apu_container);

private:
    bool start{};
    uint8_t divider{};
    uint8_t decay_counter{};
    bool use_const_volume{};
    uint8_t const_volume{};
};

