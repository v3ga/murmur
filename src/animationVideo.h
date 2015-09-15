//
//  animationVideo.h
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#pragma once
#include "animations.h"
#include "ofxHapPlayer.h"

class AnimationVideo : public Animation
{
	public:
		AnimationVideo			(string name);
		~AnimationVideo			();
 
		void					setVideo			(string name);
    
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
 
		ofxHapPlayer			m_player;
 		bool					m_bLoadVideo;
 
};

// A_drone_in_iceland_hap.mov