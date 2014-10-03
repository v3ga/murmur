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
BoidOrbit::BoidOrbit(AnimationOrbit* pAnimation, float x, float y) : Boid(x,y)
{
	mp_animation = pAnimation;
	setSpeedMinMax(1.0,2.0);
	setForceMax(0.1);
	m_locationsNbMax = 350;
	mp_particlePath = 0;
	for (int i=0;i<m_locationsNbMax;i++)
	{
		m_polyline.addVertex( ofVec2f(0.0f,0.0f) );
	}
}

//--------------------------------------------------------------
void BoidOrbit::setSpeedMinMax(float speedMin, float speedMax)
{
	maxspeed = ofRandom(speedMin, speedMax);
}

//--------------------------------------------------------------
void BoidOrbit::setForceMax(float forceMax)
{
	maxforce = forceMax;
}

//--------------------------------------------------------------
void BoidOrbit::setLocationsNbMax(int n)
{
	m_locationsNbMax = n;
	m_locations.clear();
	m_polyline.clear();
	for (int i=0;i<m_locationsNbMax;i++)
	{
		m_polyline.addVertex( location );
	}

}

//--------------------------------------------------------------
void BoidOrbit::flock(vector<Boid*>& boids)
{

	if (mp_animation->m_boidsSeparation>0.0f)
	{
		ofVec2f sep = separate(boids);   // Separation
		sep *= mp_animation->m_boidsSeparation;
		applyForce(sep);
	}
	
	if (mp_animation->m_boidsAlignement>0.0f)
	{
		ofVec2f ali = align(boids);      // Alignment
		ali *= mp_animation->m_boidsAlignement;
		applyForce(ali);
	}

	if (mp_animation->m_boidsCohesion>0.0f)
	{
		ofVec2f coh = cohesion(boids);   // Cohesion
		coh *= mp_animation->m_boidsCohesion;
		applyForce(coh);
	}
}

//--------------------------------------------------------------
void BoidOrbit::follow(ofVec2f& target, vector<Boid*>& boids)
{
	applyForce( seek(target) );
	flock(boids);
	update(0.0f);

	m_locations.push_back( location );
	if (m_locations.size()>m_locationsNbMax)
	{
		m_locations.erase(m_locations.begin());
	}
}

//--------------------------------------------------------------
void BoidOrbit::follow(vector<Boid*>& boids)
{
	if (mp_particlePath)
		follow(mp_particlePath->m_pos, boids);
}


//--------------------------------------------------------------
void BoidOrbit::update(float dt)
{
	if (mp_animation)
	{
		 // Update velocity
 		velocity+=acceleration;
	 	// Limit speed
 		velocity.limit(maxspeed /* * mp_animation->m_boidsSpeedFactor*/);
 		location+=velocity;
 		// Reset accelertion to 0 each cycle
 		acceleration.set(0.0f,0.0f);
	}
}

//--------------------------------------------------------------
void BoidOrbit::draw()
{
	int nbPoints = m_locations.size();
	for (int i=0;i<nbPoints;i++)
	{
		m_polyline[i].set(m_locations[i]);
	}

	m_polyline.draw();
}


//--------------------------------------------------------------
ParticlePath::ParticlePath()
{
	mp_orbitA = 0;
	mp_orbitB = 0;
	
	m_speed 	= 50;
	m_nbTurns	= 0;
}


//--------------------------------------------------------------
void ParticlePath::setSegment(ParticleOrbit* pA, int indexA, ParticleOrbit* pB, int indexB)
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
void ParticlePath::setSegment(ParticleOrbit* pA, int indexA)
{
	if (pA)
	{
		if (indexA==0) m_nbTurns++;
		setSegment(pA, indexA, pA, (indexA+1)%pA->getPoints().size());
	}
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
			setSegment(mp_orbitA,indexA);
		}
	}
}


//--------------------------------------------------------------
void ParticleOrbit::setReverse(bool is)
{
	m_isReverse = is;
	std::reverse(m_points.begin(), m_points.end());
}

