#pragma once
#include <cmath>

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

    double(*wavetable)(double) = [](double p) { return 2 * (-p + 0.5); };

    double phase = 0;
	double frequency = 0;
    double sampleRate = 48000;
};