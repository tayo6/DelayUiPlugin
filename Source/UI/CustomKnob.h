#pragma once
#include <JuceHeader.h>

class CustomKnob : public juce::Slider {
public:
    enum KnobType { Mix, Delay };

    CustomKnob(KnobType type);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    KnobType knobType;
};