//--------------------------------------------------------------
ParticleOrbitEllipse::ParticleOrbitEllipse()
{
	setRadius( ofVec2f(100.0,50.0) );
	setOffset( ofVec3f(0.0,0.0,0.0) );
	setRotation(20.0f);
//	setReverse(true);
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

	//std::reverse(m_points.begin(), m_points.end());
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
void ParticleOrbitEllipse::load(ofxXmlSettings& settings)
{
	m_rot = settings.getValue("rotation", 0.0f);
	m_radius.set( settings.getValue("width", 200.0f), settings.getValue("height", 100.0f) );
	m_offset.set( settings.getValue("offset_x", 0.0f), settings.getValue("offset_y", 0.0f) );
	setReverse( settings.getValue("reverse", false) );

	ofLog() << "    - m_reverse=" << ofToString(m_isReverse);
	ofLog() << "    - m_rot=" << m_rot;
	ofLog() << "    - m_radius=" << m_radius;
	ofLog() << "    - m_offset=" << m_offset;
}

//--------------------------------------------------------------
void ParticleOrbitEllipse::save(ofxXmlSettings& settings)
{
	settings.addValue("rotation", m_rot);
	settings.addValue("width", m_radius.x);
	settings.addValue("height", m_radius.y);
	settings.addValue("offset_x", m_offset.x);
	settings.addValue("offset_y", m_offset.y);
	settings.addValue("reverse", m_isReverse);
}


//--------------------------------------------------------------
AnimationOrbit::AnimationOrbit(string name) : Animation(name)
{

	for (int i=0;i<200;i++)
		m_boids.push_back( new BoidOrbit(this,0,0) );


	m_isDrawDebug 		= false;
	m_isFrameBlending	= false;
	m_frameBlending		= 0.25f;

	m_boidsSeparation 		= 1.5f;
	m_boidsCohesion			= 1.0f;
	m_boidsAlignement		= 1.0f;
	m_boidsMaxSpeedMin		= 1.0f;
	m_boidsMaxSpeedMax		= 2.0f;
	m_boidsDrawAlpha		= 1.0f;
	m_boidsSpeedFactor		= 1.0f;
	m_boidsNbParticlesPath	= 4;
	m_boidsSpeedMin			= 2.0f;
	m_boidsSpeedMax			= 5.0f;

	m_reverseForms		= false;
	m_rotationForms		= 20.0f;
	m_widthForms		= 200.0f;
	m_heightForms		= 100.0f;
	
	mp_labelDeviceId	= 0;
	mp_toggleFormReverse= 0;
	mp_sliderFormRot	= 0;
	mp_sliderFormWidth	= 0;
	mp_sliderFormHeight	= 0;
	mp_deviceCurrent	= 0;
	
	mp_mgEnergy			= 0;

	m_boidState			= boidState_rest;
				float			m_tBoidExcitation;

}

//--------------------------------------------------------------
AnimationOrbit::~AnimationOrbit()
{
	for (m_orbitsIt = m_orbits.begin(); m_orbitsIt != m_orbits.end(); ++m_orbitsIt)
	{
		delete m_orbitsIt->second;
	}

	vector<Boid*>::iterator it = m_boids.begin();
	for (; it != m_boids.end(); ++it)
	{
		delete *it;
	}
	
	deleteParticlePaths();

}

//--------------------------------------------------------------
void AnimationOrbit::deleteParticlePaths()
{
	vector<ParticlePath*>::iterator it = m_particlePaths.begin();
	for ( ; it!=m_particlePaths.end(); ++it)
	{
		delete *it;
	}

	m_particlePaths.clear();
}


//--------------------------------------------------------------
void AnimationOrbit::VM_enter()
{
}

//--------------------------------------------------------------
void AnimationOrbit::VM_update(float dt)
{
	// Energies for devices
	map<string,float>::iterator itEnergies = m_energies.begin();
	string deviceId;
	for ( ; itEnergies != m_energies.end(); ++itEnergies)
	{
	   deviceId = itEnergies->first;
	   m_energies[deviceId] -= 0.5;
	   if (m_energies[deviceId] < 0.0f) m_energies[deviceId] = 0.0f;
	}

	// Update UI
	Device* pDeviceCurrent = this->getDeviceCurrent();
	if (pDeviceCurrent && mp_mgEnergy){
		mp_mgEnergy->addPoint( m_energies[deviceId] );
	}


	// Particles on path
	vector<ParticlePath*>::iterator it = m_particlePaths.begin();
	for ( ;it != m_particlePaths.end(); ++it)
	{
		(*it)->setSpeed( 50.0f*m_boidsSpeedFactor );
		(*it)->update(dt);
	}

	// Boids
	vector<Boid*>::iterator itBoids = m_boids.begin();
	BoidOrbit* pBoid=0;
	for ( ; itBoids != m_boids.end(); ++itBoids)
	{
		pBoid = (BoidOrbit*) *itBoids;
		pBoid->follow(m_boids);
	}

	// Look for change on Device
	if (mp_deviceCurrent != getDeviceCurrent())
	{
		mp_deviceCurrent = getDeviceCurrent();
		updateUI();
	}

}

//--------------------------------------------------------------
void AnimationOrbit::VM_draw(float w, float h)
{
	ofEnableAlphaBlending();

	if (m_isFrameBlending)
	{
		ofPushStyle();
		ofFill();
		ofSetColor(0,m_frameBlending*255.0f);
		ofRect(0,0,w,h);
		ofPopStyle();
	}
	else{
		ofBackground(0);
	}


	if (m_isDrawDebug)
	{
		ofPushStyle();
		for (m_orbitsIt = m_orbits.begin(); m_orbitsIt != m_orbits.end(); ++m_orbitsIt)
		{
			ParticleOrbit* pOrbit = m_orbitsIt->second;
			Device* pDevice = getDevice( m_orbitsIt->first );
		
			ofFill();
			ofSetColor(255,0,0);
			ofPushMatrix();
				drawOrbit(pOrbit);
			ofPopMatrix();
		}
	}
	
		ofSetColor(255,255,255, m_boidsDrawAlpha*255.0f);
		vector<Boid*>::iterator it = m_boids.begin();
		for ( ; it != m_boids.end(); ++it)
		{
			(*it)->draw();
			if (m_isDrawDebug)
				ofEllipse((*it)->location, 5,5);
		}

	if (m_isDrawDebug)
	{
		ofSetColor(0,0,255);
		vector<ParticlePath*>::iterator it = m_particlePaths.begin();
		for ( ;it != m_particlePaths.end(); ++it)
		{
			ofEllipse((*it)->m_pos, 5,5);
			ofDrawBitmapString(ofToString( (*it)->getNbTurns() ), (*it)->m_pos.x+10, (*it)->m_pos.y);
		}
	}

	ofPopStyle();

	ofDisableAlphaBlending();
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
		ofDrawBitmapString(ofToString(i), points[i].x+10, points[i].y);
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void AnimationOrbit::VM_exit()
{
}

//--------------------------------------------------------------
void AnimationOrbit::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	// Nothing ? go out
    if (pDevicePacket==0) return;

	// For sound playing
	accumulateVolume(pDevicePacket->m_volume, deviceId);


	m_energies[deviceId] += pDevicePacket->m_volume;
	// ofLog() << m_energies[deviceId];

	// Create Orbit for device
	ParticleOrbit* pOrbit = getOrbitForDevice(deviceId);
	if (pOrbit == 0)
	{
		ParticleOrbitEllipse* pOrbitEllipse =  new ParticleOrbitEllipse();

		// Orbit properties for device 01
		if (deviceId == "deviceEchoSimulator01" || deviceId == "chambreEcho_001")
		{
			pOrbitEllipse->setRotation( -m_rotationForms );


		}
		// Orbit properties  for device 02
		else if (deviceId == "deviceEchoSimulator02" || deviceId == "chambreEcho_002"){
			pOrbitEllipse->setRotation( m_rotationForms );
		}
		
		// Set center
		pOrbitEllipse->setCenter( ofVec3f(x,y,0.0f) );

		// Save in map
		m_orbits[deviceId] = pOrbitEllipse;
		pOrbit = pOrbitEllipse;

		// Assign boids to paths
		createParticlePathsForOrbit(pOrbit);

		// Assign boids to paths
		assignBoidsToPaths();

	}

	// Position of deviceId changed ?
	if (pOrbit && updateDevicePosition(deviceId,x,y))
	{
		pOrbit->setCenter( ofVec3f(x,y) );
	}
	
	
}

//--------------------------------------------------------------
void AnimationOrbit::createUICustom()
{
    if (mp_UIcanvas)
    {
		int widthDefault = 320;

	    mp_UIcanvas->addWidgetDown	( new ofxUILabel("Drawing", OFX_UI_FONT_SMALL) );
    	mp_UIcanvas->addWidgetDown	( new ofxUISpacer(widthDefault, 1));

        mp_UIcanvas->addToggle("draw_debug",	&m_isDrawDebug);
        mp_UIcanvas->addToggle("trace",			&m_isFrameBlending);
        mp_UIcanvas->addSlider("trace_level", 	0.0f, 1.0f, &m_frameBlending);
        mp_UIcanvas->addSlider("alpha", 		0.0f, 1.0f, &m_boidsDrawAlpha);


	    mp_UIcanvas->addWidgetDown	( new ofxUILabel("Flocking", OFX_UI_FONT_SMALL) );
    	mp_UIcanvas->addWidgetDown	( new ofxUISpacer(widthDefault, 1));

		mp_UIcanvas->addIntSlider("nbParticlePaths", 		2, 10, &m_boidsNbParticlesPath);
        mp_UIcanvas->addIntSlider("trail_size", 50, 350, 200);

        mp_UIcanvas->addSlider("separation", 	0.0f, 1.5f, &m_boidsSeparation);
        mp_UIcanvas->addSlider("cohesion", 		0.0f, 1.5f, &m_boidsCohesion);
        mp_UIcanvas->addSlider("alignement", 	0.0f, 1.5f, &m_boidsAlignement);
		mp_UIcanvas->addRangeSlider("speed", 	1.0f, 10.0f, &m_boidsSpeedMin, &m_boidsSpeedMax);
        mp_UIcanvas->addSlider("force_max", 	0.0f, 2.0f, &m_boidsForceMax);
        mp_UIcanvas->addSlider("speed_factor", 	0.0f, 4.0f, &m_boidsSpeedFactor);
		
		mp_UIcanvas->addWidgetDown( new ofxUILabelButton("calm", true, 60,16) );
		mp_UIcanvas->addWidgetRight( new ofxUILabelButton("excited", true, 60,16) );
	
	
		vector<float> mgValues(widthDefault);
		for (int i=0;i<widthDefault;i++) mgValues[i]=0.0f;
		mp_mgEnergy = new ofxUIMovingGraph(0,0,widthDefault,70, mgValues, widthDefault, 0.0f,1.0f, "energy");
		mp_UIcanvas->addWidgetDown( mp_mgEnergy );

	    mp_labelDeviceId = new ofxUILabel("Forms", OFX_UI_FONT_SMALL);
		mp_UIcanvas->addWidgetDown	( mp_labelDeviceId );
    	mp_UIcanvas->addWidgetDown	( new ofxUISpacer(widthDefault, 1));
		mp_toggleFormReverse = mp_UIcanvas->addToggle("reverse_forms", 	&m_reverseForms);
		mp_sliderFormRot = mp_UIcanvas->addSlider("rotation_forms", 	-90.0f, 90.0f, &m_rotationForms);
		mp_sliderFormWidth = mp_UIcanvas->addSlider("width_forms", 		10.0f, 500.0f, &m_widthForms);
		mp_sliderFormHeight = mp_UIcanvas->addSlider("height_forms", 	10.0f, 500.0f, &m_heightForms);
    }
}

//--------------------------------------------------------------
ParticleOrbit* AnimationOrbit::getOrbitForDevice(Device* pDevice)
{
	if (pDevice)
		return getOrbitForDevice( pDevice->m_id );
	return 0;
}

//--------------------------------------------------------------
ParticleOrbit* AnimationOrbit::getOrbitForDevice(string deviceId)
{
	if (m_orbits.find(deviceId) != m_orbits.end())
		return m_orbits[deviceId];
	return 0;
}

//--------------------------------------------------------------
void AnimationOrbit::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);
    string name = e.widget->getName();

    if (name == "trail_size")
	{
		ofxUIIntSlider* pSliderTrail = (ofxUIIntSlider*) e.widget;

		vector<Boid*>::iterator it = m_boids.begin();
		BoidOrbit* pBoid=0;
		for ( ; it != m_boids.end(); ++it)
		{
			pBoid = (BoidOrbit*) *it;
			pBoid->setLocationsNbMax( pSliderTrail->getScaledValue() );
//			pBoid->setSpeedMinMax(pSliderSpeed->getScaledValueLow(), pSliderSpeed->getScaledValueHigh());
		}
	}
    else if (name == "speed")
	{
		ofxUIRangeSlider* pSliderSpeed = (ofxUIRangeSlider*) e.widget;

		vector<Boid*>::iterator it = m_boids.begin();
		BoidOrbit* pBoid=0;
		for ( ; it != m_boids.end(); ++it)
		{
			pBoid = (BoidOrbit*) *it;
			pBoid->setSpeedMinMax(pSliderSpeed->getScaledValueLow(), pSliderSpeed->getScaledValueHigh());
		}
	}
    else if (name == "force_max")
	{
		ofxUISlider* pSliderForceMax = (ofxUISlider*) e.widget;

		vector<Boid*>::iterator it = m_boids.begin();
		BoidOrbit* pBoid=0;
		for ( ; it != m_boids.end(); ++it)
		{
			pBoid = (BoidOrbit*) *it;
			pBoid->setForceMax(pSliderForceMax->getScaledValue());
		}
	}
    else if (name == "reverse_forms")
	{
		ofxUIToggle* pToggle = (ofxUIToggle*) e.widget;
	
		ParticleOrbitEllipse* pOrbitEllipse = (ParticleOrbitEllipse*) getOrbitForDevice(getDeviceCurrent());
		if(pOrbitEllipse) pOrbitEllipse->setReverse( pToggle->getValue() );
		
	}
    else if (name == "rotation_forms")
	{
		ofxUISlider* pSliderFormRot = (ofxUISlider*) e.widget;
	
		ParticleOrbitEllipse* pOrbitEllipse = (ParticleOrbitEllipse*) getOrbitForDevice(getDeviceCurrent());
		if(pOrbitEllipse) pOrbitEllipse->setRotation( pSliderFormRot->getScaledValue() );
		
	}
    else if (name == "width_forms")
	{
		ParticleOrbitEllipse* pOrbitEllipse = (ParticleOrbitEllipse*) getOrbitForDevice(getDeviceCurrent());
		if(pOrbitEllipse) pOrbitEllipse->setWidth( ((ofxUISlider*) e.widget)->getScaledValue() );
	}
    else if (name == "height_forms")
	{
		ParticleOrbitEllipse* pOrbitEllipse = (ParticleOrbitEllipse*) getOrbitForDevice(getDeviceCurrent());
		if(pOrbitEllipse) pOrbitEllipse->setHeight( ((ofxUISlider*) e.widget)->getScaledValue() );
	}
	else if (name == "nbParticlePaths")
	{
		int nb = ((ofxUISlider*)e.widget)->getScaledValue();
		if (nb != m_boidsNbParticlesPath)
		{
			deleteParticlePaths();
		
		
			map<string,ParticleOrbit*>::iterator it = m_orbits.begin();
			for ( ; it!=m_orbits.end(); ++it){
				createParticlePathsForOrbit( it->second );
			}
			assignBoidsToPaths();
		}
	}
	else if (name == "calm")
	{
		ofxUILabelButton* pBtn = (ofxUILabelButton*) e.widget;
		if (pBtn->getValue())
		{
			m_boidsSpeedFactor = 1.0f;
			m_boidsForceMax = 0.2f;
			m_boidsSpeedMin = 2.0f;
			m_boidsSpeedMax = 3.0f;


			vector<Boid*>::iterator it = m_boids.begin();
			BoidOrbit* pBoid=0;
			for ( ; it != m_boids.end(); ++it)
			{
				pBoid = (BoidOrbit*) *it;
				pBoid->setForceMax(m_boidsForceMax);
				pBoid->setSpeedMinMax(m_boidsSpeedMin, m_boidsSpeedMax);
			}

		}
	}
	else if (name == "excited")
	{
		ofxUILabelButton* pBtn = (ofxUILabelButton*) e.widget;
		if (pBtn->getValue())
		{
			m_boidsSpeedFactor = 9.0f;
			m_boidsForceMax = 1.0f;
			m_boidsSpeedMin = 9.0f;
			m_boidsSpeedMax = 10.0f;


			vector<Boid*>::iterator it = m_boids.begin();
			BoidOrbit* pBoid=0;
			for ( ; it != m_boids.end(); ++it)
			{
				pBoid = (BoidOrbit*) *it;
				pBoid->setForceMax(m_boidsForceMax);
				pBoid->setSpeedMinMax(m_boidsSpeedMin, m_boidsSpeedMax);
			}

		}
	}


}

