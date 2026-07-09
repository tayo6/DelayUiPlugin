#include "SvgKnob.h"

SvgKnob::SvgKnob(const char* svgData, int svgSize, const juce::String& rotaryId, const juce::String& textId)
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    svg = juce::Drawable::createFromImageData(svgData, (size_t)svgSize);
    
    if (svg != nullptr) {
        // Find the rotating part (e.g., mix_rotary or delay_pointer)
        if (rotaryId.isNotEmpty()) {
            rotaryNode = svg->findChildById(rotaryId);
        }
        
        // Find the text canvas, save its bounds, and hide it so we can draw JUCE text over it
        if (textId.isNotEmpty()) {
            textNode = svg->findChildById(textId);
            if (textNode != nullptr) {
                textBounds = textNode->getDrawableBounds();
                textNode->setVisible(false); 
            }
        }
    }
}

void SvgKnob::paint(juce::Graphics& g)
{
    if (svg != nullptr) {
        // Calculate rotation angle based on slider value
        float angle = juce::jmap((float)getValue(), (float)getMinimum(), (float)getMaximum(),
                                 -juce::MathConstants<float>::pi * 0.75f,
                                 juce::MathConstants<float>::pi * 0.75f);

        if (rotaryNode != nullptr) {
            // Rotate around the center of the entire SVG to prevent wobbling
            auto center = svg->getDrawableBounds().getCentre();
            rotaryNode->setTransform(juce::AffineTransform::rotation(angle, center.x, center.y));
        }

        // Scale and draw the SVG to fit the component bounds
        auto bounds = getLocalBounds().toFloat();
        svg->setTransformToFit(bounds, juce::RectanglePlacement::centred);
        svg->draw(g, 1.0f);

        // Draw dynamic text over the hidden canvas area
        if (textNode != nullptr) {
            auto mappedBounds = textBounds.transformedBy(svg->getTransform());
            g.setColour(juce::Colours::white); // Change to match your UI
            g.setFont(mappedBounds.getHeight() * 0.7f);
            
            juce::String valueStr = juce::String(getValue(), 1);
            g.drawText(valueStr, mappedBounds, juce::Justification::centred, false);
        }
    }
}

void SvgKnob::resized() {
    // Resizing is handled dynamically in paint() via setTransformToFit
}