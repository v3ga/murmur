//
//  murmur.h
//  murmur
//
//  Created by Julien on 29/01/2014.
//
//

#pragma once

#include "ofMain.h"
#if defined(TARGET_OSX)
	#define MURMUR_USE_SYPHON 1
#else
	#define MURMUR_USE_SYPHON 0
#endif