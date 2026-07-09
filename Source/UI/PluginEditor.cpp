#include "PluginEditor.h"

DelayMixPluginAudioProcessorEditor::DelayMixPluginAudioProcessorEditor (DelayMixPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      delayKnob(CustomKnob::Delay),
      mixKnob(CustomKnob::Mix)
{
    background = juce::Drawable::createFromImageData(BinaryData::plugin_background_svg, BinaryData::plugin_background_svgSize);

    addAndMakeVisible(delayKnob);
    addAndMakeVisible(mixKnob);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAY", delayKnob);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixKnob);

    // Set plugin size EXACTLY to the background SVG size
    if (background != nullptr) {
        auto bounds = background->getDrawableBounds();
        setSize((int)bounds.getWidth(), (int)bounds.getHeight());
    } else {
        setSize (600, 400); 
    }
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
    // NOTE: Replace these numbers with the exact X, Y, Width, and Height from your SVG editor!
    // The height should be slightly larger than the width to fit the text label at the bottom.
    // Format: setBounds(X, Y, Width, Height)
    
    delayKnob.setBounds(120, 200, 150, 180); 
    mixKnob.setBounds(330, 200, 150, 180);
}