//
//  animationAgentsFF.cpp
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#include "animationAgentsFF.h"

//--------------------------------------------------------------
AnimationAgentsFF::AnimationAgentsFF(string name) : Animation(name)
{
	m_w = 0;
	m_h = 0;
	
	m_particlesPos		= 0;
	m_bParticlesInit 	= false;
	
	m_vectorFieldAnim = 0.001f;
	m_blending = 1.0f;
	
	m_particlesSpeedFactor = 15.0f;
	m_vectorFieldAnim2 = m_vectorFieldAnim;
	
	m_properties.add( new classProperty_float("vector field speed", 0.001, 0.01, &m_vectorFieldAnim) );
	m_properties.add( new classProperty_float("blending factor", 0, 1, &m_blending) );
	m_properties.add( new classProperty_float("particles speed factor", 1, 20, &m_particlesSpeedFactor) );
	
	m_vectorFieldImage.loadImage("Images/tmpVectorFields.png");
}

//--------------------------------------------------------------
AnimationAgentsFF::~AnimationAgentsFF()
{
	delete[] m_particlesPos;
}

//--------------------------------------------------------------
void AnimationAgentsFF::createUICustom()
{
	if (mp_UIcanvas)
	{
		mp_UIcanvas->addWidgetDown(new ofxUILabel("Vector field", OFX_UI_FONT_MEDIUM));
		mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));
		
		mp_UIcanvas->addToggle("draw", &m_bDrawVectorField);
		addUISlider( m_properties.getFloat("vector field speed") );

		mp_UIcanvas->addWidgetDown(new ofxUILabel("Particles", OFX_UI_FONT_MEDIUM));
		mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));

		addUISlider( m_properties.getFloat("particles speed factor") );
		addUISlider( m_properties.getFloat("blending factor") );
	}
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_enter()
{
	setDrawBackground();
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_exit()
{
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_update(float dt)
{
    m_particleSystem.setTimeStep(dt);
	m_vectorFieldAnim2 += (m_vectorFieldAnim-m_vectorFieldAnim2)*dt*0.8; // not used for the moment
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_drawBefore(float w, float h)
{
	m_w = w;
	m_h = h;
	
	createParticles();
	m_particlesVbo.updateVertexData(m_particlesPos, m_kParticles*1024);

	m_fboParticles.begin();
	ofSetColor(0,m_blending*255);
	ofRect(0,0,w,h);
	
    if (!m_bParticlesInit) return;
	
	m_vectorField.animate(m_vectorFieldAnim);
	
	for(int i = 0; i < m_particleSystem.size(); i++)
    {
		Particle& p = m_particleSystem[i];
		ofVec3f* pos = p.mp_pos;
		ofVec2f v;

		if (pos->x>m_w) pos->x = 0;
		if (pos->x<0) pos->x = m_w;
		if (pos->y>m_h) pos->y = 0;
		if (pos->y<0) pos->y = m_h;
		
		// v.set(p.xv, p.yv);
		//v = m_vectorField.getVectorInterpolated(pos->x, pos->y,m_w,m_h);
		//p.xf = 5*v.x;
		//p.yf =5*v.y;
		
//		p.addDampingForce();
		p.updatePosition(1);
		
//		p.addForce
		(*pos) += ofRandom(1,2)*m_particlesSpeedFactor*m_vectorField.getVectorInterpolated(pos->x, pos->y,m_w,m_h);
		
	}


    m_particleSystem.update();
	ofSetColor(255, 255, 255, 255);
	glPointSize(m_particleSystem.getParticleSize());
	m_particlesVbo.draw(GL_POINTS,0,m_kParticles*1024);

   m_fboParticles.end();

}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_draw(float w, float h)
{
	drawBackground(0.0f);
	m_fboParticles.draw(0,0,w,h);
	if (m_bDrawVectorField)
	{
		ofPushStyle();
		ofSetColor(255,0,0);
		m_vectorField.draw();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void AnimationAgentsFF::createParticles()
{
	if (m_w>0.0 && m_h>0.0)
	{
		if (!m_fboParticles.isAllocated() || (m_w != m_fboParticles.getWidth() || m_h != m_fboParticles.getHeight()))
		{
			m_fboParticles.allocate(m_w,m_h,GL_RGBA32F_ARB);
			m_fboParticles.begin();
			ofBackground(0,0);
			m_fboParticles.end();
			
		}
	}


    if (m_w>0.0 && m_h>0.0 && !m_bParticlesInit)
    {

		m_vectorField.setup((int)m_w,(int)m_h,10);
		m_vectorField.setFromImage(m_vectorFieldImage);
	
        int binPower = 3;
        
        m_particleSystem.setup((int)m_w, (int)m_h, binPower);
        
        m_kParticles = 20;
        float padding = 200;
        float maxVelocity = 1.5;

		if (m_particlesPos)
		{
			delete[] m_particlesPos;
			m_particlesPos = 0;
		}
		

		m_particlesPos = new ofVec3f[m_kParticles * 1024];
		//m_particlesColor = new ofFloatColor[kParticles * 1024];

        for(int i = 0; i < m_kParticles * 1024; i++)
		{
//            float x = ofRandom(padding, m_w - padding);
  //          float y = ofRandom(padding, m_h - padding);
            float x = m_w/2;
            float y = m_h/2;
            float xv = ofRandom(-maxVelocity, maxVelocity);
            float yv = ofRandom(-maxVelocity, maxVelocity);

			m_particlesPos[i].set(x,y);


            Particle particle(m_particlesPos+i, xv, yv);
			particle.mass = ofRandom(1.0f,1.5f);
            m_particleSystem.add(particle);

        }

		m_particlesVbo.setVertexData(m_particlesPos, m_kParticles * 1024, GL_DYNAMIC_DRAW);
	 
        m_bParticlesInit = true;
    }
    

}

