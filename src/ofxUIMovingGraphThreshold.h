//
//  ofxUIMovingGraphThreshold.h
//  murmur
//
//  Created by Julien on 09/10/2014.
//
//

#pragma once 
#include "ofxUIMovingGraph.h"

class ofxUIMovingGraphThreshold : public ofxUIMovingGraph
{
	public:
		    ofxUIMovingGraphThreshold(float x, float y, float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name);
    		ofxUIMovingGraphThreshold(float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name);
 
	
		virtual void 		drawFill		();
				void		setThreshold	(float value);
				void		setValue		(float value){m_value = value;}

	private:
		float				m_thresholdValue;
		float				m_value;
};