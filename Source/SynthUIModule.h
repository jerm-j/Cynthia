#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Utils.h"

class SynthUIModule : public juce::Component
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ComboBoxAttachment = APVTS::ComboBoxAttachment;

    virtual ~SynthUIModule() = default;

    void paint(juce::Graphics &g) override = 0;

    void resized() override = 0;

    virtual void configureKnob(juce::Slider &knob) = 0;
    virtual void configureComboBox(juce::ComboBox &comboBox, const juce::StringArray &items) = 0;

    virtual void configureComponentLabel(juce::Label &componentLabel, const juce::String &componentLabelText) = 0;

protected:

    template <typename T>
    inline juce::FlexBox makeComponentWithLabel(T &component, juce::Label &componentLabel,
                                         const int componentSize, const int componentLabelSize)
    {
        // compile time assertion to ensure that only class types derived from juce::Component are passed for generic type T.
        static_assert(std::is_base_of<juce::Component, T>::value, "T must be class type derived from juce::Component");

        juce::FlexBox column;
        column.flexDirection = juce::FlexBox::Direction::column;
        column.alignItems = juce::FlexBox::AlignItems::center;

        column.items.add(juce::FlexItem(componentLabel).withMinWidth(componentSize).withMinHeight(componentLabelSize));
        column.items.add(juce::FlexItem(component).withMinWidth(componentSize).withMinHeight(componentSize));

        return column;
    }
};