/*
 *  js.cpp
 *  versatubes
 *
 *  Created by 2Roqs on 04/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#include "js.h"
#include "globals.h"
#include "soundManager.h"

//--------------------------------------------------------------
bool setupJS()
{
	// Some functions
	ofxJSDefineFunctionGlobal("ofNoise",						&ofNoise,						3);
	
    ofxJSDefineFunctionGlobal("loadShader",						&loadShader,					1);
	ofxJSDefineFunctionGlobal("beginShader",					&beginShader,					0);
	ofxJSDefineFunctionGlobal("endShader",						&endShader,						0);
	ofxJSDefineFunctionGlobal("setUniform1fShader",				&setUniform1fShader,			2);
	ofxJSDefineFunctionGlobal("setUniform2fShader",				&setUniform2fShader,			3);

	ofxJSDefineFunctionGlobal("playSound",                      &playSound,                     3);
    ofxJSDefineFunctionGlobal("setVolumeSound",                 &setVolumeSound,                2);
    ofxJSDefineFunctionGlobal("setVolumeSoundAll",              &setVolumeSoundAll,             1);
    ofxJSDefineFunctionGlobal("setVolumeSoundMainNormalized",   &setVolumeSoundMainNormalized,  1);
    
    
	// Load globals
	ofxJSScript* pScript = ofxJSLoadFromData("murmur.js", "murmur"); // "exampleId" is used for error reporting
	if (pScript){
		bool evalOk = ofxJSEval(pScript);
		if (evalOk){
		}
	}
	
	return true; // TODO err checking
}

//--------------------------------------------------------------
ofxJSDefineFunctionCpp(ofNoise){
	float val = 0.0f;

	if (argc == 1)
		val = ofNoise( ofxJSValue_TO_float(argv[0]) );
	else
	if (argc == 2)
		val = ofNoise( ofxJSValue_TO_float(argv[0]), ofxJSValue_TO_float(argv[1]) );
	else
	if (argc == 3)
		val = ofNoise( ofxJSValue_TO_float(argv[0]), ofxJSValue_TO_float(argv[1]), ofxJSValue_TO_float(argv[2]) );

	*retVal = float_TO_ofxJSValue(val);
	
	return JS_TRUE;
}

//--------------------------------------------------------------
ofxJSDefineFunctionCpp(loadShader)
{
	Animation* pAnimation = Globals::instance()->mp_animationManager->mp_animationCurrent;
	if (argc==1 && pAnimation)
	{
		pAnimation->M_loadShader(ofxJSValue_TO_string(argv[0]));
		return JS_TRUE;
	}

	return JS_FALSE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(beginShader)
{
	Animation* pAnimation = Globals::instance()->mp_animationManager->mp_animationCurrent;
	if (pAnimation)
		pAnimation->M_beginShader();
	return JS_TRUE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(endShader)
{
	Animation* pAnimation = Globals::instance()->mp_animationManager->mp_animationCurrent;
	if (pAnimation)
		pAnimation->M_endShader();

	return JS_TRUE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(setUniform1fShader)
{
	Animation* pAnimation = Globals::instance()->mp_animationManager->mp_animationCurrent;
	if (argc==2 && pAnimation)
	{
		pAnimation->m_shader.setUniform1f(ofxJSValue_TO_string(argv[0]).c_str(), ofxJSValue_TO_float(argv[1]));
		return JS_TRUE;
	}
	return JS_FALSE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(setUniform2fShader)
{
	Animation* pAnimation = Globals::instance()->mp_animationManager->mp_animationCurrent;
	if (argc==3 && pAnimation)
	{
		pAnimation->m_shader.setUniform2f(ofxJSValue_TO_string(argv[0]).c_str(), ofxJSValue_TO_float(argv[1]), ofxJSValue_TO_float(argv[2]));
		return JS_TRUE;
	}
	return JS_FALSE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(playSound)
{
	if (argc==1)
    {
        SoundManager::instance()->playSound( ofxJSValue_TO_string(argv[0]) );
		return JS_TRUE;
    }
	else if (argc==2)
    {
        SoundManager::instance()->playSound( ofxJSValue_TO_string(argv[0]),  ofxJSValue_TO_int(argv[1]) == 0 ? false : true );
		return JS_TRUE;
    }
	else if (argc==3)
    {
        SoundManager::instance()->playSound( ofxJSValue_TO_string(argv[0]),  ofxJSValue_TO_int(argv[1]) == 0 ? false : true, ofxJSValue_TO_float(argv[2]) );
		return JS_TRUE;
    }

	return JS_FALSE;
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(setVolumeSound)
{
	if (argc==2)
    {
        SoundManager::instance()->setVolume( ofxJSValue_TO_string(argv[0]),  ofxJSValue_TO_float(argv[1]));
    }
}


//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(setVolumeSoundAll)
{
	if (argc==1)
    {
        SoundManager::instance()->setVolumeAll( ofxJSValue_TO_float(argv[0]) );
    }
}

//--------------------------------------------------------------
ofxJSDeclareFunctionCpp(setVolumeSoundMainNormalized)
{
	if (argc==1)
    {
        SoundManager::instance()->setVolumeSoundMainNormalized( ofxJSValue_TO_float(argv[0]) );
    }
}
