//
//  animationParticlesMega2.cpp
//  murmur
//
//  Created by Julien on 17/05/13.
//
//

#include "animationParticlesMega2.h"
#include "soundManager.h"
#include "globals.h"


//--------------------------------------------------------------
AnimationParticlesMega2::AnimationParticlesMega2(string name) : Animation(name)
{
    m_isParticlesInit = false;
    m_volumeTarget = m_volume = 0.0f;

    m_repulsionRadius = 100.0;
    m_ampRepulsion = 10.0;
    m_ampAttraction = 10.0;
	
	m_particlesSize = 1.0f;
	m_particlesPos = 0;
	m_particlesColor = 0;

	m_colorRadiusFactor = 1.0f;
	
	mp_volumeAccum = 0;

	m_properties.add( new classProperty_float("amp. attraction",1.0f, 100.0f, &m_ampAttraction) );
	m_properties.add( new classProperty_float("amp. repulsion", 1.0f, 100.0f, &m_ampRepulsion) );
	m_properties.add( new classProperty_float("radius repulsion", 40.0f, 300.0f, &m_repulsionRadius) );
	m_properties.add( new classProperty_float("particles size", 2.0f, 10.0f, &m_particlesSize) );
	m_properties.add( new classProperty_float("radius color factor", 1.0f, 10.0f, &m_colorRadiusFactor) );

	mp_sliderAmpAttraction 		= 0;
	mp_sliderAmpRepulsion 		= 0;
	mp_sliderRadiusRepulsion 	= 0;
	mp_sliderParticleSize 		= 0;

	m_bHandlePitch = true;
}


//--------------------------------------------------------------
AnimationParticlesMega2::~AnimationParticlesMega2()
{
	map<string,ParticleForce*>::iterator it = m_mapParticleForce.begin();
	for ( ; it != m_mapParticleForce.end() ; ++it)
		delete it->second;

	delete[] m_particlesPos;
	m_particlesPos = 0;

	delete[] m_particlesColor;
	m_particlesColor = 0;
}

//--------------------------------------------------------------
void AnimationParticlesMega2::createParticles()
{
    if (m_resolution.x>0.0 && m_resolution.y>0.0 && !m_isParticlesInit)
    {
        int binPower = 3;
        
        particleSystem.setup(m_resolution.x, m_resolution.y, binPower);
        
        kParticles = 16;
        float padding = 0;
        float maxVelocity = .5;

		if (m_particlesPos)
		{
			delete[] m_particlesPos;
			m_particlesPos = 0;
		}
		if (m_particlesColor)
		{
			delete[] m_particlesColor;
			m_particlesColor = 0;
		}


		m_particlesPos = new ofVec3f[kParticles * 1024];
		m_particlesColor = new ofFloatColor[kParticles * 1024];

        for(int i = 0; i < kParticles * 1024; i++) {
            float x = ofRandom(padding, ofGetWidth() - padding);
            float y = ofRandom(padding, ofGetHeight() - padding);
            float xv = ofRandom(-maxVelocity, maxVelocity);
            float yv = ofRandom(-maxVelocity, maxVelocity);

			m_particlesPos[i].set(x,y);

			m_particlesColor[i].set( ofRandom(0.0f,1.0f) );

            Particle particle(m_particlesPos+i, m_particlesColor+i, xv, yv);
            particleSystem.add(particle);


        }

		m_particlesVbo.setVertexData(m_particlesPos, kParticles * 1024, GL_DYNAMIC_DRAW);
		m_particlesVbo.setColorData(m_particlesColor, kParticles * 1024, GL_DYNAMIC_DRAW);

        timeStep = 1;
        lineOpacity = 128;
        pointOpacity = 255;
        isMousePressed = false;
        slowMotion = false;
        particleNeighborhood = 4;
        particleRepulsion = 1;
        centerAttraction = .01;
        
        
        m_isParticlesInit = true;
    }
    
}


//--------------------------------------------------------------
void AnimationParticlesMega2::VM_enter()
{
	setDrawBackground();
}

//--------------------------------------------------------------
void AnimationParticlesMega2::createUICustom()
{
    if (mp_UIcanvas)
    {
		mp_UIcanvas->addToggle("colorFromDevice", 	&m_isColorFromDevice);


		mp_sliderAmpAttraction 		= addUISlider( m_properties.getFloat("amp. attraction") );
		mp_sliderAmpRepulsion 		= addUISlider( m_properties.getFloat("amp. repulsion") );
		mp_sliderRadiusRepulsion 	= addUISlider( m_properties.getFloat("radius repulsion") );
		mp_sliderParticleSize 		= addUISlider( m_properties.getFloat("particles size") );
		addUISlider( m_properties.getFloat("radius color factor") );
    }
}

//--------------------------------------------------------------
void AnimationParticlesMega2::updateUI()
{
	if (hasPitch())
	{
//		if (mp_sliderParticleSize) 		mp_sliderParticleSize->setValue(m_particlesSize);
		if (mp_sliderAmpAttraction) 	mp_sliderAmpAttraction->setValue(m_ampAttraction);
		if (mp_sliderAmpRepulsion) 		mp_sliderAmpRepulsion->setValue(m_ampRepulsion);
		if (mp_sliderRadiusRepulsion) 	mp_sliderRadiusRepulsion->setValue(m_repulsionRadius);
	}
}


