/*
    Envelope.h

    This Envelope class implements a simple ADSR envelope from scratch.

    This algorithm uses a one-pole filter to allow for smooth transitions
    between the stages of the envelope.

    Source: "Creating Synthesizer Plug-ins with C++ and JUCE" by Matthijs Hollemans
*/


#pragma once

const float SILENCE = 0.0001f;
class Envelope
{
public:
    float nextValue()
    {
        level = multiplier * (level - target) + target;

        if(level + target > 3.0f)
        {
            multiplier = decayMultiplier;
            target = sustainLevel;
        }

        return level;
    }

    void reset()
    {
        level = 0.0f;
        target = 0.0f;
        multiplier = 0.0f;
    }

    void attack()
    {
        level += SILENCE + SILENCE;
        target = 2.0f;
        multiplier = attackMultiplier;
    }

    void release()
    {
        target = 0.0f;
        multiplier = releaseMultiplier;
    }

    // since the release curve will never reach 0, but will eventually cross
    // the SILENCE threshold we set, we should stop the voice from playing at this point.
    // since we wont be able to hear it anymore anyway.
    inline bool isActive() const
    {
        return level > SILENCE;
    }

    inline bool isInAttack() const
    {
        return target >= 2.0f;
    }

    float level;

    float attackMultiplier;
    float decayMultiplier;
    float sustainLevel;
    float releaseMultiplier;

private:
    float multiplier;
    float target;
};