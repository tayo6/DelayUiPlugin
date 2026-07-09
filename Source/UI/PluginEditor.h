#pragma once
#include <JuceHeader.h>
#include "../DSP/PluginProcessor.h"
#include "SvgKnob.h"

class DelayMixPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    DelayMixPluginAudioProcessorEditor (DelayMixPluginAudioProcessor&);
    ~DelayMixPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DelayMixPluginAudioProcessor& audioProcessor;
    
    std::unique_ptr<juce::Drawable> background;
    
    SvgKnob delayKnob;
    SvgKnob mixKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayMixPluginAudioProcessorEditor)
};