//--------------------------------------------------------------
void AnimationOrbit::updateUI()
{
	ParticleOrbitEllipse* pOrbitEllipse = (ParticleOrbitEllipse*) getOrbitForDevice( getDeviceCurrent() );

	if (mp_labelDeviceId){
		Device* pDeviceCurrent = getDeviceCurrent();
		if (pDeviceCurrent) mp_labelDeviceId->setLabel("Forms for "+pDeviceCurrent->m_id);
	}
	
	if (mp_toggleFormReverse && pOrbitEllipse)		mp_toggleFormReverse->setValue( pOrbitEllipse->getReverse() );
	if (mp_sliderFormRot && pOrbitEllipse)			mp_sliderFormRot->setValue( pOrbitEllipse->getRotation() );
	if (mp_sliderFormWidth && pOrbitEllipse)		mp_sliderFormWidth->setValue( pOrbitEllipse->getRadius().x );
	if (mp_sliderFormHeight && pOrbitEllipse)		mp_sliderFormHeight->setValue( pOrbitEllipse->getRadius().y );

}

//--------------------------------------------------------------
void AnimationOrbit::saveProperties(string id)
{
	Animation::saveProperties(id);

	ofxXmlSettings extraData;
	string extraDataFilename = getPropertiesFilename(id,false)+"_extradata.xml";

    extraData.addTag("orbits");
	extraData.pushTag("orbits");

	int i=0;
	for (m_orbitsIt = m_orbits.begin(); m_orbitsIt != m_orbits.end(); ++m_orbitsIt)
	{
		ParticleOrbit* pOrbit = m_orbitsIt->second;
		string deviceId = m_orbitsIt->first;

		extraData.addTag("orbit");
		extraData.setAttribute("orbit", "deviceId", deviceId, i);
		extraData.pushTag("orbit", i);
		pOrbit->save(extraData);

		extraData.popTag();

		i++;
	}
	
	extraData.popTag();
	
	extraData.saveFile(extraDataFilename);
}

