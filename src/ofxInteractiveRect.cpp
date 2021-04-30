//
//  interactiveRect.cpp
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//	Updated by moebiusSurfing 2021

#include "ofxInteractiveRect.h"

//------------------------------------------------------------------------------
	//animator functions taken from 
	//https://github.com/tcoppex/ofxFontSampler/blob/main/example/src/ofApp.cpp
namespace {

	/* Return a linear value in range [0,1] every delay (in seconds). */
	float Tick(float delay) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}

	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	float Bounce(float delay) {
		return 0.5f*(1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
	}

	/* Noise function used by the gradient scaling. */
	float Noise(const ofPoint &vertex) {
		return /*24.0f **/ ofNoise(0.005f*vertex + 0.5f*ofGetElapsedTimeMillis()*0.0002f);
	}

	static constexpr int32_t kCharsetSize = 26;

}  // namespace

//--------------------------------------------------------------
ofxInteractiveRect::ofxInteractiveRect(string name)
{
	bEditMode.addListener(this, &ofxInteractiveRect::Changed_EditMode);

	bLockResize = false;
	bLock = false;

	bIsEditing = false;
	bMove = false;
	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;
	bIsOver = false;
	this->name = name;
	this->path = "";
}

//--------------------------------------------------------------
ofxInteractiveRect::~ofxInteractiveRect()
{
	bEditMode.removeListener(this, &ofxInteractiveRect::Changed_EditMode);
}

