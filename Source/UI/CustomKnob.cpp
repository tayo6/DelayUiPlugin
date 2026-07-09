#include "CustomKnob.h"

CustomKnob::CustomKnob(KnobType type) : knobType(type) {
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
}

void CustomKnob::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().toFloat();
    
    // Exact colors from your design
    juce::Colour darkBrown = juce::Colour::fromString("#422818");
    juce::Colour lightBrown = juce::Colour::fromString("#C88860");
    juce::Colour labelBgColor = juce::Colour::fromString("#E8CDBB");

    // Layout calculations
    float labelHeight = bounds.getHeight() * 0.15f;
    float knobAreaHeight = bounds.getHeight() - labelHeight - 10.0f; 
    
    float radius = juce::jmin(bounds.getWidth(), knobAreaHeight) / 2.0f * 0.85f;
    float centerX = bounds.getCentreX();
    float centerY = knobAreaHeight / 2.0f;
    
    float trackThickness = radius * 0.15f;
    
    // Angles for the arc
    float startAngle = -juce::MathConstants<float>::pi * 0.75f;
    float endAngle = juce::MathConstants<float>::pi * 0.75f;
    float currentAngle = juce::jmap((float)getValue(), (float)getMinimum(), (float)getMaximum(), startAngle, endAngle);

    // 1. Outer Dark Brown Track
    juce::Path trackArc;
    trackArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, startAngle, endAngle, true);
    g.setColour(darkBrown);
    g.strokePath(trackArc, juce::PathStrokeType(trackThickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // 2. Inner Light Brown Knob Body
    float innerRadius = radius - trackThickness;
    g.setColour(lightBrown);
    g.fillEllipse(centerX - innerRadius, centerY - innerRadius, innerRadius * 2, innerRadius * 2);

    // 3. Inner Dark Brown Ring
    g.setColour(darkBrown);
    g.drawEllipse(centerX - innerRadius, centerY - innerRadius, innerRadius * 2, innerRadius * 2, trackThickness * 0.6f);

    // 4. Dynamic Parts (Mix Arc or Delay Dot)
    if (knobType == Mix) {
        juce::Path valueArc;
        valueArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, startAngle, currentAngle, true);

        juce::ColourGradient gradient(juce::Colours::lightgreen, centerX - radius, centerY + radius,
                                      juce::Colours::red, centerX + radius, centerY + radius, false);
        gradient.addColour(0.5, juce::Colours::yellow);

        g.setGradientFill(gradient);
        g.strokePath(valueArc, juce::PathStrokeType(trackThickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    } 
    else if (knobType == Delay) {
        float dotRadius = innerRadius * 0.15f;
        float dotDistance = innerRadius * 0.65f; 
        
        float dotX = centerX + dotDistance * std::sin(currentAngle);
        float dotY = centerY - dotDistance * std::cos(currentAngle);
        
        g.setColour(darkBrown);
        g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2, dotRadius * 2);
    }

    // 5. Label Box and 0-100 Text
    float labelWidth = radius * 1.5f;
    juce::Rectangle<float> labelBounds(centerX - (labelWidth / 2.0f), bounds.getHeight() - labelHeight, labelWidth, labelHeight);
    
    g.setColour(labelBgColor);
    g.fillRect(labelBounds);

    int displayVal = (int)std::round(juce::jmap((float)getValue(), (float)getMinimum(), (float)getMaximum(), 0.0f, 100.0f));
    juce::String text = juce::String(displayVal);
    
    g.setColour(darkBrown); 
    g.setFont(juce::Font(labelHeight * 0.8f, juce::Font::bold));
    g.drawText(text, labelBounds, juce::Justification::centred, false);
}

void CustomKnob::resized() {}