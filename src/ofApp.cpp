#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (isOpen && input != nullptr) {
		int cellSize = 30; // 픽셀 크기
		for (i = 0; i < maze_row; i++) {
			for (j = 0; j < maze_col; j++) {
				char c = input[i][j];
				if (c == ' ') {
					ofSetColor(255); // 통로: 흰색
				}
				else {
					ofSetColor(0);   // 벽: 검정
				}
				ofDrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize);
			}
		}
	}
	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile() {
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	if (!openFileResult.bSuccess) {
		cout << "File dialog canceled." << endl;
		return false;
	}

	string filePath = openFileResult.getPath();
	size_t pos = filePath.find_last_of(".");
	if (pos == string::npos || filePath.substr(pos + 1) != "maz") {
		cout << "Invalid file extension. Expected '.maz'" << endl;
		return false;
	}

	ofFile file(filePath);
	if (!file.exists()) {
		cout << "Target file does not exist." << endl;
		return false;
	}

	// 기존 데이터 해제
	freeMemory();

	ofBuffer buffer(file);

	// Step 1: HEIGHT, WIDTH 계산
	maze_row = 0;
	maze_col = 0;
	for (auto& line : buffer.getLines()) {
		maze_row++;
		maze_col = max(maze_col, (int)line.length());
	}

	// Step 2: input 동적할당 및 저장
	input = (char**)malloc(sizeof(char*) * maze_row);
	for (int i = 0; i < maze_row; i++) {
		input[i] = (char*)malloc(sizeof(char) * maze_col);
		memset(input[i], ' ', maze_col); // 기본 공백
	}

	int i = 0;
	for (auto& line : buffer.getLines()) {
		for (int j = 0; j < line.length(); j++) {
			input[i][j] = line[j];
		}
		i++;
	}

	// Step 3: adjList 생성
	adjList = (node**)malloc(sizeof(node*) * maze_row * maze_col);
	for (int i = 0; i < maze_row * maze_col; i++) {
		adjList[i] = NULL;
	}

	node* temp;
	for (int i = 1; i < maze_row - 1; i += 2) {
		for (int j = 1; j < maze_col - 1; j += 2) {
			int curIdx = i * maze_col + j;

			// up
			if (input[i - 1][j] == ' ') {
				temp = (node*)malloc(sizeof(node));
				temp->v = (i - 2) * maze_col + j;
				temp->link = adjList[curIdx];
				adjList[curIdx] = temp;
			}

			// down
			if (input[i + 1][j] == ' ') {
				temp = (node*)malloc(sizeof(node));
				temp->v = (i + 2) * maze_col + j;
				temp->link = adjList[curIdx];
				adjList[curIdx] = temp;
			}

			// left
			if (input[i][j - 1] == ' ') {
				temp = (node*)malloc(sizeof(node));
				temp->v = i * maze_col + j - 2;
				temp->link = adjList[curIdx];
				adjList[curIdx] = temp;
			}

			// right
			if (input[i][j + 1] == ' ') {
				temp = (node*)malloc(sizeof(node));
				temp->v = i * maze_col + j + 2;
				temp->link = adjList[curIdx];
				adjList[curIdx] = temp;
			}
		}
	}

	isOpen = 1;
	printf("Maze loaded. Size: %d rows × %d columns\n", maze_row, maze_col);
	return true;
}


void ofApp::freeMemory() {
	//TO DO
	// malloc한 memory를 free해주는 함수
	int i;
	if (input != NULL) {
		for (i = 0; i < maze_row; i++) {
			free(input[i]);
		}
		free(input);
		input = NULL;
	}
	if (adjList != NULL) {
		for (int i = 0; i < maze_row * maze_col; i++) {
			node* cur = adjList[i];
			while (cur != NULL) {
				node* prev = cur;
				cur = cur->link;
				free(prev);
			}
		}
		free(adjList);
		adjList = NULL;
	}

	maze_row = 0;
	maze_col = 0;
	
	printf("[INFO] Memory freed for previous maze data.\n");
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	return false;
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
}
