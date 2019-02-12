#pragma once
#ifndef LISTBOXMODEL_H
#define LISTBOXMODEL_H
#include <string>
#include "../JuceLibraryCode/JuceHeader.h"
#include <Windows.h>
#include <map>

class List : public Component, public TableListBoxModel {
public:
void toClipboard(const std::string &s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
	bool sortcol(const StringArray& v1,
		const StringArray& v2 ) {
		int to = TOSORT;
		return v1[to][0] < v2[to][0];
	}
	void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &mouseEvnt) {
		std::cout << String(items.at(rowNumber).getReference(columnId - 1) )<< std::endl;
		if (items.size() > rowNumber && columnId - 1 < items.at(0).size())
			toClipboard(String(items.at(rowNumber).getReference(columnId - 1)).toStdString());

	}
	List(){
		TOSORT = 0;
		fontSize = 18;
		font = Font(fontSize);
		//box.setModel(this); 
		//box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
		//box.setRowHeight(30);
		box_1.setModel(this);
		box_1.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
		box_1.setRowHeight(30);
		header = new TableHeaderComponent();
		box_1.setHeader(header);
		box_1.autoSizeAllColumns();
		addAndMakeVisible(box_1); 
	}
	void sortOrderChanged(int newSortColumnId, bool isForwards) override {
		TOSORT = newSortColumnId - 1;
		ISFORWARDS = !isForwards;
		std::sort(items.begin(), items.end(), [](const StringArray& a,const StringArray& b) {
			int i = 0;
			
			int numberA = std::atoi(a[TOSORT].toStdString().c_str());
			int numberB = std::atoi(b[TOSORT].toStdString().c_str());
			if(numberA == 0 || numberB == 0)
				while (1) {
					if (a[TOSORT][i] == b[TOSORT][i]) {
						i++;
						if(a[TOSORT].length() <= i || b[TOSORT].length() <= i)
							if (ISFORWARDS)
								return a[TOSORT].length() < b[TOSORT].length();
							else
								return a[TOSORT].length() > b[TOSORT].length();
					}else
						if (ISFORWARDS)
							return a[TOSORT][i] < b[TOSORT][i];
						else
							return a[TOSORT][i] > b[TOSORT][i];
				}
			if(ISFORWARDS)
				return numberA < numberB;
			else
				return numberA > numberB;
		});
		//std::sort(items.begin(), items.end(), sortcol);
		//items;
		//printf("%i, %i\n", newSortColumnId, isForwards);
		//std::vector<StringArray> items;

		

	}
	~List() {

	}
	//void paintListBoxItem(int rowNumber, Graphics &g, int width, int height,
	//	bool rowIsSelected) override {

	//	if (!isPositiveAndBelow(rowNumber, items.size())) return;
	//	g.fillAll(Colour::greyLevel(rowIsSelected ? 0.15f : 0.05f));
	//	g.setFont(18);
	//	g.setColour(Colours::whitesmoke);
	//	g.drawFittedText(items.getReference(rowNumber), { 6,0,width - 12,height }, Justification::centredLeft, 1, 1.f);
	//}
	double getTablePosition() {
		
		return box_1.getVerticalPosition();
		//return box_1.getRowPosition(0, true);
	}
	void paintRowBackground(Graphics &g, int rowNumber, int width, int height,
		bool rowIsSelected) override {
		if (!isPositiveAndBelow(rowNumber, items.size())) return;
		
		g.fillAll(Colour::greyLevel(rowIsSelected ? 0.15f : 0.05f));
		g.setColour(Colours::whitesmoke);
	}
	void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height,
		bool rowIsSelected) override {
		//if (!isPositiveAndBelow(rowNumber, items.size())) return;
		//g.fillAll(Colour::greyLevel(rowIsSelected ? 0.15f : 0.05f));
		g.setFont(fontSize);
		g.setColour(Colours::whitesmoke);//items.at(rowNumber-1).getReference(columnId-1)
		if(items.size() > rowNumber && columnId-1 < items.at(0).size())
		g.drawFittedText(String(items.at(rowNumber).getReference(columnId - 1)), { 6,0,width - 12,height }, Justification::centredLeft, 1, 1.f);
	}
	juce::ScrollBar & getHorizontalScrollBar() const{
		return box_1.getHorizontalScrollBar();
	}
	juce::ScrollBar & getVerticalScrollBar() const {
		return box_1.getVerticalScrollBar(); 
	}
	int getNumRows() override {
		return items.size();
	}

	void addColumn(const String& columnName,
		int columnId,
		int width,
		int minimumWidth = 30,
		int maximumWidth = -1,
		int propertyFlags = TableHeaderComponent::ColumnPropertyFlags::defaultFlags,
		int insertIndex = -1)
	{
		header->addColumn(columnName, columnId, width, minimumWidth, maximumWidth, propertyFlags, insertIndex);
		box_1.autoSizeColumn(columnId);
	}
	void addItem(const StringArray &input) {
		items.push_back(input);
		box_1.updateContent();
		box_1.selectRow(items.size() - 1);
	}
	void addItem(const std::vector<std::string> &input) {
		StringArray input_;
		for (auto var : input)input_.add(var);
		items.push_back(input_);
		box_1.updateContent();
		box_1.selectRow(items.size() - 1);
	}
	StringArray getSelectedRowArray() {
		int row = box_1.getSelectedRow();
		if (items.size() > row) {
			return items.at(row);
		}
		return String();
	}
	void resized() override;
	void Enabled(bool enable) {
		this->setEnabled(enable);
		box_1.setEnabled(enable);
		if(header != nullptr)
			header->setEnabled(enable);
		box_1.updateContent();
		
	}
	void setSelectedRow(int row) {
		if(row < box_1.getNumRows())
			box_1.selectRow(row);
	}

	int getSelectedRow() {
		return box_1.getSelectedRow();
		
	}
	void autoSizeAllColumns() {
		int colNum = 0;
		box_1.updateContent();
		int rowNum = box_1.getNumRows();
		if (header != nullptr)colNum = header->getNumColumns(false);
		for (int k = 1; k <= colNum; k++) {
			int maxColSize = font.getStringWidth(this->header->getColumnName(k)) + 12;
			for (int i = 0; i < rowNum; i++) {
				int	ColSize = font.getStringWidth(items.at(i).getReference(k - 1)) + 12;
				if (maxColSize < ColSize)maxColSize = ColSize;
			}
			if(header!= nullptr)
				header->setColumnWidth(k, maxColSize);
		}
	}
	void clearTable() {
		//if(header != nullptr)
		//	header->removeAllColumns();
		//box_1.deleteAllChildren();
		
		items.clear();
		box_1.updateContent();

		
		

	}
	
private:
	static int TOSORT;
	static bool ISFORWARDS;
	int fontSize;
	Font font;
	TableHeaderComponent * header;
	TableListBox box_1;
	//ListBox box;
	std::vector<StringArray> items;
};
#endif // !LISTBOXMODEL_H