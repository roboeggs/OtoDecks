#pragma once

#include <JuceHeader.h>

// InfiniteRotarySliderComponent class provides a custom rotary slider with infinite rotation capability.
class InfiniteRotarySliderComponent : public juce::Component
{
public:
    // Constructor: Initializes the InfiniteRotarySliderComponent.
    InfiniteRotarySliderComponent();
    // Destructor: Cleans up resources.
    ~InfiniteRotarySliderComponent() override;

    // Returns the current value of the slider.
    float getValue();

    // Sets the range of the slider.
    void setRange(double newMinimum, double newMaximum);
    // Sets the value of the slider.
    void setValue(double newValue);
    // Sets the angle of the slider.
    void setAngle(float newAngle);

    // Sets the minimum value for one revolution of the slider.
    void setOneRevolutionSliderMin(float newMin);
    // Sets the maximum value for one revolution of the slider.
    void setOneRevolutionSliderMax(float newMax);
    // Sets the end angle of the slider.
    void setEndAngle(float newEndAngle);
    // Sets the start value of the slider.
    void setStartValue(float newStartValue);
    // Sets whether the start value needs to be updated.
    void setStartValueNeedUpdate(bool newStartValueNeedUpdate);
    // Sets the step value for the slider.
    void setStep(float newStep);
    // Sets the adjusted value of the slider.
    void setAdjustedValue(float newAdjustedValue);

    // Paints the slider component.
    void paint(juce::Graphics&) override;
    // Resizes the slider component.
    void resized() override;

    // Callback function to notify when the value changes.
    std::function<void()> onValueChange;

    // Callback function to notify when the track position changes.
    std::function<void()> onTrackPositionChange;

    // Underlying juce::Slider component is exposed in order to allow the user to set styles etc.
    juce::Slider slider;

    // Returns the current angle of the slider.
    float getAngle();
    // Returns the adjusted value of the slider.
    float getAdjustedValue();

private:
    // Processes the slider value change.
    void m_processSliderValue(float value);
    // Sets the flag to update the start value.
    void m_setValueStartNeedUpdate();
    // Sets the end angle of the slider.
    void m_setAdjustedAngleEnd();
    // Returns the range of the slider.
    float m_getSliderRange();

    float m_oneRevolutionSliderMin{ 0.0f }; // Minimum value for one revolution
    float m_oneRevolutionSliderMax{ 360.0f }; // Maximum value for one revolution

    float m_angle{ 0.0f }; // Current angle of the slider

    float m_endAngle{ 0.0f }; // End angle of the slider

    float m_startValue{ 0.0f }; // Start value of the slider

    bool m_startValueNeedUpdate{ true }; // Flag indicating whether the start value needs to be updated

    float m_value{ 0.0f }; // Current value of the slider
    float m_step{ 1.0f }; // Step value for the slider
    float m_adjustedValue{ 0.0f }; // Adjusted value of the slider

    float m_knobMin{ 0.0f }; // Minimum value of the knob
    float m_knobMax{ 100.0f * 360 }; // Maximum value of the knob

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfiniteRotarySliderComponent)
};


