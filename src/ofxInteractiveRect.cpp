//
//  interactiveRect.cpp
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//	Updated by moebiusSurfing 2021

#include "ofxInteractiveRect.h"

//--------------------------------------------------------------
ofxInteractiveRect::ofxInteractiveRect(string name)
{
	bEditMode.addListener(this, &ofxInteractiveRect::Changed_EditMode);

	bLockResize = false;

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
	ofLogVerbose(__FUNCTION__) << "interactiveRect " << this->name <<" enableEdit " << (string)(enable?"true":"false");
    
    if (enable != bIsEditing)
    {
        if (enable)
        {
            ofRegisterMouseEvents(this);
        }
        else
        {
            ofUnregisterMouseEvents(this);
          //  saveSettings();
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
    if(name!="")
    {
        this->name = name;
    }
    
    if (path!="")
    {
        this->path = path;
    }

	string filename = this->path + prefixName + this->name ;
	 
	if(saveJson){
		filename += ".json";
		ofSaveJson(filename, toJson());
	}else{
		filename += ".xml";
		toXml().save(filename);
	}
	
	ofLogVerbose(__FUNCTION__) << "saved settings: "<< filename;
}


ofJson ofxInteractiveRect::toJson()
{
	ofJson j;//("interactiveRect");

	j["x"] = this->ofRectangle::x;
	j["y"] = this->ofRectangle::y;
	j["width"] =  this->ofRectangle::width;
	j["height"] =  this->ofRectangle::height;
	j["name"] =  this->name;
	j["path"] =  this->path;
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
	if(r)
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
    if(name!="")
    {
        this->name = name;
    }
    
    if (path!="")
    {
        this->path = path;
    }

	string filename = this->path + prefixName + this->name ;
	 
	if(loadJson)
	{
		filename += ".json";
		
		fromJson(ofLoadJson(filename));
		return true;
		
	}else{
		filename += ".xml";
		ofXml xml;
		if(xml.load(filename))
		{
			if(fromXml(xml))
			{
				return true;
			}
		}
	}
	ofLogVerbose(__FUNCTION__) << "unable to load : "<< filename;
	
	return false;
}

//--------------------------------------------------------------
void ofxInteractiveRect::draw()
{
	if (bIsEditing)
    {
        ofPushStyle();

		if (bIsOver)
        {
			if (bPressed)
            {
				ofSetColor(colorEditingPressed);
			}
            else
            {
				ofSetColor(colorEditingHover);//alpha not working?
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
            ofSetColor(colorBorder.r, colorBorder.g, colorBorder.b, colorBorder.a * 0.5);

            if (bUp)
            {
                ofDrawRectangle(x, y, width, BORDER_SIZE);
            }
            else if(bDown)
            {
                ofDrawRectangle(x, y + height - BORDER_SIZE, width, BORDER_SIZE);
            }
            
            if (bLeft)
            {
                ofDrawRectangle(x, y, BORDER_SIZE, height);
            }
            else if(bRight)
            {
                ofDrawRectangle(x + width - BORDER_SIZE, y, BORDER_SIZE, height);
            }
        }

        ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseMoved(ofMouseEventArgs & mouse)
{
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
				if (mouse.x < x + BORDER_SIZE && mouse.x > x)
				{
					bLeft = true;
					bMove = false;
				}
				else if (mouse.x < x + width && mouse.x > x + width - BORDER_SIZE)
				{
					bRight = true;
					bMove = false;
				}

				if (mouse.y > y && mouse.y < y + BORDER_SIZE)
				{
					bUp = true;
					bMove = false;
				}
				else if (mouse.y > y + height - BORDER_SIZE && mouse.y < y + height)
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
			if (mouse.x < x + BORDER_SIZE && mouse.x > x)
			{
				bLeft = true;
				bMove = false;
			}
			else if (mouse.x < x + width && mouse.x > x + width - BORDER_SIZE)
			{
				bRight = true;
				bMove = false;
			}
			if (mouse.y > y && mouse.y < y + BORDER_SIZE)
			{
				bUp = true;
				bMove = false;
			}
			else if (mouse.y > y + height - BORDER_SIZE && mouse.y < y + height)
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
	//if (!bLockResize) 
	{
		if (bUp)
		{
			y += mouse.y - mousePrev.y;
			height += mousePrev.y - mouse.y;
		}
		else if (bDown)
		{
			height += mouse.y - mousePrev.y;
		}
		if (bLeft)
		{
			x += mouse.x - mousePrev.x;
			width += mousePrev.x - mouse.x;
		}
		else if (bRight)
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