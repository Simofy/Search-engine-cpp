#include "listBoxModel.h"

void List::resized() {
	box_1.setBounds(getLocalBounds());
}

int List::TOSORT = 0;
bool List::ISFORWARDS = false;