#pragma once
#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "listBoxModel.h"
#include <vector>
#include <map>
#include "sqlite3.h"
#include "operands.h"

class SuppliersComponent : public Component,
	public Button::Listener
{
public:
	//==============================================================================
	SuppliersComponent() {}
	SuppliersComponent(sqlite3 *db) {
		this->db = db;
		if (db != nullptr)
			setup();
		addAndMakeVisible(list);
		int i = 0;
		for each (auto var in data_warehouseHeader)
		{

			list.addColumn(var, ++i, 60);
		}
		for each (auto var in data_warehouse) {
			list.addItem(var);
		}
		list.autoSizeAllColumns();
	}
	~SuppliersComponent() {
	}
	void buttonClicked(Button* button) override {
		
	}
	void setSelectedRow(int row) {
		if (row < list.getNumRows())
			list.setSelectedRow(row);
	}
	int getSelectedRow() {
		return list.getSelectedRow();
	}
	StringArray getSelectedRowArray() {
		return list.getSelectedRowArray();
	}
	void paint(Graphics& g) override {
		//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	}
	void resized() override {
		list.setBounds(0, 0, getWidth(), getHeight());
	}
	//std::vector<std::map<String, String>> data_warehouse;
	std::vector<StringArray> data_warehouse;
	StringArray data_warehouseHeader;
	sqlite3 *db;
	void setup() {
		sqlite3_stmt *statement;
		std::string tableName = "supplier";
		std::string query("select * from " + tableName);
		int state = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
		//sqlite3_prepare_v2();
		if (state == SQLITE_OK) {
			int ctotal = sqlite3_column_count(statement);
			int res = 0;
			bool forHeader = false;
			while (1) {
				res = sqlite3_step(statement);
				if (res == SQLITE_ROW) {
					StringArray data;
					for (int i = 0; i < ctotal; i++) {
						if (!forHeader) {
							data_warehouseHeader.add(stows((char*)sqlite3_column_name(statement, i)).c_str());
						}
						data.add(stows((char*)sqlite3_column_text(statement, i)).c_str());
					}
					forHeader = true;
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
	//TextButton logOut;
	//TextButton suppliers;
	//TextButton orderSupply;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SuppliersComponent)
};
/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#endif // !WAREHOUSE_H

