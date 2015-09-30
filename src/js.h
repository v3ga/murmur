/*
 *  js.h
 *  versatubes
 *
 *  Created by 2Roqs on 04/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxJavascript.h"

bool setupJS();

ofxJSDeclareFunctionCpp(ofNoise);

/*
ofxJSDeclareFunctionCpp(loadShader);
ofxJSDeclareFunctionCpp(beginShader);
ofxJSDeclareFunctionCpp(endShader);
ofxJSDeclareFunctionCpp(setUniform1fShader);
ofxJSDeclareFunctionCpp(setUniform2fShader);
*/

ofxJSDeclareFunctionCpp(playSound);
ofxJSDeclareFunctionCpp(getSoundsForAnimation);
ofxJSDeclareFunctionCpp(playSoundForThisAnimation);
ofxJSDeclareFunctionCpp(setVolumeSound);
ofxJSDeclareFunctionCpp(setVolumeSoundAll);
ofxJSDeclareFunctionCpp(setVolumeSoundMainNormalized); // pass value between 0 and 1

ofxJSDeclareFunctionCpp(setDeviceColorHueSaturation);
ofxJSDeclareFunctionCpp(setDeviceColor);

ofxJSDeclareFunctionCpp(setAnimation);
ofxJSDeclareFunctionCpp(setAnimationDirect);



static char s_jsStringBuffer[2048];
static string s_jsTemp;





