/*
  ==============================================================================

    InfiniteRotarySliderComponent.cpp
    Created: 20 Feb 2025 5:39:23pm
    Author:  andfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InfiniteRotarySliderComponent.h"


//==============================================================================
InfiniteRotarySliderComponent::InfiniteRotarySliderComponent()
{
    slider.setSliderStyle(juce::Slider::Rotary);

    /** Hide underlying slider UI. TODO - let the user use lookAndFeel methods for that */
    slider.setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::transparentBlack);

    /** TODO Make configurable by the user */
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    slider.setRotaryParameters(0.0f, juce::MathConstants<float>::twoPi, false);
    slider.setRange(m_oneRevolutionSliderMin, m_oneRevolutionSliderMax);

    slider.onValueChange = [this] {
        m_processSliderValue(slider.getValue());
        };
    slider.onDragStart = [this] {
        m_setValueStartNeedUpdate();
        };
    slider.onDragEnd = [this] {
        m_setAdjustedAngleEnd();
        };

    addAndMakeVisible(slider);

}

InfiniteRotarySliderComponent::~InfiniteRotarySliderComponent()
{
}

void InfiniteRotarySliderComponent::paint (juce::Graphics& g)
{
    //float rectH = 10.0f;

    float rectW = 1.0f;
    float rectH = 10.0f;
    float lineThickness = 4.0f;
    //float radius = 80.0f;
    float radius = juce::jmin(getWidth(), getHeight()) * 0.5f - lineThickness;

    juce::Path p;

    p.addRectangle((getWidth() - rectW) * 0.5f,
        getHeight() * 0.5f - radius,
        rectW,
        rectH);

    /** Rotate the mark to show slider's rotation */
    p.applyTransform(
        juce::AffineTransform::rotation(
            getAngle(),
            getWidth() * 0.5,
            getHeight() * 0.5));

    g.setColour(juce::Colours::white);
    g.fillPath(p);

    ///** Fill the whole circle with marks */
    //for (int i{}; i < 19; ++i) {
    //    p.applyTransform(
    //        juce::AffineTransform::rotation(
    //            0.1f * juce::MathConstants<float>::pi,
    //            getWidth() * 0.5,
    //            getHeight() * 0.5));
    //    g.fillPath(p);
    //}

    /** Add knob handle (just a spot) */
    float spotR = 10.0f;

    juce::Path spot;

    juce::ColourGradient gradient(juce::Colours::orange, 0, 0, juce::Colours::blue, getWidth(), getHeight(), false);
    g.setGradientFill(gradient);

    //g.setColour(juce::Colours::orange);

    g.drawEllipse(getWidth() * 0.5 - radius,
                getHeight() * 0.5 - radius,
                radius * 2,
                radius * 2,
                lineThickness);

    spot.addEllipse(getWidth() * 0.5f - spotR,
        getHeight() * 0.5f - radius + spotR * 2.0f,
        spotR * 2.0f,
        spotR * 2.0f);

    spot.applyTransform(
        juce::AffineTransform::rotation(
            getAngle(),
            getWidth() * 0.5,
            getHeight() * 0.5));

    g.fillPath(spot);

}

void InfiniteRotarySliderComponent::resized()
{
    /** Occupy all available space */
    slider.setBounds(0, 0, getWidth(), getHeight());

}


void InfiniteRotarySliderComponent::m_processSliderValue(float value) {
    /** UI part */
    if (m_startValueNeedUpdate) {
        m_startValue = value;
        m_startValueNeedUpdate = false;
    }
    else {
        m_angle = m_endAngle + (value - m_startValue) / m_getSliderRange() * juce::MathConstants<float>::twoPi;
    }

    /** Data part */
    if (abs(value - m_startValue) > m_step) {
        if (m_value > value) {
            m_adjustedValue -= m_step;
        }
        else if (value - m_value > m_step) {
            if ((value - m_value) > (m_oneRevolutionSliderMax - m_oneRevolutionSliderMin) * 0.9f) {
                m_adjustedValue -= m_step;
            }
            else {
                m_adjustedValue += m_step;
            }
        }
    }

    m_value = (float)((int)value / m_step);

    m_adjustedValue = juce::jmin(m_adjustedValue, m_knobMax);
    m_adjustedValue = juce::jmax(m_adjustedValue, m_knobMin);

    if (onValueChange) {
        onValueChange();
    }
}

void InfiniteRotarySliderComponent::m_setValueStartNeedUpdate() {
    m_startValueNeedUpdate = true;
}

void InfiniteRotarySliderComponent::m_setAdjustedAngleEnd() {
    m_endAngle = m_angle;
}

float InfiniteRotarySliderComponent::m_getSliderRange() {
    return m_oneRevolutionSliderMax - m_oneRevolutionSliderMin;
}

float InfiniteRotarySliderComponent::getValue() {
    return m_adjustedValue;
}

float InfiniteRotarySliderComponent::getAngle() {
    return m_angle;
}

float InfiniteRotarySliderComponent::getAdjustedValue() {
    return m_adjustedValue;
}

void InfiniteRotarySliderComponent::setRange(double newMinimum, double newMaximum) {
    m_knobMin = (float)newMinimum;
    m_knobMax = (float)newMaximum;
}

void InfiniteRotarySliderComponent::setValue(double newValue) {
    m_adjustedValue = newValue;
}

void InfiniteRotarySliderComponent::setAngle(float newAngle)
{
    m_angle = newAngle;
    m_endAngle = newAngle; // Update the end angle to match the new angle
    repaint(); // Trigger a repaint to update the visual representation
}