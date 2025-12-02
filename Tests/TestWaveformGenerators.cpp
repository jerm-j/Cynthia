#include <gtest/gtest.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../Source/Cynthia_DSP/WaveformGenerator.h"

/*
    Test Suite Name: TestWaveformGenerators
    Test Name: SineWaveIsFilledAndInRange

    This test ensures that the function fillWavetable() will indeed populate the wavetable,
    and only with sample values of a sine wave that are between -1 and 1 inclusive.
*/

inline static int sineChannelIndex = 0;
inline static int sawChannelIndex = 1;
inline static int triChannelIndex = 2;
inline static int squareChannelIndex = 3;

TEST(TestWaveformGenerators, SineWaveIsFilledAndInRange)
{
    juce::AudioBuffer<float> table;
    table.setSize(1, 2048);
    
    SineGenerator sine;
    sine.fillWavetable(table, sineChannelIndex);

    ASSERT_GT(table.getNumSamples(), 0) << "Wavetable is empty!";

    auto* samples = table.getReadPointer(0);
    for(int sampleIndex = 0; sampleIndex < table.getNumSamples(); ++sampleIndex)
    {
        EXPECT_GE(samples[sampleIndex], -1.0f) << "Sample out of range at index " << sampleIndex;
        EXPECT_LE(samples[sampleIndex], 1.0f) << "Sample out of range at index " << sampleIndex;
    }
}

/*
    Test Suite Name: TestWaveformGenerators
    Test Name: SawtoothWaveIsFilledAndInRange

    This test ensures that the function fillWavetable() will indeed populate the wavetable,
    and only with sample values of a sawtooth wave that are between -1 and 1 inclusive.
*/

TEST(TestWaveformGenerators, SawtoothWaveIsFilledAndInRange)
{
    juce::AudioBuffer<float> table;
    table.setSize(1, 2048);
    
    SawtoothGenerator sawtooth;
    sawtooth.fillWavetable(table, sawChannelIndex);

    ASSERT_GT(table.getNumSamples(), 0) << "Wavetable is empty!";

    auto* samples = table.getReadPointer(0);
    for(int sampleIndex = 0; sampleIndex < table.getNumSamples(); ++sampleIndex)
    {
        EXPECT_GE(samples[sampleIndex], -1.0f) << "Sample out of range at index " << sampleIndex;
        EXPECT_LE(samples[sampleIndex], 1.0f) << "Sample out of range at index " << sampleIndex;
    }
}

/*
    Test Suite Name: TestWaveformGenerators
    Test Name: TriangleWaveIsFilledAndInRange

    This test ensures that the function fillWavetable() will indeed populate the wavetable,
    and only with sample values of a triangle wave that are between -1 and 1 inclusive.
*/

TEST(TestWaveformGenerators, TriangleWaveIsFilledAndInRange)
{
    juce::AudioBuffer<float> table;
    table.setSize(1, 2048);
    
    TriangleGenerator triangle;
    triangle.fillWavetable(table, 2);

    ASSERT_GT(table.getNumSamples(), 0) << "Wavetable is empty!";

    auto* samples = table.getReadPointer(0);
    for(int sampleIndex = 0; sampleIndex < table.getNumSamples(); ++sampleIndex)
    {
        EXPECT_GE(samples[sampleIndex], -1.0f) << "Sample out of range at index " << sampleIndex;
        EXPECT_LE(samples[sampleIndex], 1.0f) << "Sample out of range at index " << sampleIndex;
    }
}

/*
    Test Suite Name: TestWaveformGenerators
    Test Name: SquareWaveIsFilledAndInRange

    This test ensures that the function fillWavetable() will indeed populate the wavetable,
    and only with sample values of a triangle wave that are between -1 and 1 inclusive.
*/

TEST(TestWaveformGenerators, SquareWaveIsFilledAndInRange)
{
    juce::AudioBuffer<float> table;
    table.setSize(1, 2048);
    
    SquareGenerator square;
    square.fillWavetable(table, squareChannelIndex);

    ASSERT_GT(table.getNumSamples(), 0) << "Wavetable is empty!";

    auto* samples = table.getReadPointer(0);
    for(int sampleIndex = 0; sampleIndex < table.getNumSamples(); ++sampleIndex)
    {
        EXPECT_GE(samples[sampleIndex], -1.0f) << "Sample out of range at index " << sampleIndex;
        EXPECT_LE(samples[sampleIndex], 1.0f) << "Sample out of range at index " << sampleIndex;
    }
}