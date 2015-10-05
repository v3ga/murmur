//
//  LineFFElement.h
//  murmur
//
//  Created by Julien on 25/09/2015.
//
//

#pragma once
#include "ofMain.h"

class AnimationLinesFF;
class LineFFElement
{
	public:
		LineFFElement					(AnimationLinesFF* pParent, ofVec3f pos, ofVec3f dir);
	
 
 		void		update				(float dt);
		void		draw				();

		AnimationLinesFF* 	mp_parent;

		ofColor				m_color;
		ofVec3f				m_pos, m_dir;
		float				m_rot;
		float				m_radius, m_radiusTarget;

		ofMesh*				mp_meshPlane;
};