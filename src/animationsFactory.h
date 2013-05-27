//
//  animationsFactory.h
//  murmur
//
//  Created by Julien on 16/05/13.
//
//

#pragma once
#include "ofMain.h"

class Animation;
class AnimationsFactory
{
    public:
        static Animation* create(string);
};