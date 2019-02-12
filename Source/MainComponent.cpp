#include "MainComponent.h"
#include <Windows.h>
#include <stdio.h>
//==============================================================================

MainContentComponent::MainContentComponent()
{
	threadId = 0;
	dataNumber = 0;
	searchResult = nullptr;
	field = nullptr;
	//searchStatus = false;
	//==========================
	int driveId = 0;
	DWORD dwBufferSize = GetLogicalDriveStrings(0, NULL);
	char* pDrives = (char *)malloc(dwBufferSize + 1);
	char* pDrivesToDelete = pDrives;
	if (pDrives != NULL)
	{
		GetLogicalDriveStrings(dwBufferSize, pDrives);
		while (*pDrives)
		{
			UINT nDriveType = GetDriveType(pDrives);

			if (DRIVE_FIXED == nDriveType)
			{
				//strIPAddresses.Add(pDrives);
				wichDrive.addItem(pDrives, ++driveId);
			}
			pDrives += lstrlen(pDrives) + 1;
		}

		free(pDrivesToDelete);
		pDrivesToDelete = NULL;
	}
	//==========================
	inputSearchDirBox.setLookAndFeel(&this->getLookAndFeel());
	inputSearchBox.setLookAndFeel(&this->getLookAndFeel());
	outputRealTime.setLookAndFeel(&this->getLookAndFeel());
	outputRealTime.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::red);
	outputRealTime.setEnabled(false);
	sliderBetweenInputBox.setLookAndFeel(&this->getLookAndFeel());
	sliderBetweenInputBox.setTextBoxStyle(juce::Slider::NoTextBox, false, 0,0);
	sliderBetweenInputBox.setRange(0.0, 1.0, 0.01);
	sliderBetweenInputBox.setScrollWheelEnabled(false);
	sliderBetweenInputBox.setMouseClickGrabsKeyboardFocus(true);
	//sliderBetweenInputBox.setSliderSnapsToMousePosition(false);
	wichDrive.setLookAndFeel(&this->getLookAndFeel());
	addAndMakeVisible(wichDrive);
	if(wichDrive.getNumItems() > 0)
		wichDrive.setSelectedItemIndex(0, true);
	//wichDrive.addItem("D:/", 0);
	//wichDrive.addItem("C:/", 1);
	addAndMakeVisible(sliderBetweenInputBox);
	addAndMakeVisible(inputSearchDirBox); 
	addAndMakeVisible(inputSearchBox);

	buttonSearch.setLookAndFeel(&this->getLookAndFeel());
	buttonSearch.setButtonText(L"Search");




	addAndMakeVisible(buttonSearch);
	buttonSearch.addListener(this);
	buttonSearch.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
	searchResult = new searchResultsComponent({"NR", "DIRECTORY", "FILE NAME","FILE TYPE", "TIME IN MS"});
	addAndMakeVisible(searchResult);

	addAndMakeVisible(inputDirBoxLabel);
	inputDirBoxLabel.setText("Directory", dontSendNotification);
	inputDirBoxLabel.setLookAndFeel(&this->getLookAndFeel());
	inputDirBoxLabel.attachToComponent(&inputSearchDirBox, false);
	//inputDirBoxLabel.setColour(Label::textColourId, Colours::orange);
	inputDirBoxLabel.setJustificationType(Justification::centredTop);
	
	addAndMakeVisible(inputBoxLabel);
	inputBoxLabel.setText("Search", dontSendNotification);
	inputBoxLabel.attachToComponent(&inputSearchBox, false);
	inputBoxLabel.setLookAndFeel(&this->getLookAndFeel());
	inputBoxLabel.setJustificationType(Justification::centredTop);

	flagCaseSensitive.setLookAndFeel(&this->getLookAndFeel());
	flagCaseSensitive.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
	flagCaseSensitive.setButtonText("Case sensitive");
	flagCaseSensitive.changeWidthToFitText();
	addAndMakeVisible(flagCaseSensitive);






	flagMatchWord.setLookAndFeel(&this->getLookAndFeel());
	flagMatchWord.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
	flagMatchWord.setButtonText("Match Word");
	flagMatchWord.changeWidthToFitText();
	addAndMakeVisible(flagMatchWord);
	
	flagIncludeExstensions.setLookAndFeel(&this->getLookAndFeel());
	flagIncludeExstensions.setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
	flagIncludeExstensions.setButtonText("Search with extensions");
	flagIncludeExstensions.changeWidthToFitText();
	addAndMakeVisible(flagIncludeExstensions);
	addAndMakeVisible(outputRealTime);
	
	setSize(800, 600);
}

