#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

/**
 * @brief The ADSREnvelope class is a type of Envelope following the classic ADSR envelope shape. Durations of attack, decay and release are inherited from Envelope.
 */

class ADSREnvelope : public Envelope {
public:

    /**
     * @brief Default constructor that does nothing.
     */

    ADSREnvelope();

    /**
     * @brief Constructor that sets the envelope's length and fills it.
     * @param length: the envelope's length in samples
     * @param sampleRate: the sample rate in Hz
     */

    ADSREnvelope(int64_t length, int sampleRate);

    // Envelope interface
public:

    /**
     * @brief Fills the ADSR envelope.
     *
     * Fills the envelope according to the inherited values of attack, decay and
     * release. Sustain time will be set to the difference between total time
     * and combined attack, decay and release times.
     */

    void fill() override;

    ~ADSREnvelope() = default;
};

}
}
