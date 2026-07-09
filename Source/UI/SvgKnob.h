#pragma once
#include <JuceHeader.h>

class SvgKnob : public juce::Slider {
public:
    SvgKnob(const char* svgData, int svgSize, const juce::String& rotaryId, const juce::String& textId);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<juce::Drawable> svg;
    juce::Drawable* rotaryNode = nullptr;
    juce::Drawable* textNode = nullptr;
    juce::Rectangle<float> textBounds;
};