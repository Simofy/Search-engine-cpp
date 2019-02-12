#pragma once
#ifndef ORDERFIELD_H
#define ORDERFIELD_H

#include "sqlite3.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "listBoxModel.h"
class addAndSubstractField : public Component,
	public Button::Listener {
	TextButton plus;
	TextButton minus;
	int number;
public:
	int getNumber() {
		return number;
	}
	addAndSubstractField() {
		number = 0;
		plus.setButtonText("+");
		minus.setButtonText("-");
		plus.addListener(this);
		minus.addListener(this);
		plus.setColour(TextButton::ColourIds::buttonColourId, Colours::lightcoral);
		minus.setColour(TextButton::ColourIds::buttonColourId, Colours::lightcoral);
		plus.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
		minus.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
		this->addAndMakeVisible(&plus);
		this->addAndMakeVisible(&minus);
		

	}
	void buttonClicked(Button* button) override {
		if (button == &plus) {
			number++;
		}
		if (button == &minus) {
			number--;
		}
		repaint();
	}
	void paint(Graphics& g) override {
		juce::Rectangle<int> bound;
		bound.setWidth(getWidth() * 1.f / 3.f);
		bound.setHeight(getHeight());
		bound.setX(getWidth() * 1.f / 3.f);
		bound.setY(0);
		g.fillAll(Colours::coral);
		g.setColour(Colours::whitesmoke);
		g.drawFittedText(String(number), bound, Justification::centred, 1, 1.f);
		//g.fillAll(Colours::azure);
	}
	void resized() override {
		minus.setBounds(0, 0, getWidth() * 1.f / 3.f, getHeight());
		plus.setBounds(getWidth() * 2.f / 3.f, 0, getWidth() * 1.f / 3.f, getHeight());
	}
};
class orderFieldComponent : public Component,
	public Button::Listener {
public:
	orderFieldComponent() {}
	int a;
	std::vector<addAndSubstractField*> addSub;
	std::vector<StringArray> generateOrder() {
		std::vector<StringArray> toReturn;

		std::vector<StringArray>data;
		for each (auto var in data_warehouse){
			var.remove(0);
			data.push_back(var);
		}
		//if (data_warehouse.size() > 1){
		//	data = std::vector<StringArray>(data_warehouse.begin() + 1, data_warehouse.end());
		//else
		//	return toReturn;
		//4
		int i = 0;
		for each (auto var in addSub) {

			int numb = var->getNumber();
			if (numb > 0) {
				if (data.size() > i) {
					int value = data.at(i).getReference(4).getIntValue();
					value += numb;
					data.at(i).getReference(4) = String(value);
				}
				toReturn.push_back(data.at(i));
			}
			//data_warehouse
			i++;
		}
		return toReturn;
	}
	orderFieldComponent(sqlite3 *db, String supplier) {
		a = 0;
		this->db = db;
		if (db != nullptr)
			setup(supplier);
		
		int i = 0;
		for each (auto var in data_warehouseHeader)
		{
			list.addColumn(var, ++i, 60, 60,-1, TableHeaderComponent::ColumnPropertyFlags::notSortable || TableHeaderComponent::ColumnPropertyFlags::notResizable);
		}
		i = 1;
		
		for each (auto var in data_warehouse) {
			
			list.addItem(var);
			addSub.push_back(new addAndSubstractField());
			addSub.back()->setBounds(0, i * 30, 60, 30);
			list.addAndMakeVisible(addSub.back());
			i++;
		}
		list.autoSizeAllColumns();
		addChildComponent(&list);
		list.setVisible(true);
		//addAndMakeVisible(&list);
	}
	~orderFieldComponent() {
		for each (auto var in addSub)delete(var);
	}
	void buttonClicked(Button* button) override {
	}
	void paintOverChildren(Graphics& g)  override {
		//if (list.isMouseOver(true)) {
			double pos = list.getTablePosition();
			int i = 1;
			int w =getHeight() - (list.getNumRows()+1)*30;
			for each (auto var in addSub) {
				juce::Rectangle<int> bound = var->getBounds();
				bound.setY(i * 30 + w * pos);
				var->setBounds(bound);
				i++;
			}
		//}
	}
	void paint(Graphics& g)  override{

		
	}
	void resized() override {
		list.setBounds(0, 0, getWidth(), getHeight());
	}
	//std::vector<std::map<String, String>> data_warehouse;
	sqlite3 *db;
	void setSelectedRow(int row) {
		if (row < list.getNumRows())
			list.setSelectedRow(row);
	}
	int getSelectedRow() {
		return list.getSelectedRow();
	}
	void setup(String supplier) {
		sqlite3_stmt *statement;
		std::string tableName = "warehouse";
		std::string query("select * from " + tableName);
		int state = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
		//sqlite3_prepare_v2();
		if (state == SQLITE_OK) {
			int ctotal = sqlite3_column_count(statement);
			int res = 0;
			bool forHeader = false;
			data_warehouseHeader.add(L"Prideti");
			while (1) {
				res = sqlite3_step(statement);
				if (res == SQLITE_ROW) {
					StringArray data;
					data.add("");
					bool toAdd = false;
					for (int i = 0; i < ctotal; i++) {
						if (!forHeader) {
							data_warehouseHeader.add(stows((char*)sqlite3_column_name(statement, i)).c_str());
						}
						if (String(stows((char*)sqlite3_column_text(statement, i)).c_str()) == supplier)toAdd = true;
						data.add(stows((char*)sqlite3_column_text(statement, i)).c_str());
					}
					forHeader = true;
					if(toAdd)
						data_warehouse.push_back(data);
				}
				if (res == SQLITE_DONE || res == SQLITE_ERROR) {
					break;
				}
			}
		}
	}
private:
	List list;
	std::vector<StringArray> data_warehouse;
	StringArray data_warehouseHeader;
};



#endif // !ORDERFIELD_H

