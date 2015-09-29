//
//  blur.h
//  murmur
//
//  Created by Julien on 29/09/2015.
//
//

#pragma once
#include "ofMain.h"

class blur
{
	public:
			void			loadShaders		(string dir);
	
			void			allocate		(int w, int h);
			void			apply			(ofFbo&, float blurAmount, int nbPasses=1);
 
			ofFbo			m_fboX, m_fboY;
			ofShader		m_shaderX,m_shaderY;
 
			ofFbo&			get				();
};