MainContentComponent::~MainContentComponent(){
	delete(searchResult);
}

void MainContentComponent::paint(Graphics& g)
{
	g.setFont(24.0f);
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	//int barHeight = getHeight() * 1.f / 12.f;
	//g.setFont(Font(16.0f));
	g.setColour(Colours::white);

	double pos = sliderBetweenInputBox.getValue();
	double posTest = sliderBetweenInputBox.getPositionOfValue(pos);

	double offsetX = getWidth() * 0.01f;
	double offsetY = 24;
	inputSearchDirBox.setBounds(offsetX + 70, offsetY, posTest + 60, 24);
	double pos_test = inputSearchDirBox.getPosition().x + inputSearchDirBox.getBounds().getWidth() + 10;
	inputSearchBox.setBounds(pos_test, offsetY, getWidth() - pos_test - offsetX, 24);
	//g.drawFittedText("Directory", inputSearchBox.getPosition().getX(), 0, inputSearchDirBox.getWidth(), 24, juce::Justification::centred, 1);
	//g.drawFittedText("Search", pos_test, 0, inputSearchBox.getWidth(), 24, juce::Justification::centredBottom,1);
	//g.drawFittedText(juce::String(pos), 0, 0, (getWidth() * 1.f / 6.f), barHeight, juce::Justification::centred, true);
	//searchLock.lock();
	//if (files.size() != dataNumber) {
	//	if (searchResult != nullptr) {
	//		searchResult->addItem(files.back());
	//	}
	//	dataNumber = files.size();
	//}
	//searchLock.unlock();
	if(searchStatus.size() == 0)
		outputRealTime.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::green);
	else
		outputRealTime.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::red);
	outputRealTime.setBounds(0, getHeight() - offsetY, getWidth() * mainFactor, offsetY);
}
struct listUpdate {
	searchResultsComponent *searchResult;
	int data;
	std::vector<int> *flagToFinish;
	std::mutex *searchLock;
	std::vector<search::StringArray> *files;
	Button* button;//for reset button, or to indicate that search is finished
	timer::Timer *timer_;
	double* mainFactor;
};
int manualListUpdate(listUpdate data) {


	int sizeOfArray = 0;
	int sizeOfNewArray = 0;
	while (1) {
		if (data.searchLock != nullptr) {
			data.searchLock->lock();
			if(data.searchResult != nullptr)
				sizeOfArray = data.searchResult->getNumOfRows();
			if (data.files != nullptr)
				sizeOfNewArray = data.files->size();
			if (sizeOfArray < sizeOfNewArray && data.searchResult != nullptr && data.files != nullptr) {
				
				for (auto var = data.files->begin()+ sizeOfArray; var != data.files->end(); var++) {
					//var->set(0, String(++data.data));
					var->at(0) = std::to_string(++data.data);
					
					data.searchResult->addItem(*var);
				}
			}
			//std::cout << *data.flagToFinish << std::endl;
			if (data.flagToFinish != nullptr && data.flagToFinish->size() <= 0) {

				if (data.button != nullptr)data.button->setEnabled(true);
				if (data.timer_ != nullptr)data.timer_->stop();
				data.searchLock->unlock();
				return 1;
			}
			data.searchLock->unlock();
		}
	}
	return 0;
}


