#include "PluginEditor.h"

DelayMixPluginAudioProcessorEditor::DelayMixPluginAudioProcessorEditor (DelayMixPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      // Initialize knobs with their respective SVG data and internal SVG IDs
      delayKnob(BinaryData::delay_knob_svg, BinaryData::delay_knob_svgSize, "delay_pointer", "delay_value_canva"),
      mixKnob(BinaryData::mix_knob_svg, BinaryData::mix_knob_svgSize, "mix_rotary", "mix_value_canva")
{
    background = juce::Drawable::createFromImageData(BinaryData::plugin_background_svg, BinaryData::plugin_background_svgSize);

    addAndMakeVisible(delayKnob);
    addAndMakeVisible(mixKnob);

    // Attach UI to DSP parameters
    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY", delayKnob);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixKnob);

    // Set the window size (Adjust to match your plugin_background.svg dimensions)
    setSize (600, 400); 
}

DelayMixPluginAudioProcessorEditor::~DelayMixPluginAudioProcessorEditor() {}

void DelayMixPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (background != nullptr) {
        background->setTransformToFit(getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
        background->draw(g, 1.0f);
    } else {
        g.fillAll (juce::Colours::darkgrey);
    }
}

void DelayMixPluginAudioProcessorEditor::resized()
{
    // Adjust these coordinates to place the knobs correctly over your background
    delayKnob.setBounds(100, 100, 150, 150);
    mixKnob.setBounds(350, 100, 150, 150);
}