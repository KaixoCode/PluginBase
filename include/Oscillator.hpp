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

template<typename T>
class VoiceBank
{
public:
    VoiceBank(int voices)
        : m_Voices(voices)
    {
        m_Pressed.reserve(voices);
        m_Notes.reserve(voices);
        for (int i = 0; i < voices; i++)
            m_Notes.push_back(-1),
            m_Available.push_back(i),
            m_GeneratorVoices.emplace_back();
    }

    void NotePress(int note)
    {
        // Release the longest held note
        if (m_Available.size() == 0)
        {
            int longestheld = m_Pressed.back();
            m_Pressed.pop_back();

            // Set note to -1 and emplace to available.
            m_Notes[longestheld] = -1;
            m_Available.emplace(m_Available.begin(), longestheld);
        }

        // Get an available voice
        if (!m_Available.empty())
        {
            int voice = m_Available.back();
            m_Available.pop_back();

            // Emplace it to pressed voices queue
            m_Pressed.emplace(m_Pressed.begin(), voice);

            // Set voice to note
            m_Notes[voice] = note;
            m_GeneratorVoices[voice].frequency = NoteToFreq(note);
            m_GeneratorVoices[voice].phase = 0;
        }
    }

    void NoteRelease(int note)
    {
        // Find the note in the pressed notes per voice
        while (true)
        {
            auto& it = std::find(m_Notes.begin(), m_Notes.end(), note);
            if (it != m_Notes.end())
            {
                // If it was pressed, get the voice index
                int voice = std::distance(m_Notes.begin(), it);

                // Set note to -1 and emplace to available.
                m_Notes[voice] = -1;
                m_Available.emplace(m_Available.begin(), voice);

                // Erase it from the pressed queue
                auto& it2 = std::find(m_Pressed.begin(), m_Pressed.end(), voice);
                if (it2 != m_Pressed.end())
                    m_Pressed.erase(it2);
            }
            else break;
        }
    }

    void WaveTable(double(*wavetable)(double))
    {
        for (auto& i : m_GeneratorVoices)
            i.wavetable = wavetable;
    }

    float NextSample()
    {
        float out = 0;
        for (int i = 0; i < m_Voices; i++)
            if (m_Notes[i] != -1)
                out += m_GeneratorVoices[i].NextSample();

        return out;
    }

    
    static inline float NoteToFreq(int note)
    {
        return 440.0 * std::pow(2.0, (note - 69) / 12.0);
    }
    
    
    int m_Voices;
    std::vector<T> m_GeneratorVoices;
    
    // Current pressed notes per voice
    std::vector<int> m_Notes;

    // Queue to track longest held note
    std::vector<int> m_Pressed;

    // Available voices
    std::vector<int> m_Available;


};
