//
//  animationWaves.cpp
//  murmur
//
//  Created by Julien on 14/07/2015.
//
//

#include "animationWaves.h"
#include "device.h"
#include "globals.h"

//--------------------------------------------------------------
Wave::Wave(WaveManager* pParent, float x, float y)
{
	mp_parent = pParent;
	m_loc.set(x,y);
	m_radius = 0.0f;
	m_radiusSpeed = 50.0f;
	m_phase = 0.0f;
	m_nbPoints = 0;
	m_age = 0;
	setNbPoints(10);
	m_color.set(255,255);
}

//--------------------------------------------------------------
void Wave::setNbPoints(int nb)
{
	if (nb != m_nbPoints)
	{
		m_nbPoints = nb;

		m_points.clear();
		for (int i=0;i<m_nbPoints;i++)
		{
			m_points.push_back( ofVec3f() );
		}
	}
	updatePoints();
}

//--------------------------------------------------------------
void Wave::updatePoints()
{
	m_loc.x += m_dir.x;
	m_loc.y += m_dir.y;

	float angle = 0.0f;
	float angleStep = TWO_PI / float(m_nbPoints);
	for (int i=0; i<m_nbPoints; i++)
	{
		m_points[i].set
		(
			m_loc.x + m_radius*cos( m_phase+angle ),
			m_loc.y + m_radius*sin( m_phase+angle )
		);
		
		angle += angleStep;
	
	}
}

//--------------------------------------------------------------
void Wave::update(float dt)
{
	m_age += dt;
	if (m_age>m_ageMax) m_age = m_ageMax;
	m_color.a = 255*(1.0-m_age/m_ageMax);
	m_radius += m_radiusSpeed*dt;
	updatePoints();
}

//--------------------------------------------------------------
WaveManager::WaveManager(AnimationWaves* pParent)
{
   mp_parent = pParent;
   m_volumeAccum.setTriggerInCb(sM_volumeTriggerIn, (void*) this);
}

//--------------------------------------------------------------
WaveManager::~WaveManager()
{
	for (int i=0;i<m_waves.size();i++)
		delete m_waves[i];
	m_waves.clear();
}


//--------------------------------------------------------------
void WaveManager::update(float dt)
{
	if (mp_parent==0) return;

	m_volumeAccum.m_valueTriggerIn 	= mp_parent->m_valueTriggerIn;
	m_volumeAccum.m_valueTriggerOut = mp_parent->m_valueTriggerOut;


	int nbWaves = m_waves.size();
	for (int i=0; i<nbWaves;i++)
	{
		m_waves[i]->setAgeMax(mp_parent->m_ageMax);
		m_waves[i]->setSpeed(mp_parent->m_speed);
		
		m_waves[i]->update(dt);
	}

	// Remove old ones
	vector<Wave*>::iterator it = m_waves.begin();
	while( it != m_waves.end())
	{
		if ( (*it)->isOld() )
		{
			delete *it;
			it = m_waves.erase(it);
		}
		else{
			++it;
		}
	}

	computeMesh();
}

//--------------------------------------------------------------
void WaveManager::draw()
{
	int nbWaves = m_waves.size();
	if (nbWaves>0)
	{
		m_mesh.draw();
	}
}


//--------------------------------------------------------------
void WaveManager::onNewPacket(DevicePacket* pDevicePacket, ofColor deviceColor, float x, float y)
{
	m_lastPacketPos.set(x,y);
	m_lastPacketColor = deviceColor;
	m_volumeAccum.add( pDevicePacket->m_volume );
}

//--------------------------------------------------------------
void WaveManager::sM_volumeTriggerIn(void* pData, VolumeAccum*)
{
	WaveManager* pThis = (WaveManager*) pData;
	
    // New wave
	Wave* pNewWave = new Wave(pThis,pThis->m_lastPacketPos.x,pThis->m_lastPacketPos.y);
	pNewWave->setDir
	(
		ofRandom(-0.5f, 0.5f)*pThis->mp_parent->m_waveDirAmp,
		ofRandom(-0.5f, 0.5f)*pThis->mp_parent->m_waveDirAmp
	);
	
	pNewWave->setColor( pThis->mp_parent->m_isColorFromDevice ?  ofColor(pThis->m_lastPacketColor,255) : ofColor(255,255));
	pNewWave->setAgeMax(pThis->mp_parent->m_ageMax);
	pNewWave->setNbPoints( pThis->mp_parent->m_nbWavePoints );
	
	// Save this wave
	pThis->m_waves.push_back(pNewWave);

	// Compute Mesh
	pThis->computeMesh();
}

