//
//  animationLinesFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once

#include "animations.h"

class AnimationLinesFF : public Animation
{
	public:
		AnimationLinesFF			(string name);
		~AnimationLinesFF			();
 
 
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
};
