#pragma once
#include <vector>
#include <deque>
#include "Grain.hpp"
#include "Essence.hpp"
#include <span.h>
#include <mutex>
#include <rubberband/RubberBandStretcher.h>

namespace Granulation {
namespace Synthesis {

class SequenceStrategy;
class Envelope;
class Source;

/**
 * @brief The Scheduler class is the link between a Granulator and its SequenceStrategy.
 * It manages Grains and synthetizes them when needed.
 *
 * The Scheduler is equipped with a mutex that ensures no access is made to the Grain list
 * while Grains are being synthetized.
 */

class Scheduler
{
public:

    /**
     * @brief A dummy constructor
     */

    Scheduler();

    /**
     * @brief Constructor for the Scheduler class
     * @param strategy: A strategy
     */

    Scheduler(SequenceStrategy* strategy);
    virtual ~Scheduler();

    /**
     * @brief Synthetizes one sample.
     * @param maxgrains: the maximal number of Grains to manage
     * @param loop: whether the Grains should loop
     * @return The synthetized sample
     */

    virtual float synthetize(int maxgrains = 1, bool loop = false);

    /**
     * @brief Synthetizes several samples.
     * @param vec: a gsl::span that will contain the synthetized samples
     * @param maxgrains: the maximal number of Grains to manage
     * @param loop: whether the Grains should loop
     */

    virtual void synthetize(gsl::span<float> vec, int maxgrains, bool loop = false);

    /**
     * @brief Sets the Strategy for the Scheduler.
     * @param strategy: pointer to a new Strategy
     */

    virtual void setStrategy(SequenceStrategy* strategy);

    /**
     * @brief Adds a Grain to the list of grains
     * @param g: a Grain
     * @param maxgrains: the maximal number of Grains to manage
     */

    virtual void addGrain(const Grain& g, int maxgrains);

    /**
     * @brief Activates the next Grain
     */

    virtual void activateNext();

    /**
     * @brief Gets the current number of managed Grains
     * @return The grain count
     */

    virtual int grainCount() const;

    /**
     * @brief Updates the time since the beginning
     * @param streamTime: the number of seconds elapsed since the beginning
     */

    virtual void updateTime(double streamTime);

    /**
     * @brief Sets the interonset in the SequenceStrategy
     * @param i: new interonset in ms
     */

    virtual void setInteronset(int i);

    /**
     * @brief Removes all Grains from the list
     */

    virtual void clearGrains();

    /**
     * @brief Gets the last grain added
     * @return A constant reference to the last Grain added to the Scheduler
     */

    virtual const Grain& lastGrainAdded() const;

    /**
     * @brief Gets the list of Grains managed by the Scheduler
     * @return A constant std::deque equivalent to the one used by the Scheduler
     */

    virtual const std::deque<Grain>& grains() const;

    /**
     * @brief Sets the reading direction to forwards or backwards.
     * @param readBackwards: true to read backwards, false to read forwards
     */

    virtual void setReadDirection(bool readBackwards);

private:
    mutable std::mutex m_grainsLock;
    std::deque<Grain> m_grains;
    SequenceStrategy* m_strategy;

    /**
     * @brief Removes all completed grains
     */

    void removeCompleted();
    int m_actives{0};
    bool m_readBackwards{false};
};

}
}
