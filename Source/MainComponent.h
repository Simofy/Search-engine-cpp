/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "listBoxModel.h"
#include <vector>
#include <map>
#include "sqlite3.h"
#include "operands.h"
#include "warehouse.h"
#include "suppliers.h"
#include "button.h"
#include "orderField.h"
#include <bitset>

#include <Windows.h>
#include <thread>
#include "timer.h"
#include <shared_mutex>
#include <algorithm>
#include "searchEngine.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, 
	public Button::Listener
{
public:
	

	LookAndFeel_V1 b;
	LookAndFeel_V2 c;
	LookAndFeel_V3 d;
	LookAndFeel_V4 e;
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
	void buttonClicked(Button* button) override;
    void paint (Graphics&) override;
    void resized() override;
	//std::vector<std::map<String, String>> data_warehouse;
	std::vector<StringArray> data_warehouse;
	StringArray data_warehouseHeader;
	//void setup() {
	//	int rc;
	//	

	//	sqlite3_stmt *statement;
	//	std::string tableName = "warehouse";
	//	std::string query("select * from " + tableName);
	//	int state = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
	//	//sqlite3_prepare_v2();
	//	if (state == SQLITE_OK) {
	//		int ctotal = sqlite3_column_count(statement);
	//		int res = 0;
	//		bool forHeader = false;
	//		
	//		while (1) {
	//			res = sqlite3_step(statement);
	//			if (res == SQLITE_ROW) {

	//				//std::map<String, String> data;
	//				StringArray data;
	//				for (int i = 0; i < ctotal; i++) {
	//					if (!forHeader) {
	//						data_warehouseHeader.add(stows((char*)sqlite3_column_name(statement, i)).c_str());
	//					}
	//					data.add(stows((char*)sqlite3_column_text(statement, i)).c_str());
	//				}

	//				forHeader = true;
	//				data_warehouse.push_back(data);
	//				//stops.push_back(sf_stop(info));
	//			}
	//			if (res == SQLITE_DONE || res == SQLITE_ERROR) {
	//				break;
	//			}
	//		}
	//	}
	//	sqlite3_close(db);
	//}
private:
	searchResultsComponent *searchResult;
	//rectButton order;
	//rectButton suppliers;
	juce::TextEditor inputSearchDirBox;
	juce::TextEditor inputSearchBox;

	juce::TextEditor outputRealTime;

	juce::ComboBox wichDrive;

	juce::Label inputDirBoxLabel;
	juce::Label inputBoxLabel;
	juce::Slider sliderBetweenInputBox;
	
	juce::ToggleButton flagCaseSensitive;
	juce::ToggleButton flagMatchWord;
	juce::ToggleButton flagIncludeExstensions;

	int dataNumber;
	juce::Label flagCaseSensitiveLabel;
	juce::Label flagMatchWordLabel;
	juce::Label flagIncludeExstensionsLabel;
	
	timer::Timer timer1;

	unsigned int threadId;

	std::mutex searchLock;
	std::thread searchThread;
	std::vector<std::vector<std::string>> files;
	std::vector<int> searchStatus;
	double mainFactor;

	TextButton buttonSearch;

	orderFieldComponent *field;
	TextButton accept;

	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
/*
==============================================================================

This file was auto-generated!

==============================================================================
*/
