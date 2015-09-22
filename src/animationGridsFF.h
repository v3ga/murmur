//
//  animationGridsFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once
#include "animations.h"


class AnimationGridsFF : public Animation
{
	public:
		AnimationGridsFF			(string name);
		~AnimationGridsFF			();
 
 
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
};
