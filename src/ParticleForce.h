//
//  ParticleForce.h
//  murmur
//
//  Created by Julien on 02/10/2015.
//
//

#pragma once
#include "ofMain.h"

class ParticleForce
{
	public:
		ParticleForce		(float anchorx, float anchory);

	    ofVec2f             m_anchor;
		float				m_volume, m_volumeTarget;
		ofColor				m_color;
	
		void				update					(float dt);
};

