#pragma once
#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "listBoxModel.h"
#include <vector>
#include <map>
#include "sqlite3.h"
#include "operands.h"

class searchResultsComponent : public Component,
	public Button::Listener
{
public:
	//==============================================================================
	searchResultsComponent() {}
	searchResultsComponent(StringArray columnNames) {

		//data_warehouse.clear();
		//StringArray data;
		//data.add("1");
		//data.add("C/");
		//data.add("C/TEST.txt");
		//data.add("20");
		//data_warehouse.push_back(data);
		
		list = new List();
		addAndMakeVisible(list);
		int i = 0;
		for each (auto var in columnNames)
		{
			if(list != nullptr)
				list->addColumn(var, ++i, 60);
		}
		
		//for each (auto var in data_warehouse) {
		//	if (list != nullptr)
		//		list->addItem(var);
		//}
		if (list != nullptr)
			list->autoSizeAllColumns();
	}
	void addItem(StringArray data) {
		data_warehouse.push_back(data);
		if (list != nullptr)
			list->addItem(data);
	}
	void addItem(std::vector<std::string> data) {
		StringArray data_;
		for (auto var : data)data_.add(var);
		data_warehouse.push_back(data_);
		if (list != nullptr)
			list->addItem(data_);
	}
	void clearTable() {
		data_warehouse.clear();
		if (list != nullptr)list->clearTable();
	}
	~searchResultsComponent() {
		delete(list);
	}
	int getNumOfRows() {
		if (list != nullptr)
			return list->getNumRows();
		else
			return 0;
	}
	void buttonClicked(Button* button) override {

	}

	void paintOverChildren(Graphics& g)  override {
		//juce::Rectangle<int> test= list.getTablePosition();
		//printf("%i, %i s\n", test.getX(), test.getY());
	}
	void paint(Graphics& g) override {

		//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	}
	void resized() override {
		if (list != nullptr)
			list->setBounds(0, 0, getWidth(), getHeight() );
	}
	//std::vector<std::map<String, String>> data_warehouse;
	std::vector<StringArray> data_warehouse;
	//StringArray data_warehouseHeader;
	void setSelectedRow(int row) {
		if (list != nullptr)
			if (row < list->getNumRows())
				list->setSelectedRow(row);
	}
	int getSelectedRow() {
		if (list != nullptr)
			return list->getSelectedRow();
		else
			return -1;
	}
	int getUniqeId(int id) {
		for each (auto var in data_warehouse)
		{
			
			if (id == var.getReference(0).getIntValue())id = getUniqeId((unsigned int)rand());
		}
		return id;
	}
private:
	List *list;
	//TextButton logOut;
	//TextButton suppliers;
	//TextButton orderSupply;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(searchResultsComponent)
};
/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#endif // !WAREHOUSE_H

