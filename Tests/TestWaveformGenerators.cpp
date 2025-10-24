#include <gtest/gtest.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "WaveformGenerator.h"

/*
    Test Suite Name: TestWaveformGenerators
    Test Name: SineWaveIsFilledAndInRange

    This test ensures that the function fillWavetable() will indeed populate the wavetable,
    and only with sample values of a sine wave that are between -1 and 1 inclusive.
*/

TEST(TestWaveformGenerators, SineWaveIsFilledAndInRange)
{
    juce::AudioBuffer<float> table;
    table.setSize(1, 128);
    
    SineGenerator sine;
    sine.fillWavetable(table);

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
    table.setSize(1, 128);
    
    SawtoothGenerator sawtooth;
    sawtooth.fillWavetable(table);

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
    table.setSize(1, 128);
    
    TriangleGenerator triangle;
    triangle.fillWavetable(table);

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
    table.setSize(1, 128);
    
    SquareGenerator square;
    square.fillWavetable(table);

    ASSERT_GT(table.getNumSamples(), 0) << "Wavetable is empty!";

    auto* samples = table.getReadPointer(0);
    for(int sampleIndex = 0; sampleIndex < table.getNumSamples(); ++sampleIndex)
    {
        EXPECT_GE(samples[sampleIndex], -1.0f) << "Sample out of range at index " << sampleIndex;
        EXPECT_LE(samples[sampleIndex], 1.0f) << "Sample out of range at index " << sampleIndex;
    }
}