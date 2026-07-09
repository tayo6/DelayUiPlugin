#include "PluginProcessor.h"
#include "../UI/PluginEditor.h"

DelayMixPluginAudioProcessor::DelayMixPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
#endif
       apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

DelayMixPluginAudioProcessor::~DelayMixPluginAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout DelayMixPluginAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    // Delay from 0ms to 1000ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAY", "Delay Time", 0.0f, 1000.0f, 250.0f));
    // Mix from 0.0 (Dry) to 1.0 (Wet)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 1.0f, 0.5f));
    return { params.begin(), params.end() };
}

void DelayMixPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    delayLine.prepare(spec);
    delayLine.setMaximumDelayInSamples(sampleRate * 2.0); // Max 2 seconds
}

void DelayMixPluginAudioProcessor::releaseResources() {}

void DelayMixPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float delayMs = apvts.getRawParameterValue("DELAY")->load();
    float mix = apvts.getRawParameterValue("MIX")->load();
    
    float delaySamples = (delayMs / 1000.0f) * (float)getSampleRate();
    delayLine.setDelay(delaySamples);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float drySample = channelData[sample];
            delayLine.pushSample(channel, drySample);
            float wetSample = delayLine.popSample(channel);
            
            // Simple Mix logic
            channelData[sample] = (drySample * (1.0f - mix)) + (wetSample * mix);
        }
    }
}

juce::AudioProcessorEditor* DelayMixPluginAudioProcessor::createEditor() { return new DelayMixPluginAudioProcessorEditor (*this); }
void DelayMixPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void DelayMixPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DelayMixPluginAudioProcessor(); }