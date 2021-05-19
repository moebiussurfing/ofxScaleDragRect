//
//  interactiveRect.h
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//  Updated by moebiusSurfing 2021
//


/*

TODO:

+ lock x,y,w,h upper/bottom drag borders
+ lock min/max size for the rect and avoid flipping..
+ add mouse drag scale from center (get from ofxSCENE-SVG
+ add helpers to fit/rescale..

*/


#pragma once

#include "ofMain.h"

#define BORDER_DRAG_SIZE 15

class ofxInteractiveRect : public ofRectangle
{

private:
	ofColor colorBorderDraggable = { ofColor::yellow };//draggable borders
	ofColor colorEditingHover{ ofColor(50, 70) };
	ofColor colorEditingPressedBorder{ ofColor(50, 200) };//borders
	ofColor colorEditingMoving{ ofColor(127, 127) };//borders. fill color when hover and moving the rectangle

	bool bDrawBroder = true;
	
	//rounded
	bool bRounded = false;
	float rounded = 5.0;
public:
	void setRounded(float r) {
		if (r > 0) bRounded = true;
		else bRounded = false;
		rounded = r;
	}

	//TODO:
private:
	bool bLockX = false;
	bool bLockY = false;
	bool bLockW = false;
	bool bLockH = false;

public:
	void setLockX(bool b) { bLockX = b; };
	void setLockY(bool b) { bLockY = b; };
	void setLockW(bool b) { bLockW = b; };
	void setLockH(bool b) { bLockH = b; };

public:
	ofParameter<bool> bLockResize{ "Lock Resize",false };
	ofParameter<bool> bLock{ "Lock",false };
	void setLockResize(bool b) {
		bLockResize = b;
	}
	void setLock(bool b) {
		bLock = b;
	}

	void setColorBorderDraggable(const ofColor c) {
		colorBorderDraggable = c;
	}
	void setColorEditingHover(const ofColor c) {
		colorEditingHover = c;
	}
	void setColorEditingPressedBorder(const ofColor c) {
		colorEditingPressedBorder = c;
	}
	void setColorEditingMoving(const ofColor c) {
		colorEditingMoving = c;
	}

	bool isMouseOver() {
		bool b = inside(ofGetMouseX(), ofGetMouseY());

		return b;
	}
private:
	bool bAllScreenMouse = false;

public:
	void setAllScreenMouse(bool b) {
		bAllScreenMouse = b;
	}

	ofxInteractiveRect(string name);
	virtual ~ofxInteractiveRect();

	void enableEdit(bool enable = true);
	void disableEdit();
	void toggleEdit();
	bool isEditing() { return bIsEditing; }

	void draw();
	void drawBorder();

	bool loadSettings(string name = "", string path = "", bool loadJson = false);
	void saveSettings(string name = "", string path = "", bool saveJson = false);

	//TODO:
	//store name and path to allow autosave..
private:
	bool bAutoSave = true;

public:
	void setAutoSave(bool b = true) {
		bAutoSave = b;
	}

public:
//private:
	void mouseMoved(ofMouseEventArgs & mouse);
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);

	void mouseScrolled(ofMouseEventArgs & mouse);
	void mouseEntered(ofMouseEventArgs & mouse);
	void mouseExited(ofMouseEventArgs & mouse);

public:
	float getRectX() { return x; }
	float getRectY() { return y; }
	float getRectWidth() { return width; }
	float getRectHeight() { return height; }

private:
	string name;
	string path;

public:
	ofRectangle getRect();
	void setRect(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

private:
	ofJson toJson();
	void fromJson(const ofJson& j);

	ofXml toXml();
	bool fromXml(const ofXml& x);

public:
	ofParameter<bool> bEditMode{ "EDIT BOX", false };
	void Changed_EditMode(bool & b);

protected:
	bool bIsEditing;
	bool bMove;
	bool bIsOver;
	bool bLeft, bRight, bUp, bDown;
	bool bPressed;
	glm::vec2 mousePrev;

	std::string prefixName = "Rectangle_";

	//--

	// restore/reset position and shape
public:
	void setCentered()
	{
		setPosition(ofGetWidth() / 2 - getWidth() / 2, ofGetHeight() / 2 - getHeight() / 2);
	}
	void setRestoreSize()
	{
		setWidth(800);
		setHeight(800 / (16 / 9.f));
	}
	void setRestore()
	{
		setRestoreSize();
		setCentered();
	}
};
