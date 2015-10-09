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
	
	m_particleSpeedMax = 5;
	m_particlesSize = 4;
	m_kParticles = 20;
	
	m_properties.add( new classProperty_float("vector field speed", 0.001, 0.01, &m_vectorFieldAnim) );
	m_properties.add( new classProperty_float("blending factor", 0, 1, &m_blending) );
	//m_properties.add( new classProperty_int("particles number (k)", 5, 20, &m_kParticles) );
	m_properties.add( new classProperty_float("particles speed factor", 1, 20, &m_particlesSpeedFactor) );
	m_properties.add( new classProperty_float("particles speed max", 0.5, 10, &m_particleSpeedMax) );
	m_properties.add( new classProperty_float("particles size", 1, 8, &m_particlesSize) );
	

	m_vectorFieldImage.loadImage("Images/tmpVectorFields.png");
	


	m_resetModes.push_back("Center");
	m_resetModes.push_back("Cross");
	m_resetModes.push_back("Line width");
	m_resetModes.push_back("Line height");
	m_resetModes.push_back("Grid");
	m_resetModes.push_back("Rect");
	m_resetModes.push_back("Screen");

	m_resetMode = m_resetModes[0];
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
		
		//addUISlider( m_properties.getInt("particles number (k)") );
		addUISlider( m_properties.getFloat("particles speed factor") );
		addUISlider( m_properties.getFloat("particles speed max") );
		addUISlider( m_properties.getFloat("particles size") );
		addUISlider( m_properties.getFloat("blending factor") );

		mp_UIcanvas->addWidgetDown(new ofxUILabel("Reset mode", OFX_UI_FONT_MEDIUM));
		mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));

		mp_btnReset = new ofxUILabelButton("Reset", false, 100,16,0,0,OFX_UI_FONT_SMALL);
		mp_UIcanvas->addWidgetDown( mp_btnReset );


		ofxUIRadio* pRadioResetMode = new ofxUIRadio("radioResetModes", m_resetModes, OFX_UI_ORIENTATION_VERTICAL, 16,16,0,0);
		mp_UIcanvas->addWidgetDown( pRadioResetMode );
		
		pRadioResetMode->activateToggle("Center");
	}
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_enter()
{
	setDrawBackground();
	//createParticles();
	//resetParticles(m_resetMode);
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_exit()
{
}

//--------------------------------------------------------------
void AnimationAgentsFF::VM_update(float dt)
{
    m_particleSystem.setTimeStep(dt);
//	m_vectorFieldAnim2 += (m_vectorFieldAnim-m_vectorFieldAnim2)*dt*0.8; // not used for the moment
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
	
    if (!m_bParticlesInit || !m_vectorField.isAllocated()) return;
	
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
		//p.updatePosition(1);

		float timeStep = 1.0f;
		
		p.xv += p.xf;
		p.yv += p.yf;
		
		p.xv = ofClamp(p.xv,-m_particleSpeedMax,m_particleSpeedMax);
		p.yv = ofClamp(p.yv,-m_particleSpeedMax,m_particleSpeedMax);
//		x += xv * timeStep;
//		y += yv * timeStep;
		p.mp_pos->x += p.xv * timeStep;
		p.mp_pos->y += p.yv * timeStep;


		
//		p.addForce
		(*pos) += ofRandom(1,5)*m_particlesSpeedFactor*m_vectorField.getVectorInterpolated(pos->x, pos->y,m_w,m_h);
		
	}


    m_particleSystem.update();
	ofSetColor(255, 255, 255, 255);
	glPointSize(m_particlesSize);
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
void AnimationAgentsFF::resetParticles(string mode)
{
        for(int i = 0; i < m_kParticles * 1024; i++)
		{
//            float x = ofRandom(padding, m_w - padding);
  //          float y = ofRandom(padding, m_h - padding);

			float maxVelocity = m_particleSpeedMax;
			
			if (mode == "Center")
			{
		        float padding = 200;

	            float x = m_w/2;
    	        float y = m_h/2;
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);
	

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			}
			else if (mode == "Line width")
			{
	            float x = ofRandom(0.0f,m_w);
    	        float y = m_h/2;
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}
			else if (mode == "Line height")
			{
	            float x = m_w/2;
    	        float y = ofRandom(0.0f,m_h);
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}
			else if (mode == "Cross")
			{
				float rnd = ofRandomf();
			
	            float x = rnd < 0.5f ? m_w/2 : ofRandom(m_w);
    	        float y = rnd < 0.5f ? ofRandom(m_h) : m_h/2;
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}
			else if (mode == "Grid")
			{
				int nbRows = 10;
				int nbColumns = 20;
				
				int r = (int)ofRandom(float(nbRows));
				int c = (int)ofRandom(float(nbColumns));

			
	            float x = m_w / float(nbColumns) * float(c);
	            float y = m_h / float(nbRows) * float(r);
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}
			else if (mode == "Rect")
			{

			
	            float x = ofRandom(m_w/2-0.25*m_w, m_w/2+0.25*m_w);
	            float y = ofRandom(m_h/2-0.25*m_h, m_h/2+0.25*m_h);
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}
			else if (mode == "Screen")
			{

			
	            float x = ofRandom(m_w);
	            float y = ofRandom(m_h);
        	    float xv = ofRandom(-maxVelocity, maxVelocity);
            	float yv = ofRandom(-maxVelocity, maxVelocity);

				m_particlesPos[i].set(x,y);

	            Particle particle(m_particlesPos+i, xv, yv);
				particle.mass = ofRandom(1.0f,1.5f);
        	    //m_particleSystem.add(particle);
			
			}

			
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

			m_vectorField.deallocate();
			m_vectorField.setup((int)m_w,(int)m_h,10);
			m_vectorField.setFromImage(m_vectorFieldImage);
		}
	}


    if (m_w>0.0 && m_h>0.0 && !m_bParticlesInit && m_kParticles>0)
    {
        m_particleSystem.setup((int)m_w, (int)m_h, 3);

		if (m_particlesPos)
		{
			delete[] m_particlesPos;
			m_particlesPos = 0;
		}

		m_particlesPos = new ofVec3f[m_kParticles * 1024];
        for(int i = 0; i < m_kParticles * 1024; i++)
		{
			m_particlesPos[i].set(0.0f,0.0f);
			Particle particle(m_particlesPos+i, 0.0f, 0.0f);
			m_particleSystem.add(particle);
		}

		m_particlesVbo.setVertexData(m_particlesPos, m_kParticles * 1024, GL_DYNAMIC_DRAW);
		resetParticles(m_resetMode);
	 
        m_bParticlesInit = true;
    }
    

}

//--------------------------------------------------------------
void AnimationAgentsFF::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	if (e.getKind() == OFX_UI_WIDGET_TOGGLE)
	{
		if (e.getToggle()->getValue()>0)
		{
			if (isToggleResetMode(name))
			{
				m_resetMode = name;
			}
		}
	}
	
	if (name == "Reset")
	{
		resetParticles(m_resetMode);
	}
	else if (name == "particles number (k)")
	{
		m_kParticles = (int)e.getSlider()->getScaledValue();
		m_bParticlesInit = false;
	}

}


//--------------------------------------------------------------
bool AnimationAgentsFF::isToggleResetMode(string name)
{
	for (int i=0;i<m_resetModes.size();i++)
		if (m_resetModes[i] == name)
			return true;
	return false;
}