//--------------------------------------------------------------
void AnimationOrbit::loadProperties(string id)
{
	Animation::loadProperties(id);

	ofxXmlSettings extraData;
	if ( extraData.loadFile( getPropertiesFilename(id,false)+"_extradata.xml" ) )
	{
		// Loop through device
		DeviceManager* pDeviceManager = GLOBALS->mp_deviceManager;
		if (pDeviceManager)
		{
		    extraData.pushTag("orbits");
			int nbTagOrbits = extraData.getNumTags("orbit");
			for (int i=0;i<pDeviceManager->m_listDevices.size();i++)
			{
				Device* pDevice = pDeviceManager->m_listDevices[i];
				for (int j=0; j<nbTagOrbits; j++ )
				{
					string deviceId = extraData.getAttribute("orbit", "deviceId", "???", j);
					if (deviceId == pDevice->m_id)
					{
						extraData.pushTag("orbit", j);
						
						// Factory here ?
						ParticleOrbitEllipse* pOrbit = 0;
						if (m_orbits.find(deviceId) != m_orbits.end())
						{
							pOrbit = (ParticleOrbitEllipse*) m_orbits[deviceId];
						}
						else{
							pOrbit = new ParticleOrbitEllipse();
							m_orbits[deviceId] = pOrbit;
						}
						
						if (pOrbit)
						{
							ofLog() << "- loading extra data for " << deviceId;
							pOrbit->load(extraData);

							// Create Particle which runs along this orbit
							createParticlePathsForOrbit(pOrbit);
						}
																
						extraData.popTag();
						break;
					}
				}
			}
			extraData.popTag();


		
	
		}
	}
	
	assignBoidsToPaths();
}


//--------------------------------------------------------------
void AnimationOrbit::createParticlePathsForOrbit(ParticleOrbit* pOrbit)
{
  for (int i=0;i<m_boidsNbParticlesPath;i++)
  {
	 ParticlePath* pParticlePath = new ParticlePath();
	 pParticlePath->setSegment(pOrbit, i * (int)pOrbit->getPoints().size() / m_boidsNbParticlesPath);

	 m_particlePaths.push_back( pParticlePath );
  }
}

//--------------------------------------------------------------
void AnimationOrbit::assignBoidsToPaths()
{
	// Assign orbit / point to boid
	if (m_particlePaths.size()>0)
	{
		for (int i=0;i<m_boids.size();i++)
		{
			int indexParticlePath = (int) ofRandom(m_particlePaths.size());
			// Choose a random particle
			((BoidOrbit*)m_boids[i])->setPath( m_particlePaths[indexParticlePath] );
		}
	}
}




