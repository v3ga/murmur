//
//  RadarFFElement.h
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#pragma once
#include "ofMain.h"

class RadarFFElement
{
	public:
	   RadarFFElement			(float x, float y);
	   ~RadarFFElement			();
	
		// Type
 		int						m_type;
		enum{
			TYPE_SIMPLE			= 0,
			TYPE_FILLED			= 1,
			TYPE_DOTTED			= 2,
			TYPE_RADIAL			= 3
		};
 
 
		// Type data
		ofPolyline				m_arc;
	
		// Color
		ofColor					m_color;
 
		// Position of emission
		ofVec2f					m_pos;

		// Radius
 		float 					m_radius;
		float					m_radiusWidth;
		float					m_radiusSpeed;

		// Angle span (in degrees)
		float					m_angleBegin;
		float					m_angleEnd;


		// Rotation around z-axis
		float					m_rot;
 
		virtual void			draw					();
		virtual void			update					(float dt);
		virtual void			updateCustom			(float dt);
};

