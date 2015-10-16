//
//  animationEmpty.h
//  murmur
//
//  Created by Julien on 15/10/2015.
//
//

#pragma once

#include "animations.h"
#include "ofxHapPlayer.h"

class AnimationEmpty : public Animation
{
	public:
		AnimationEmpty			(string name);


        virtual void			VM_draw					(float w, float h);

};