//--------------------------------------------------------------
void ofxInteractiveRect::enableEdit(bool enable)
{
	ofLogVerbose(__FUNCTION__) << "interactiveRect " << this->name << " enableEdit " << (string)(enable ? "true" : "false");

	if (enable != bIsEditing)
	{
		if (enable)
		{
			ofRegisterMouseEvents(this);
		}
		else
		{
			ofUnregisterMouseEvents(this);
			//saveSettings();
		}
		bIsEditing = enable;

		bEditMode.setWithoutEventNotifications(bIsEditing);
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::disableEdit()
{
	enableEdit(false);
}

//--------------------------------------------------------------
void ofxInteractiveRect::toggleEdit()
{
	enableEdit(!bIsEditing);
}

//--------------------------------------------------------------
ofRectangle ofxInteractiveRect::getRect()
{
	return ofRectangle(x, y, width, height);
}

//--------------------------------------------------------------
void ofxInteractiveRect::saveSettings(string name, string path, bool saveJson)
{
	if (name != "")
	{
		this->name = name;
	}

	if (path != "")
	{
		this->path = path;
	}

	string filename = this->path + prefixName + this->name;

	if (saveJson) {
		filename += ".json";
		ofSaveJson(filename, toJson());
	}
	else {
		filename += ".xml";
		toXml().save(filename);
	}

	ofLogVerbose(__FUNCTION__) << "saved settings: " << filename;
}

ofJson ofxInteractiveRect::toJson()
{
	ofJson j;//("interactiveRect");

	j["x"] = this->ofRectangle::x;
	j["y"] = this->ofRectangle::y;
	j["width"] = this->ofRectangle::width;
	j["height"] = this->ofRectangle::height;
	j["name"] = this->name;
	j["path"] = this->path;
	j["isEditing"] = this->bIsEditing;
	return j;
}

void ofxInteractiveRect::fromJson(const ofJson& j)
{
	j["x"].get_to(this->ofRectangle::x);
	j["y"].get_to(this->ofRectangle::y);
	j["width"].get_to(this->ofRectangle::width);
	j["height"].get_to(this->ofRectangle::height);
	j["name"].get_to(this->name);
	j["path"].get_to(this->path);
	bool editing;

	j["isEditing"].get_to(editing);
	enableEdit(editing);
}

ofXml ofxInteractiveRect::toXml()
{
	ofXml xml;
	auto r = xml.appendChild("interactiveRect");

	r.appendChild("path").set(this->path);
	r.appendChild("x").set(this->ofRectangle::x);
	r.appendChild("y").set(this->ofRectangle::y);
	r.appendChild("width").set(this->ofRectangle::width);
	r.appendChild("height").set(this->ofRectangle::height);
	r.appendChild("name").set(this->name);
	r.appendChild("isEditing").set(this->bIsEditing);

	return xml;
}

bool ofxInteractiveRect::fromXml(const ofXml& xml)
{
	auto r = xml.getChild("interactiveRect");
	if (r)
	{
		this->path = r.getChild("path").getValue();
		this->ofRectangle::x = r.getChild("x").getFloatValue();
		this->ofRectangle::y = r.getChild("y").getFloatValue();
		this->ofRectangle::width = r.getChild("width").getFloatValue();
		this->ofRectangle::height = r.getChild("height").getFloatValue();
		this->name = r.getChild("name").getValue();
		enableEdit(r.getChild("isEditing").getBoolValue());

		return true;
	}

	return false;
}

//--------------------------------------------------------------
bool ofxInteractiveRect::loadSettings(string name, string path, bool loadJson)
{
	if (name != "")
	{
		this->name = name;
	}

	if (path != "")
	{
		this->path = path;
	}

	string filename = this->path + prefixName + this->name;

	if (loadJson)
	{
		filename += ".json";

		fromJson(ofLoadJson(filename));
		return true;

	}
	else {
		filename += ".xml";
		ofXml xml;
		if (xml.load(filename))
		{
			if (fromXml(xml))
			{
				return true;
			}
		}
	}
	ofLogVerbose(__FUNCTION__) << "unable to load : " << filename;

	return false;
}

//TODO:
//--------------------------------------------------------------
void ofxInteractiveRect::drawBorder()
{
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2.0);
	ofSetColor(colorEditingMoving);
	ofDrawRectangle(*this);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxInteractiveRect::draw()
{
	//TODO:
	//lock 
	//slows fps..?
	//if (this->getHeight() < 100) this->setHeight(100);

	if (bIsEditing)
	{
		ofPushStyle();

		if (bIsOver)
		{
			if (bPressed)
			{
				ofSetColor(colorEditingPressedBorder);
			}
			else
			{
				ofSetColor(colorEditingHover);
			}
			ofNoFill();
			ofDrawRectangle(*this);
		}

		ofFill();
		if (bMove)
		{
			ofSetColor(colorEditingMoving);
			ofDrawRectangle(*this);
		}
		else
		{
			ofSetColor(colorBorderDraggable.r, colorBorderDraggable.g, colorBorderDraggable.b, colorBorderDraggable.a * 0.5);

			if (bUp)
			{
				ofDrawRectangle(x, y, width, BORDER_DRAG_SIZE);
			}
			else if (bDown)
			{
				ofDrawRectangle(x, y + height - BORDER_DRAG_SIZE, width, BORDER_DRAG_SIZE);
			}
			if (bLeft)
			{
				ofDrawRectangle(x, y, BORDER_DRAG_SIZE, height);
			}
			else if (bRight)
			{
				ofDrawRectangle(x + width - BORDER_DRAG_SIZE, y, BORDER_DRAG_SIZE, height);
			}
		}

		ofPopStyle();
	}

	//-

	////TODO:
	////border
	//if (bDrawBroder)
	//{
	//	ofPushStyle();

	//	ofNoFill();
	//	ofSetLineWidth(2);
	//	ofColor _c = ofColor(255, 64);
	//	//ofColor _c = colorBorderDraggable;
	//	if (bIsEditing)
	//	{
	//		float v = ofMap(Bounce(0.5), 0, 1, 0.25, 1);
	//		_c.set(_c.r, _c.g, _c.b, _c.a*v);
	//	}
	//	ofSetColor(_c);
	//	ofDrawRectangle(*this);

	//	ofPopStyle();
	//}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseMoved(ofMouseEventArgs & mouse)
{
	if (bLock) return;

	if (!bPressed)
	{
		bIsOver = inside(mouse.x, mouse.y);

		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;

		if (bIsOver)
		{
			bMove = true;

			if (!bLockResize)
			{
				if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW)
				{
					bLeft = true;
					bMove = false;
				}
				else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW)
				{
					bRight = true;
					bMove = false;
				}

				if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH)
				{
					bUp = true;
					bMove = false;
				}
				else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH)
				{
					bDown = true;
					bMove = false;
				}
			}
		}
		else
		{
			bMove = false;
		}
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mousePressed(ofMouseEventArgs & mouse)
{
	if (bLock) return;

	mousePrev = mouse;
	bPressed = true;

	if (!bAllScreenMouse) bIsOver = inside(mouse.x, mouse.y);
	else bIsOver = true;

	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;

	if (bIsOver)
	{
		bMove = true;

		if (!bLockResize)
		{
			if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW)
			{
				bLeft = true;
				bMove = false;
			}
			else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW)
			{
				bRight = true;
				bMove = false;
			}
			if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH)
			{
				bUp = true;
				bMove = false;
			}
			else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH)
			{
				bDown = true;
				bMove = false;
			}
		}
	}
	else
	{
		bMove = false;
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseDragged(ofMouseEventArgs & mouse)
{
	if (bLock) return;

	//if (!bLockResize) 
	{
		if (bUp && !bLockX)
		{
			y += mouse.y - mousePrev.y;
			height += mousePrev.y - mouse.y;
		}
		else if (bDown && !bLockY)
		{
			height += mouse.y - mousePrev.y;
		}
		if (bLeft && !bLockW)
		{
			x += mouse.x - mousePrev.x;
			width += mousePrev.x - mouse.x;
		}
		else if (bRight && !bLockH)
		{
			width += mouse.x - mousePrev.x;
		}
	}

	if (bMove)
	{
		x += mouse.x - mousePrev.x;
		y += mouse.y - mousePrev.y;
	}

	mousePrev = mouse;
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseReleased(ofMouseEventArgs & mouse)
{
	if (bLock) return;

	//if (!bLockResize) 
	{
		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;
	}
	bMove = false;
	bPressed = false;

	//clamp inside the window
	int _min = 20;
	width = ofClamp(width, _min, ofGetWidth());
	height = ofClamp(height, _min, ofGetHeight());
}

//--------------------------------------------------------------
void ofxInteractiveRect::Changed_EditMode(bool & b)
{
	enableEdit(b);
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseScrolled(ofMouseEventArgs & mouse) {}
void ofxInteractiveRect::mouseEntered(ofMouseEventArgs & mouse) {}
void ofxInteractiveRect::mouseExited(ofMouseEventArgs & mouse) {}