void MainContentComponent::resized()	
{

	double offsetX = getWidth() * 0.01f;
	double offsetY = 24;
	int barHeight = getHeight() * 1.f / 12.f;
	wichDrive.setBounds(offsetX, offsetY, 70,24);

	flagCaseSensitive.setBounds(offsetX, offsetY * 2, 0, offsetY);
	flagCaseSensitive.changeWidthToFitText();
	
	double offsetFlag1 = flagCaseSensitive.getBounds().getX() + flagCaseSensitive.getBounds().getWidth();
	flagMatchWord.setBounds(offsetFlag1, offsetY * 2, 0, offsetY);
	flagMatchWord.changeWidthToFitText();
	offsetFlag1 = flagMatchWord.getBounds().getX() + flagMatchWord.getBounds().getWidth();
	flagIncludeExstensions.setBounds(offsetFlag1, offsetY * 2, 0, offsetY);
	flagIncludeExstensions.changeWidthToFitText();
	offsetFlag1 = flagIncludeExstensions.getBounds().getX() + flagIncludeExstensions.getBounds().getWidth();
	if (offsetFlag1 > getWidth()) {
		flagIncludeExstensions.setBounds(offsetX, offsetY * 3, 0, offsetY);
		flagIncludeExstensions.changeWidthToFitText();
	}

	if(getWidth() - offsetX - 70 <  offsetFlag1)
		buttonSearch.setBounds(getWidth() - offsetX - 70, offsetY * 3, 70, offsetY);
	else
		buttonSearch.setBounds(getWidth() - offsetX - 70, offsetY * 2, 70, offsetY);
	inputSearchDirBox.setBounds(getWidth() * 0.01f + 70, getWidth() * 0.01f, (getWidth() * 1.f / 6.f), 24);
	inputSearchBox.setBounds(getWidth() * 0.01f + 70, getWidth() * 0.01f, (getWidth() * 1.f / 6.f), 24);

	sliderBetweenInputBox.setBounds(getWidth() * 0.01f + 140 - 5, offsetY, getWidth() - (getWidth() * 0.01f + 210 - 5) , 24);
	outputRealTime.setBounds(0, getHeight() - offsetY, getWidth() * mainFactor, offsetY);
	if (searchResult != nullptr) {
		double yOffset = buttonSearch.getBounds().getY() + buttonSearch.getBounds().getHeight();
		searchResult->setBounds(0, yOffset, getWidth(), getHeight() - yOffset - offsetY);
	}

}


#include <conio.h>
unsigned int maxThreadCount = std::thread::hardware_concurrency();

void MainContentComponent::buttonClicked(Button* button)
{
	if (button == &buttonSearch) {
		int flags = 0;
		flags = flagCaseSensitive.getToggleState() | flagMatchWord.getToggleState()*2 | flagIncludeExstensions.getToggleState() * 4 ;
		//std::cout << (1 & flags) << " " << (2 & flags) << " " << (4 & flags) << std::endl;
		mainFactor = 0;
		search::thread_data data(-1);
		data.maxThreadCount = maxThreadCount;
		data.flags = flags;
		data.ats = &files;
		data.mainFactor = &mainFactor;
		//std::cout << wichDrive.getItemText(wichDrive.getSelectedId()).toStdString()<< std::endl;

		std::string mainDir = wichDrive.getItemText(wichDrive.getSelectedId()-1).toStdString(); 
		std::cout << wichDrive.getSelectedId() << std::endl;
		if(mainDir.size() > 0)
			mainDir.pop_back();
		std::string secDir = inputSearchDirBox.getText().toStdString();
		if (secDir.size() > 0)
			mainDir += '/' + secDir;
		std::cout << mainDir << std::endl;
		//"B:/OneDrive/mokslai/Daugiagijis/searchEngineV1/data";//wichDrive.getItemText(wichDrive.getSelectedId()).toStdString();
		
		//if (mainDir.size() > 0);
		//	mainDir.pop_back();
		//mainDir.push_back('/');
		data.folder = mainDir;
		//+
		//	inputSearchDirBox.getText().toStdString();
		threadId = 0;
		searchStatus.clear();
		files.clear();
		if(searchResult != nullptr)
			searchResult->clearTable();
		data.search = inputSearchBox.getText().toStdString();
		data.child = false;
		data.ISthread = true;
		timer1.start();
		data.timer1 = &timer1;
		data.threadId = &threadId;
		data.searchLock = &searchLock;
		data.firstRun = true;
		data.factor = 1;
		data.status = &searchStatus;
		listUpdate dataManual;
		dataManual.data = 0;
		dataManual.files = &files;
		dataManual.flagToFinish = &searchStatus;
		dataManual.searchLock = &searchLock;
		dataManual.searchResult = searchResult;
		dataManual.timer_ = &timer1;
		dataManual.button = &buttonSearch;
		dataManual.mainFactor = &mainFactor;
		
		searchStatus.push_back(-1);

		std::thread manual(manualListUpdate, dataManual);
		manual.detach();
		searchThread = std::thread(search::search_folder, data);
		searchThread.detach();
		button->setEnabled(false);
		searchStatus.pop_back();
		//for (auto var : files) {
		//	std::cout << var.joinIntoString(" _ ") << std::endl;
		//}


		//// 000  001 Extensions, 010 Match word, 100 case sensitive
		//std::cout << "Flags: " <<std::bitset<3>(flags)<<" " <<wichDrive.getItemText(wichDrive.getSelectedId()) << inputSearchDirBox.getText() << " _ "\
		//	<< inputSearchBox.getText() << std::endl;
	}
	
}