//--------------------------------------------------------------
void WaveManager::computeMesh()
{
	// Clear vertices
	m_mesh.clear();
	m_mesh.enableColors();
	m_mesh.enableIndices();
	
	// Add vertices + color
	int nbWaves = m_waves.size(); // ofLog() << nbWaves;
	for (int i=0; i<nbWaves;i++)
	{
		for (int k=0; k<m_waves[i]->m_nbPoints; k++)
		{
			m_mesh.addColor(m_waves[i]->m_color);
			m_mesh.addVertex( m_waves[i]->m_points[k] );
		}
	}
	
	// create primitives
	if (nbWaves == 1)
	{
		m_mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		for (int k=0; k<m_waves[0]->m_nbPoints; k++)
		{
			m_mesh.addIndex( k );
		}
	}
	else
	{
		m_mesh.setMode(OF_PRIMITIVE_LINES);
		for (int i=0; i<nbWaves-1;i++)
		{
			int nbPoints = m_waves[i]->m_nbPoints; // same for all waves
			int offset = i*nbPoints;
			for (int k=0; k<m_waves[i]->m_nbPoints; k++)
			{
				m_mesh.addIndex( offset+k );
				m_mesh.addIndex( offset+(k+1)%nbPoints );

				m_mesh.addIndex( offset+(k+1)%nbPoints );
				m_mesh.addIndex( offset+(k+1)%nbPoints + nbPoints );

				m_mesh.addIndex( offset+(k+1)%nbPoints + nbPoints );
				m_mesh.addIndex( offset+k + nbPoints );

				m_mesh.addIndex( offset+k + nbPoints );
				m_mesh.addIndex( offset+k );

			}
		}
	}
	
}

//--------------------------------------------------------------
void WaveManager::setNbPoints(int nb)
{
	int nbWaves = m_waves.size(); // ofLog() << nbWaves;
	for (int i=0; i<nbWaves;i++)
	{
		m_waves[i]->setNbPoints(nb);
	}
	computeMesh();
}

//--------------------------------------------------------------
AnimationWaves::AnimationWaves(string name) : Animation(name)
{
	m_waveDirAmp = 0.5f;
	m_valueTriggerIn = 0.3f;
	m_valueTriggerOut = 0.1f;
	m_nbWavePoints = 10;
}

//--------------------------------------------------------------
AnimationWaves::~AnimationWaves()
{
	m_wavesDeviceIt = m_wavesDevice.begin();
	for ( ; m_wavesDeviceIt != m_wavesDevice.end(); ++m_wavesDeviceIt)
	{
		delete m_wavesDeviceIt->second;
	}
	m_wavesDevice.clear();
}

//--------------------------------------------------------------
void AnimationWaves::createUICustom()
{
	if (mp_UIcanvas)
	{
		mp_UIcanvas->addToggle("colorFromDevice", 	&m_isColorFromDevice);
	}

	m_properties.add( new classProperty_float("nbWavePoints", 			3.0f, 20.0f, 	&m_nbWavePoints) );
	m_properties.add( new classProperty_float("speed", 				20.0f, 100.0f, 	&m_speed) );
	m_properties.add( new classProperty_float("ageMax", 			1.0f, 20.0f, 	&m_ageMax) );
	m_properties.add( new classProperty_float("waveDirAmp", 		0.0f, 3.0f, 	&m_waveDirAmp) );
	m_properties.add( new classProperty_float("valueTriggerIn", 	0.0f, 1.0f, 	&m_valueTriggerIn) );
	m_properties.add( new classProperty_float("valueTriggerOut", 	0.0f, 1.0f, 	&m_valueTriggerOut) );
	m_properties.add( new classProperty_float("lineWidthWave", 		0.0f, 20.0f, 	&m_lineWidthWave) );
	
	
	addUISlider( m_properties.getFloat("nbWavePoints") );
	addUISlider( m_properties.getFloat("speed") );
	addUISlider( m_properties.getFloat("ageMax") );
	addUISlider( m_properties.getFloat("waveDirAmp") );
	addUISlider( m_properties.getFloat("valueTriggerIn") );
	addUISlider( m_properties.getFloat("valueTriggerOut") );
//	addUISlider( m_properties.getFloat("lineWidthWave") );
}

//--------------------------------------------------------------
void AnimationWaves::VM_update(float dt)
{
	m_wavesDeviceIt = m_wavesDevice.begin();
	for ( ; m_wavesDeviceIt != m_wavesDevice.end(); ++m_wavesDeviceIt)
	{
		WaveManager* pWaveManager = m_wavesDeviceIt->second;
		pWaveManager->update(dt);
	}
}

//--------------------------------------------------------------
void AnimationWaves::VM_draw(float w, float h)
{
	drawBackground(0);
	ofEnableAlphaBlending();
//	glLineWidth(m_lineWidthWave);
	m_wavesDeviceIt = m_wavesDevice.begin();
	for ( ; m_wavesDeviceIt != m_wavesDevice.end(); ++m_wavesDeviceIt)
	{
		m_wavesDeviceIt->second->draw();
	}
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void AnimationWaves::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	if (name == "nbWavePoints")
	{
		m_wavesDeviceIt = m_wavesDevice.begin();
		for ( ; m_wavesDeviceIt != m_wavesDevice.end(); ++m_wavesDeviceIt)
			m_wavesDeviceIt->second->setNbPoints( (int) e.getSlider()->getScaledValue() );
	}
}

//--------------------------------------------------------------
void AnimationWaves::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	if (m_wavesDevice.find(deviceId) == m_wavesDevice.end())
	{
		m_wavesDevice[deviceId] = new WaveManager(this);
	}

	// Get device to have full color
	if (GLOBALS->mp_deviceManager)
	{
		Device* pDevice = GLOBALS->mp_deviceManager->getDeviceById( deviceId );
		m_wavesDevice[deviceId]->onNewPacket(pDevicePacket, pDevice->m_color, x, y);
	}
}
