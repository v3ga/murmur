//
//  RadarFFElement.h
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#pragma once
#include "ofMain.h"

class AnimationRadarFF;
class RadarFFElement
{
	public:
	   RadarFFElement			(AnimationRadarFF* pParent, float x, float y);
	   virtual ~RadarFFElement	();
 
	   // Animation parent
	   AnimationRadarFF*		mp_parent;
	
		// Age
		float					m_age;
	
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
		float					m_rotSpeed;
 
		// Struct. to compute points along arc
		ofPolyline				m_arc;

 
		virtual void			draw					();
		virtual void			update					(float dt);
		virtual void			updateCustom			(float dt);
};

