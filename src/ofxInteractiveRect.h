//
//  interactiveRect.h
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//

#pragma once

#include "ofMain.h"

#define BORDER_SIZE 10

class ofxInteractiveRect : public ofRectangle
{

private:
	ofColor colorBorder = { ofColor::yellow };//draggable borders
	ofColor colorEditingHover{ ofColor(50, 70) };
	ofColor colorEditingPressed{ ofColor(50, 200) };//borders
	ofColor colorEditingMoving{ ofColor(127, 127) };//borders

public:
	ofParameter<bool> bLockResize{ "Lock Resize",false };
	void setLockResize(bool b) {
		bLockResize = b;
	}

	void setColorBorder(const ofColor c) {
		colorBorder = c;
	}
	void setColorEditingHover(const ofColor c) {
		colorEditingHover = c;
	}
	void setColorEditingPressed(const ofColor c) {
		colorEditingPressed = c;
	}
	void setColorEditingMoving(const ofColor c) {
		colorEditingMoving = c;
	}
	
	bool isMouseOver() {
		bool b = inside(ofGetMouseX(), ofGetMouseY());

		return b;
	}

	bool bAllScreenMouse = false;
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

	bool loadSettings(string name = "", string path = "", bool loadJson = false);
	void saveSettings(string name = "", string path = "", bool saveJson = false);

	void mouseMoved(ofMouseEventArgs & mouse);
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);

	void mouseScrolled(ofMouseEventArgs & mouse);
	void mouseEntered(ofMouseEventArgs & mouse);
	void mouseExited(ofMouseEventArgs & mouse);

	float getRectX() { return x; }
	float getRectY() { return y; }
	float getRectWidth() { return width; }
	float getRectHeight() { return height; }
	string name;
	string path;

	ofRectangle getRect();
	void setRect(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	ofJson toJson();
	void fromJson(const ofJson& j);

	ofXml toXml();
	bool fromXml(const ofXml& x);

public:
	ofParameter<bool> bEditMode{ "Edit Rectangle", false };
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

	// restore
	void setCentered()
	{
		setWidth(800);
		setHeight(800 / (16 / 9.f));
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
