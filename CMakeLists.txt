cmake_minimum_required(VERSION 3.20)
project(DelayMixPlugin VERSION 1.0.0)

# Fetch JUCE 7.0.12
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
)
FetchContent_MakeAvailable(juce)

# Define the Plugin
juce_add_plugin(DelayMixPlugin
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE DlMx
    FORMATS VST3
    PRODUCT_NAME "DelayMixPlugin")

# ---> Tell JUCE to generate JuceHeader.h for our plugin <---
juce_generate_juce_header(DelayMixPlugin)

# Embed SVG Assets into BinaryData
juce_add_binary_data(DelayMixAssets
    HEADER_NAME "BinaryData.h"
    NAMESPACE BinaryData
    SOURCES
        Resources/plugin_background.svg
        Resources/delay_knob.svg
        Resources/mix_knob.svg
)

# Add Source Files
target_sources(DelayMixPlugin PRIVATE
    Source/DSP/PluginProcessor.cpp
    Source/UI/SvgKnob.cpp
    Source/UI/PluginEditor.cpp
)

target_compile_definitions(DelayMixPlugin PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(DelayMixPlugin PRIVATE
    DelayMixAssets
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)