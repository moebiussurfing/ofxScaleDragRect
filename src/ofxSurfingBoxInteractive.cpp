#include "ofxSurfingBoxInteractive.h"

////--------------------------------------------------------------
//ofxSurfingBoxInteractive::ofxSurfingBoxInteractive(){
//	//ofAddListener(ofEvents().update, this, &ofxSurfingBoxInteractive::update);
//	//ofAddListener(ofEvents().keyPressed, this, &ofxSurfingBoxInteractive::keyPressed);
//}
//
////--------------------------------------------------------------
//ofxSurfingBoxInteractive::~ofxSurfingBoxInteractive(){
//	//ofRemoveListener(ofEvents().update, this, &ofxSurfingBoxInteractive::update);
//	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingBoxInteractive::keyPressed);
//}

//--------------------------------------------------------------
void ofxSurfingBoxInteractive::setup(){
	float _x, _y, _w, _h;
	_x = this->x;
	_y = this->y;
	_w = this->getWidth();
	_h = this->getHeight();

	clicker.set(_x, _y, _w, _h);//full screen
	//clicker.set(0, 0, ofGetWidth(), ofGetHeight());//full screen
	clicker.setDebug(false);
}

void ofxSurfingBoxInteractive::drawDebug() {

	// flip bg colors on double and triple click
	if (clicker.isMouseDoubleClick()) bState1 = !bState1;
	if (clicker.isMouseTripleClick()) bState2 = !bState2;

	if (bState2) ofClear(bState1 ? ofColor::blue : ofColor::yellow);
	else ofClear(bState1 ? ofColor::black : ofColor::white);

	stringstream s;
	s << "DoubleClick changes color." << endl;
	s << "TripleClick changes RGB/Black&White." << endl;
	s << "number Clicks: " << clicker.clickCounter << endl;
	//NOTE:can't check these easycallbakcs twice on same frame
	//s << "doubleClicked: " << "[" << (obj.isMouseDoubleClick() ? "x" : " ") << "]" << endl;
	//s << "tripleClicked: " << "[" << (obj.isMouseTripleClick() ? "x" : " ") << "]" << endl;
	s << endl;
	s << "isMouseOver: " << clicker.isMouseOver() << endl;
	s << "isMousePressed(0): " << clicker.isMousePressed(0) << endl;
	s << "isMousePressed(1): " << clicker.isMousePressed(1) << endl;
	s << "isMousePressed(2): " << clicker.isMousePressed(2) << endl;
	s << "getStateChangeMillis(): " << clicker.getStateChangeMillis();
	ofDrawBitmapStringHighlight(s.str(), 10, 30);
}

////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::update(){
//}

////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::update(ofEventArgs & args)
//{
//}
//
////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::draw(){
//}
//
////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::keyPressed(ofKeyEventArgs &eventArgs)
//{
//	const int key = eventArgs.key;
//
//	// modifiers
//	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
//	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
//	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
//	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
//
//	ofLogNotice(__FUNCTION__) << " : " << key;
//
//	//--
//
//	if (false){}
//
//	else if (key == 'G' && !mod_ALT)
//	{
//	}
//}
//
////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::exit(){
//}
//
////--------------------------------------------------------------
//void ofxSurfingBoxInteractive::setBool(bool b){
//		ofLogNotice(__FUNCTION__) << ofToString(b?"true":"false");
//}
//
////--------------------------------------------------------------
//bool ofxSurfingBoxInteractive::getBool(){
//	return true;
//}

