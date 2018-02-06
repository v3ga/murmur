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
   m_pitchLast = 0.0f;
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
	
	m_volumeAccum.update(dt);
	if (
			mp_parent->m_enableTimeResetAccum &&
			( m_volumeAccum.m_state == VolumeAccum::STATE_WAVE_INSIDE &&  m_volumeAccum.m_stateTime >= mp_parent->m_timeResetAccum )
		)
	{
		m_volumeAccum.reset();
	}


	if (mp_parent->hasPitch())
	{
//		updateNbPoints();
	}


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
	m_volumeAccum.add( pDevicePacket->m_volume, pDevicePacket->m_pitch );
}

//--------------------------------------------------------------
void WaveManager::sM_volumeTriggerIn(void* pData, VolumeAccum* pVolumAccum)
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

	if (pThis->mp_parent->hasPitch())
	{
		pThis->m_pitchLast = pThis->m_volumeAccum.m_valuePitchMean;
		pThis->updateNbPoints();
	}
	
	pNewWave->setNbPoints( pThis->m_nbPoints );
	
	// ofLog() << pThis->m_nbPoints;
	
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
void WaveManager::updateNbPoints()
{
	setNbPoints( ofMap(m_pitchLast,0.0f,1.0f, mp_parent->m_properties.getFloat("nbWavePoints")->m_max, mp_parent->m_properties.getFloat("nbWavePoints")->m_min) );
}

//--------------------------------------------------------------
void WaveManager::setNbPoints(int nb)
{
	if (m_nbPoints != nb)
	{
		m_nbPoints = nb;

		int nbWaves = m_waves.size();
		for (int i=0; i<nbWaves;i++)
			m_waves[i]->setNbPoints(nb);
		computeMesh();
	}
}

//--------------------------------------------------------------
AnimationWaves::AnimationWaves(string name) : Animation(name)
{
	m_waveDirAmp 			= 0.5f;
	m_valueTriggerIn 		= 0.3f;
	m_valueTriggerOut 		= 0.1f;
	m_nbWavePoints 			= 10;
	m_enableTimeResetAccum 	= false;
	m_timeResetAccum 		= 0.5f;
	mp_sliderNbWavePoints 	= 0;

	loadPropertiesDefinition();


	m_properties.add( new classProperty_float("nbWavePoints", 		getPropDef("nbwavepoints:min", 3.0f), 		getPropDef("nbwavepoints:max", 20.0f), 		&m_nbWavePoints) );
	m_properties.add( new classProperty_float("speed", 				getPropDef("speed:min", 20.0f), 			getPropDef("speed:max", 100.0f), 			&m_speed) );
	m_properties.add( new classProperty_float("ageMax", 			getPropDef("agemax:min", 1.0f), 			getPropDef("agemax:max", 20.0f), 			&m_ageMax) );
	m_properties.add( new classProperty_float("waveDirAmp", 		getPropDef("wavediramp:min", 0.0f), 		getPropDef("wavediramp:max", 3.0f), 		&m_waveDirAmp) );
	m_properties.add( new classProperty_float("valueTriggerIn", 	0.0f, 1.0f, 	&m_valueTriggerIn) );
	m_properties.add( new classProperty_float("valueTriggerOut", 	0.0f, 1.0f, 	&m_valueTriggerOut) );
	m_properties.add( new classProperty_float("lineWidthWave", 		getPropDef("linewidthwave:min", 0.0f), 		getPropDef("linewidthwave:max", 20.0f), 	&m_lineWidthWave) );
	m_properties.add( new classProperty_bool("enableResetAccum", 					&m_enableTimeResetAccum) );
	m_properties.add( new classProperty_float("timeResetAccum", 	0.1f, 1.0f,		&m_timeResetAccum) );

	m_bHandlePitch = true;

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


	mp_sliderNbWavePoints = addUISlider( m_properties.getFloat("nbWavePoints") );
	addUISlider( m_properties.getFloat("speed") );
	addUISlider( m_properties.getFloat("ageMax") );
	addUISlider( m_properties.getFloat("waveDirAmp") );
	addUISlider( m_properties.getFloat("valueTriggerIn") );
	addUISlider( m_properties.getFloat("valueTriggerOut") );
	addUItoggle( m_properties.getBool("enableResetAccum"));
	addUISlider( m_properties.getFloat("timeResetAccum") );
//	addUISlider( m_properties.getFloat("lineWidthWave") );
}

//--------------------------------------------------------------
void AnimationWaves::VM_update(float dt)
{
	updateUIVolume();
	if (hasPitch())
	{
//		m_nbWavePoints = (int)ofMap(m_pitchLast,0.0f,1.0f,15,3);
//		updateNbPointsForWaves();

		if (mp_sliderNbWavePoints) mp_sliderNbWavePoints->setValue( m_nbWavePoints );
	}

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
//	ofDisableAlphaBlending();
}


//--------------------------------------------------------------
void AnimationWaves::updateNbPointsForWaves()
{
	m_wavesDeviceIt = m_wavesDevice.begin();
	for ( ; m_wavesDeviceIt != m_wavesDevice.end(); ++m_wavesDeviceIt)
		m_wavesDeviceIt->second->setNbPoints( m_nbWavePoints );
}

//--------------------------------------------------------------
void AnimationWaves::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);

	string name = e.getName();
	if (name == "nbWavePoints")
	{
		m_nbWavePoints = (int) e.getSlider()->getScaledValue();
		updateNbPointsForWaves();
	}
}

//--------------------------------------------------------------
void AnimationWaves::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	accumulateVolumeAndPitch(pDevicePacket->m_volume, pDevicePacket->m_pitch, deviceId);
	m_pitchLast = getVolumAccumForDevice(deviceId)->m_valuePitchMean;

	if (m_wavesDevice.find(deviceId) == m_wavesDevice.end())
	{
		m_wavesDevice[deviceId] = new WaveManager(this);
	}

	// Get device to have full color
	if (GLOBALS->mp_deviceManager)
	{
		Device* pDevice = GLOBALS->mp_deviceManager->getDeviceById( deviceId );
		m_wavesDevice[deviceId]->onNewPacket(pDevicePacket, /*pDevice->m_color*/ pDevicePacket->m_color, x, y);
	}
}
