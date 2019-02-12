#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#ifndef BUTTON_H
#define BUTTON_H

class rectButton : public juce::Button {
private:
public:
	rectButton(String &name) : Button(name) {

	}
	rectButton() : Button("") {
	}
	virtual void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) override{

		//LookAndFeel &feel = this->getLookAndFeel();
		//g.setColour(findColour(juce::TextButton::ColourIds::buttonColourId * juce::TextButton::ColourIds::buttonOnColourId));
		//g.drawRect(0, 0, getWidth(), getHeight());
		
		juce::Colour a(255,255,255);
		juce::Colour colour = findColour(juce::TextButton::ColourIds::buttonColourId);
		colour = isMouseOverButton ? colour.brighter(0.2f) : colour;

		g.setColour(colour);
		g.fillRect(0, 0, getWidth(), getHeight());

		juce::Rectangle<int>b(getLocalBounds());

		g.setColour(findColour(juce::TextButton::ColourIds::textColourOffId));
		if (isButtonDown) {
			g.setColour(colour.brighter(0.4f));
			float offx = b.getWidth() * 0.05f;
			float offy = b.getHeight() * 0.05f;
			//b.setWidth(b.getWidth() - offx * 2.f);
			//b.setHeight(b.getHeight() - offy * 2.f);
			b.setBounds(offx, offy, b.getWidth() - offx * 1.5f, b.getHeight() - offy * 1.5f);
			//b.setX(offx);
			//b.setY(offy);
			g.fillRect(b);
			g.setColour(findColour(juce::TextButton::ColourIds::textColourOnId));
		}

		g.drawFittedText(this->getButtonText(), b, Justification::centred, true);
		g.setColour(findColour(juce::TextButton::ColourIds::buttonColourId).brighter());
		g.drawRect(getLocalBounds());
		//g.setFont(20.0f);
			
		//g.fillAll();
	}

};

#endif // !BUTTON_H