//--------------------------------------------------------------
void AnimationParticlesMega2::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);
    string name = e.widget->getName();
    if (name == "particles size")
    {
		m_particlesSize = ((ofxUISlider*) e.widget)->getScaledValue();
    particleSystem.setParticleSize(m_particlesSize);

	}
}

//--------------------------------------------------------------
void AnimationParticlesMega2::VM_update(float dt)
{
	if (hasPitch())
	{
//		if (mp_volumeAccum)
//			m_particlesSize = ofMap(mp_volumeAccum->m_valueMean,0.0f,1.0f,m_properties.getFloat("particles size")->m_min, m_properties.getFloat("particles size")->m_max);
		m_ampAttraction = ofMap(m_pitchLast, 0.0f,1.0f, 5, 50);
		m_ampRepulsion = ofMap(m_pitchLast, 0.0f,1.0f, 40, 100);
		m_repulsionRadius = ofMap(m_pitchLast, 0.0f,1.0f, 140, 300);
	}

	updateUIVolume();
	updateUI();


	map<string,ParticleForce*>::iterator it = m_mapParticleForce.begin();
	ParticleForce* pParticleForce=0;
	for ( ; it != m_mapParticleForce.end() ; ++it)
	{
		pParticleForce = it->second;
		pParticleForce->update(dt);
	}
	
    //SoundManager::instance()->setVolumeSoundMainNormalized(m_volume);
}

//--------------------------------------------------------------
void AnimationParticlesMega2::VM_draw(float w, float h)
{
	drawBackground(0);

    m_resolution.set(w,h);
    createParticles();

    if (!m_isParticlesInit) return;
    
    particleSystem.setTimeStep(timeStep);
 
	m_particlesVbo.updateVertexData(m_particlesPos, kParticles*1024);
	m_particlesVbo.updateColorData(m_particlesColor, kParticles*1024);
    
	ofEnableAlphaBlending();
//	ofSetColor(255, 255, 255, lineOpacity);
	particleSystem.setupForces();
    
	// apply per-particle forces
//	glBegin(GL_LINES);
	for(int i = 0; i < particleSystem.size(); i++)
    {
		Particle& cur = particleSystem[i];

		// global force on other particles
		//particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion/100);
		
        // forces on this particle
		cur.bounceOffWalls(0, 0, w, h);
		cur.addDampingForce();
	}
//	glEnd();
    
	// single global forces
	map<string,ParticleForce*>::iterator it = m_mapParticleForce.begin();
	ParticleForce* pParticleForce=0;
	for ( ; it != m_mapParticleForce.end() ; ++it)
	{
		pParticleForce = it->second;
	
		particleSystem.m_color = m_isColorFromDevice ? pParticleForce->m_color : ofColor(255);
		particleSystem.addAttractionForce(pParticleForce->m_anchor.x, pParticleForce->m_anchor.y, w, centerAttraction*m_ampAttraction);
    	particleSystem.addRepulsionForce(pParticleForce->m_anchor.x, pParticleForce->m_anchor.y, pParticleForce->m_volume*m_repulsionRadius, pParticleForce->m_volume*m_ampRepulsion, false);
    	particleSystem.addRepulsionForce(pParticleForce->m_anchor.x, pParticleForce->m_anchor.y, pParticleForce->m_volume*m_repulsionRadius*m_colorRadiusFactor, 0, true);
	}

        
    particleSystem.update();
	ofSetColor(255, 255, 255, pointOpacity);
	glPointSize(particleSystem.getParticleSize());
	m_particlesVbo.draw(GL_POINTS,0,kParticles*1024);
//	ofDisableAlphaBlending();
 }

//--------------------------------------------------------------
void AnimationParticlesMega2::VM_exit()
{
    
}

//--------------------------------------------------------------
void AnimationParticlesMega2::onVolumAccumEvent(string deviceId)
{
	Animation::onVolumAccumEvent(deviceId);
}

//--------------------------------------------------------------
void AnimationParticlesMega2::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	if (pDevicePacket)
		accumulateVolumeAndPitch(pDevicePacket->m_volume, pDevicePacket->m_pitch, deviceId);

	// TEMP
	mp_volumeAccum = getVolumAccumForDevice(deviceId);

	// Create force
	 map<string,ParticleForce*>::iterator it = m_mapParticleForce.find(deviceId);
	 ParticleForce* pParticleForce=0;

	 if ( it == m_mapParticleForce.end())
	 {
		 pParticleForce = new ParticleForce(x,y);
		 m_mapParticleForce[deviceId] = pParticleForce;
	 }
	 else
	 {
		 pParticleForce = it->second;
	 }



	 if (pParticleForce && pDevicePacket)
	 {
		 pParticleForce->m_anchor.set(x,y);
		 pParticleForce->m_volumeTarget = pDevicePacket->m_volume;
		 
		 Device* pDevice = GLOBALS->mp_deviceManager->getDeviceById( deviceId );

//		 pParticleForce->m_color = pDevice->m_color;
		 pParticleForce->m_color = pDevicePacket->m_color;
		// printf("pDevicePacket->m_volume = %.5f\n", pDevicePacket->m_volume);

		if (hasPitch())
		{
			m_pitchLast = pDevicePacket->m_pitch;
		}
	 }
}




