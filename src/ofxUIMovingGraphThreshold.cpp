//
//  ofxUIMovingGraphThreshold.cpp
//  murmur
//
//  Created by Julien on 09/10/2014.
//
//

#include "ofxUIMovingGraphThreshold.h"


//--------------------------------------------------------------
ofxUIMovingGraphThreshold::ofxUIMovingGraphThreshold(float x, float y, float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name)
: ofxUIMovingGraph(x,y,w,h,_buffer,_bufferSize,_min,_max,_name)
{
	setThreshold(0.0f);
	m_value = 0.0f;
}

//--------------------------------------------------------------
ofxUIMovingGraphThreshold::ofxUIMovingGraphThreshold(float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name)
: ofxUIMovingGraph(w,h,_buffer,_bufferSize,_min,_max,_name)
{
	setThreshold(0.0f);
	m_value = 0.0f;
}

//--------------------------------------------------------------
void ofxUIMovingGraphThreshold::setThreshold(float _value)
{
	m_thresholdValue = _value;
	if (m_thresholdValue<getMin()){
		ofLog()<< "setting thresold to mid range (value too low)";
		m_thresholdValue = ofLerp(getMin(),getMax(),0.5f);
	}
	else
	if (m_thresholdValue>getMax()){
		ofLog()<< "setting thresold to mid range (value too high)";
		m_thresholdValue = ofLerp(getMin(),getMax(),0.5f);
	}
}


//--------------------------------------------------------------
void ofxUIMovingGraphThreshold::drawFill()
{
	ofxUIMovingGraph::drawFill();

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(rect->getX(), rect->getY()+scale, 0);
	ofSetLineWidth(1.5);


	float yTh = ofMap(m_thresholdValue, getMin(), getMax(), scale, -scale);
	ofSetColor(200,0,0);
	ofLine(0,yTh,rect->getWidth(),yTh);

	float yVal = ofMap(m_value, getMin(), getMax(), scale, -scale);
	ofSetColor(200,200,0);
	ofLine(0,yVal,rect->getWidth(),yVal);


	ofPopMatrix();
	ofPopStyle();
}


/*
    if(draw_fill)
    {
        ofNoFill();
        if(draw_fill_highlight)
        {
            ofxUISetColor(color_fill_highlight);
        }
        else
        {
            ofxUISetColor(color_fill);
        }
        ofPushMatrix();
        ofTranslate(rect->getX(), rect->getY()+scale, 0);
        ofSetLineWidth(1.5);
        ofBeginShape();
        for (unsigned int i = 0; i < bufferSize; i++)
        {
            ofVertex(inc*(float)i, ofxUIMap(buffer[i], min, max, scale, -scale, true));
        }
        ofEndShape();
        ofSetLineWidth(1);
        ofPopMatrix();
    }
*/