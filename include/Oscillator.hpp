#pragma once
#include <cmath>

namespace Wavetables
{
    static inline double(Sine)(double p) { return std::sin(p * 3.14159265359 * 2); };
    static inline double(Square)(double p) { return p > 0.5 ? -1 : 1; };
    static inline double(Saw)(double p) { return 2 * (-p + 0.5); };
    static inline double(Triangle)(double p) { return 4 * std::abs(0.5 - p) - 1; };
}

class Oscillator
{
public:
	float NextSample(double phaseoffset = 0)
	{
        double delta = frequency / sampleRate;
        phase = std::fmod(1 + phase + phaseoffset + delta, 1);
        return wavetable(phase);
	}

    float Sample(double phaseoffset = 0)
    {
        return wavetable(std::fmod(1 + phase + phaseoffset, 1));
    }

    double(*wavetable)(double) = Wavetables::Sine;

    double phase = 0;
	double frequency = 0;
    double sampleRate = 48000;
};

