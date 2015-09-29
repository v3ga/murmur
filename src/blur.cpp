//
//  blur.cpp
//  murmur
//
//  Created by Julien on 29/09/2015.
//
//

#include "blur.h"


void blur::loadShaders(string dir)
{
	m_shaderX.load(dir+"/blur.vert", dir+"/blurX.frag");
	m_shaderY.load(dir+"/blur.vert", dir+"/blurY.frag");
}

void blur::allocate(int w, int h)
{
	m_fboX.allocate(w,h);
	m_fboY.allocate(w,h);
}

void blur::apply(ofFbo& fbo, float blurAmount, int nbPasses)
{
	int w = m_fboX.getWidth();
	int h = m_fboX.getHeight();
	ofFbo* input = &fbo;


	for (int i=0;i<nbPasses;i++)
	{
		m_fboX.begin();
		m_shaderX.begin();
		m_shaderX.setUniform1f("blurAmnt", blurAmount);
		input->draw(0,0,w,h);
		m_shaderX.end();
		m_fboX.end();

		m_fboY.begin();
		m_shaderY.begin();
		m_shaderY.setUniform1f("blurAmnt", blurAmount);
		m_fboX.draw(0,0,w,h);
		m_shaderY.end();
		m_fboY.end();
		
		input = &m_fboY;
	}
}

ofFbo& blur::get()
{
	return m_fboY;
}

