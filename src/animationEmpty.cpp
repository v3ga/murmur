//
//  animationEmpty.cpp
//  murmur
//
//  Created by Julien on 15/10/2015.
//
//

#include "animationEmpty.h"

AnimationEmpty::AnimationEmpty(string name) : Animation(name)
{
}


void AnimationEmpty::VM_draw(float w, float h)
{
	ofBackground(0);
}
