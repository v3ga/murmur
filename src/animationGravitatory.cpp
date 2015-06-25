//
//  animationGravitatory.cpp
//  murmur
//
//  Created by Julien on 23/06/2015.
//
//

#include "animationGravitatory.h"

//--------------------------------------------------------------
ParticleGravitatory::ParticleGravitatory(AnimationGravitatory* pParent, string deviceId, float x, float y, ofColor color)
{
	mp_parent = pParent;
	m_deviceId = deviceId;
	m_pos.set(x,y);
	m_angle = ofRandom(TWO_PI);
	m_angleSpeed = PI/2;
	m_size = ofRandom(5,30);
	m_ageMax = ofRandom(15,30);
	m_color = color;


}

//--------------------------------------------------------------
void ParticleGravitatory::update(float dt)
{
	//ofLog() << ofToString( cos( m_angle )*300*dt );

	m_pos.x += cos( m_angle )*200*dt;
	m_pos.y += sin( m_angle )*200*dt;

	m_angle += m_angleSpeed*dt*mp_parent->m_angleSpeedFactor;
	m_age += dt;
}

//--------------------------------------------------------------
void ParticleGravitatory::draw()
{
	float alpha = (1.0f-m_age/m_ageMax) * 255.0f;
	ofSetColor(m_color,alpha*mp_parent->m_alphaFactor);
	ofEllipse(m_pos.x,m_pos.y,m_size*mp_parent->m_sizeFactor,m_size*mp_parent->m_sizeFactor);
}

//--------------------------------------------------------------
AnimationGravitatory::AnimationGravitatory(string name) : Animation(name)
{
	m_sizeFactor		= 1.0f;
	m_alphaFactor		= 1.0f;
	m_angleSpeedFactor	= 1.0f;
}

//--------------------------------------------------------------
AnimationGravitatory::~AnimationGravitatory()
{
	map< string, vector<ParticleGravitatory*> >::iterator it = m_particlesDevice.begin();

	for ( ; it != m_particlesDevice.end(); ++it )
	{
		 vector<ParticleGravitatory*>& particles = it->second;
		for (int i=0;i<particles.size();i++)
			delete particles[i];
		particles.clear();
	}
}


//--------------------------------------------------------------
void AnimationGravitatory::createUICustom()
{
	if (mp_UIcanvas)
	{
		mp_UIcanvas->addToggle("colorFromDevice", 	&m_isColorFromDevice);
	}

		m_properties.add( new classProperty_float("sizeFactor",			0.0f, 1.0f, 	&m_sizeFactor) );
		m_properties.add( new classProperty_float("alphaFactor", 		0.0f, 1.0f, 	&m_alphaFactor) );
		m_properties.add( new classProperty_float("angleSpeedFactor", 	0.0f, 1.0f, 	&m_angleSpeedFactor) );

		addUISlider( m_properties.getFloat("sizeFactor") );
		addUISlider( m_properties.getFloat("alphaFactor") );
		addUISlider( m_properties.getFloat("angleSpeedFactor") );


/*		m_properties.add( new classProperty_float("particles size", 1.0f, 5.0f, &m_particlesSize) );
	

		addUISlider( m_properties.getFloat("particles size") );

*/

}

//--------------------------------------------------------------
void AnimationGravitatory::VM_update(float dt)
{
	map< string, vector<ParticleGravitatory*> >::iterator it = m_particlesDevice.begin();

	for ( ; it != m_particlesDevice.end(); ++it )
	{
		vector<ParticleGravitatory*>& particles = it->second;
		for (int i=0;i<particles.size();i++)
			particles[i]->update(dt);


		vector<ParticleGravitatory*>::iterator it2 = particles.begin();
		while( it2 != particles.end())
		{
			if ( (*it2)->m_age >= (*it2)->m_ageMax){
				delete *it2;
				it2 = particles.erase(it2);
			}
			else{
				++it2;
			}
		}
	}
}

//--------------------------------------------------------------
void AnimationGravitatory::VM_draw(float w, float h)
{
	drawBackground(0);
	ofEnableAlphaBlending();

	map< string, vector<ParticleGravitatory*> >::iterator it = m_particlesDevice.begin();
	for ( ; it != m_particlesDevice.end(); ++it )
	{
		 vector<ParticleGravitatory*>& particles = it->second;

		for (int i=0;i<particles.size();i++)
			particles[i]->draw();
	}

	ofDisableAlphaBlending();
}


//--------------------------------------------------------------
void AnimationGravitatory::guiEvent(ofxUIEventArgs &e)
{
	
}

//--------------------------------------------------------------
void AnimationGravitatory::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	if ( m_particlesDevice.find(deviceId) == m_particlesDevice.end())
		m_particlesDevice[deviceId] = vector<ParticleGravitatory*>();

	vector<ParticleGravitatory*>& particles = m_particlesDevice[deviceId];
	
	if (pDevicePacket->m_volume>0.2f)
	{
		particles.push_back( new ParticleGravitatory(this, deviceId,x,y,  m_isColorFromDevice ?  pDevicePacket->m_color : ofColor(255) ) );
		//ofLog() << deviceId << " : " << ofToString( particles.size() ) << " particles";
	}
}


