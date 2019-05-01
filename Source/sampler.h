#ifndef RAYTRACER_SAMPLER_H
#define RAYTRACER_SAMPLER_H

#include <random>

namespace scg
{

class Sampler
{
private:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

public:
    Sampler()
    {
        distribution = std::uniform_real_distribution<float>(0, 1);
    }

    float nextFloat()
    {
        return distribution(generator);
    }

    unsigned int nextDiscrete(unsigned int range)
    {
        return (unsigned int)/*std::floor*/(nextFloat() * range);
    }
};

/* Implementation from https://github.com/RichieSams/lantern
 * Lantern - A path tracer
 *
 * Lantern is the legal property of Adrian Astley
 * Copyright Adrian Astley 2015 - 2016
*/
/*
class UniformSampler
{
private:
    uint64_t m_state;
    const uint64_t m_sequence;

    // Note: Could replace this with memcpy, which gcc optimizes to the same assembly
    // as the code below. I'm not sure how other compiler treat it though, since it's
    // really part of the C runtime. The union seems to be portable enough.
    static float UintBitsToFloat(uint32_t i)
    {
        union
        {
            float f;
            uint32_t i;
        } unionHack;
        unionHack.i = i;
        return unionHack.f;
    }

public:
    Sampler(uint64_t seed, uint64_t sequence = 0)
        : m_state(seed),
          m_sequence(sequence)
    {
        NextUInt();
    }

    // PCG psuedo-random number generator
    // http://www.pcg-random.org/
    uint32_t NextUInt()
    {
        uint64_t oldState = m_state;

        m_state = oldState * 6364136223846793005ULL + (m_sequence | 1);
        uint32_t xorShifted = (uint32_t) (((oldState >> 18u) ^ oldState) >> 27u);
        uint32_t rot = oldState >> 59u;

        // Stop complaining about unary minus on an unsigned
        // It's intended
#pragma warning ( push)
#pragma warning ( disable : 4146)
        return (xorShifted >> rot) | (xorShifted << ((-rot) & 31));
#pragma warning ( pop)
    }

    float nextFloat()
    {
        uint32_t temp = NextUInt();
        // 2x-5x faster than i/float(UINT_MAX)
        return UintBitsToFloat((temp >> 9u) | 0x3F800000u) - 1.0f;
    }

    int nextDiscrete(unsigned int range)
    {
        return (int) (range * nextFloat());
    }
};
*/

}

#endif //RAYTRACER_SAMPLER_H
