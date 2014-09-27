//
//  animationOrbit.cpp
//  murmur
//
//  Created by Julien on 25/09/2014.
//
//

#include "animationOrbit.h"
#include "globals.h"


//--------------------------------------------------------------
ParticlePath::ParticlePath()
{
	mp_orbitA = 0;
	mp_orbitB = 0;
	
	m_speed = 50;
}


//--------------------------------------------------------------
void ParticlePath::setSegment(int indexA, ParticleOrbit* pA, int indexB, ParticleOrbit* pB)
{
	m_indexA = indexA;
	mp_orbitA = pA;
	m_A = mp_orbitA->getPoints()[indexA];
	
	m_indexB = indexB;
	mp_orbitB = pB;
	m_B = mp_orbitB->getPoints()[indexB];

	m_u = (m_B-m_A).normalized();
	m_isReachB = false;

	m_pos = m_A;
}

//--------------------------------------------------------------
void ParticlePath::update(float dt)
{
	if (mp_orbitA == 0) return;

	m_pos += m_u*m_speed*dt;

	if ( m_pos.distance(m_A) >=  m_A.distance(m_B))
	{
		if (mp_orbitB == mp_orbitA)
		{
			int indexA = m_indexB;
			int indexB = (m_indexA+1)%mp_orbitA->getPoints().size();
			setSegment(indexA,mp_orbitA,indexB,mp_orbitB);
		}

	}
}


//--------------------------------------------------------------
ParticleOrbitEllipse::ParticleOrbitEllipse()
{
	setRadius( ofVec2f(100.0,50.0) );
	setOffset( ofVec3f(0.0,0.0,0.0) );
	setRotation(20.0f);
}

//--------------------------------------------------------------
void ParticleOrbitEllipse::computePoints()
{
	m_points.clear();

	ofVec2f pointEllipse;
	ofVec3f point;
	float aR = 0.0f;
	for (float angle = 0.0f; angle < 360.0f; angle+=30.0f)
	{
		aR = ofDegToRad(angle);
		pointEllipse.set(
							m_radius.x * cos(aR),
							m_radius.y * sin(aR)
						);

		float rotR = ofDegToRad(m_rot);
		point.set(
					m_center.x+m_offset.x + pointEllipse.x*cos(rotR) - pointEllipse.y*sin(rotR),
					m_center.y+m_offset.y + pointEllipse.x*sin(rotR) + pointEllipse.y*cos(rotR)
		);
		
		m_points.push_back( point );
	}

}

//--------------------------------------------------------------
void ParticleOrbitEllipse::setOffset(ofVec3f offset)
{
	m_offset = offset;
	computePoints();
}

//--------------------------------------------------------------
void ParticleOrbitEllipse::setRadius(ofVec2f radius)
{
	m_radius = radius;
	computePoints();
}

//--------------------------------------------------------------
void ParticleOrbitEllipse::setRotation(float rot)
{
	m_rot = rot;
	computePoints();
}


//--------------------------------------------------------------
AnimationOrbit::AnimationOrbit(string name) : Animation(name)
{
	mp_testParticle = new ParticlePath();
}

//--------------------------------------------------------------
AnimationOrbit::~AnimationOrbit()
{
	for (m_orbitsIt = m_orbits.begin(); m_orbitsIt != m_orbits.end(); ++m_orbitsIt)
	{
		delete m_orbitsIt->second;
	}

	delete mp_testParticle;
}
 
//--------------------------------------------------------------
void AnimationOrbit::VM_enter()
{
}

//--------------------------------------------------------------
void AnimationOrbit::VM_update(float dt)
{
	if (mp_testParticle)
		mp_testParticle->update(dt);
}

//--------------------------------------------------------------
void AnimationOrbit::VM_draw(float w, float h)
{
	ofBackground(0);
	ofPushStyle();
	for (m_orbitsIt = m_orbits.begin(); m_orbitsIt != m_orbits.end(); ++m_orbitsIt)
	{
		ParticleOrbit* pOrbit = m_orbitsIt->second;
		Device* pDevice = getDevice( m_orbitsIt->first );
		
		ofFill();
		ofSetColor(255,0,0);
		ofPushMatrix();
//			ofTranslate( pOrbit->getCenter().x, pOrbit->getCenter().y );
//			ofEllipse( 0, 0, 10,10 );
			drawOrbit(pOrbit);
		ofPopMatrix();
	}

	if (mp_testParticle){
		ofSetColor(0,0,255);
		ofEllipse(mp_testParticle->m_pos, 5,5);
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void AnimationOrbit::drawOrbit(ParticleOrbit* pOrbit)
{
	if (pOrbit==0) return;

	ofPushStyle();
	ofFill();
	ofSetColor(255,0,0);
	vector<ofVec3f>& points = pOrbit->getPoints();
	int nbPoints = points.size();
	for (int i=0;i<nbPoints;i++)
	{
		ofLine( points[i].x, points[i].y, points[(i+1)%nbPoints].x, points[(i+1)%nbPoints].y );
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void AnimationOrbit::VM_exit()
{
}

//--------------------------------------------------------------
void AnimationOrbit::onNewPacket(DevicePacket*, string deviceId, float x, float y)
{
	// Create Orbit for device
	ParticleOrbit* pOrbit = getOrbitForDevice(deviceId);
	if (pOrbit == 0)
	{
		ParticleOrbitEllipse* pOrbitEllipse =  new ParticleOrbitEllipse();
		if (deviceId == "deviceEchoSimulator01" || deviceId == "chambreEcho_001"){
			pOrbitEllipse->setRotation( -20 );


		}
		if (deviceId == "deviceEchoSimulator02" || deviceId == "chambreEcho_002"){
			pOrbitEllipse->setRotation( 20 );
		}
		pOrbitEllipse->setCenter( ofVec3f(x,y,0.0f) );
		m_orbits[deviceId] = pOrbitEllipse;

		pOrbit = pOrbitEllipse;
	}
	if (pOrbit && updateDevicePosition(deviceId,x,y))
	{
		pOrbit->setCenter( ofVec3f(x,y) );
// TEMP
mp_testParticle->setSegment(0, pOrbit, 1, pOrbit);
	}
	
	
}

//--------------------------------------------------------------
void AnimationOrbit::createUICustom()
{

}

//--------------------------------------------------------------
ParticleOrbit* AnimationOrbit::getOrbitForDevice(Device* pDevice)
{
	return 0;
}

//--------------------------------------------------------------
ParticleOrbit* AnimationOrbit::getOrbitForDevice(string deviceId)
{
	if (m_orbits.find(deviceId) != m_orbits.end())
		return m_orbits[deviceId];
	return 0;
}

