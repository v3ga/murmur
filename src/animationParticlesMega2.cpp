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
ParticleForce::ParticleForce(float anchorx, float anchory)
{
	m_anchor.set(anchorx, anchory);
	m_volume = m_volumeTarget = 0.0f;
}


//--------------------------------------------------------------
void ParticleForce::update(float dt)
{
    m_volume += (m_volumeTarget-m_volume)*0.4f;
}


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
}

//--------------------------------------------------------------
void AnimationParticlesMega2::createUICustom()
{
    if (mp_UIcanvas)
    {
		mp_UIcanvas->addToggle("colorFromDevice", 	&m_isColorFromDevice);

		m_properties.add( new classProperty_float("amp. attraction",1.0f, 100.0f, &m_ampAttraction) );
		m_properties.add( new classProperty_float("amp. repulsion", 1.0f, 100.0f, &m_ampRepulsion) );
		m_properties.add( new classProperty_float("radius repulsion", 40.0f, 300.0f, &m_repulsionRadius) );
		m_properties.add( new classProperty_float("particles size", 1.0f, 5.0f, &m_particlesSize) );
		m_properties.add( new classProperty_float("radius color factor", 1.0f, 10.0f, &m_colorRadiusFactor) );
	

		addUISlider( m_properties.getFloat("amp. attraction") );
		addUISlider( m_properties.getFloat("amp. repulsion") );
		addUISlider( m_properties.getFloat("radius repulsion") );
		addUISlider( m_properties.getFloat("particles size") );
		addUISlider( m_properties.getFloat("radius color factor") );
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
    m_volume += (m_volumeTarget-m_volume)*0.4f;
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
		accumulateVolume(pDevicePacket->m_volume, deviceId);

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

		 pParticleForce->m_color = pDevice->m_color;
		 //printf("pDevicePacket->m_volume = %.5f\n", pDevicePacket->m_volume);
	 }


/*
    m_anchor.set(x, y);
    if (pDevicePacket)
    {
        m_volumeTarget = pDevicePacket->m_volume;
    }
*/
}




//--------------------------------------------------------------
ParticleSystem::ParticleSystem() :
timeStep(1) {
}

//--------------------------------------------------------------
void ParticleSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k;
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);

}

//--------------------------------------------------------------
void ParticleSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

//--------------------------------------------------------------
void ParticleSystem::add(Particle particle) {
	particles.push_back(particle);
}

//--------------------------------------------------------------
unsigned ParticleSystem::size() const {
	return particles.size();
}

//--------------------------------------------------------------
Particle& ParticleSystem::operator[](unsigned i) {
	return particles[i];
}

//--------------------------------------------------------------
vector<Particle*> ParticleSystem::getNeighbors(Particle& particle, float radius) {
	return getNeighbors(particle.mp_pos->x, particle.mp_pos->y, radius);
}

//--------------------------------------------------------------
vector<Particle*> ParticleSystem::getNeighbors(float x, float y, float radius) {
	vector<Particle*> region = getRegion(
                                         (int) (x - radius),
                                         (int) (y - radius),
                                         (int) (x + radius),
                                         (int) (y + radius));
	vector<Particle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		Particle& cur = *region[i];
		xd = cur.mp_pos->x - x;
		yd = cur.mp_pos->y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

//--------------------------------------------------------------
vector<Particle*> ParticleSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<Particle*> region;
	back_insert_iterator< vector<Particle*> > back = back_inserter(region);
	unsigned minXBin = minX >> k;
	unsigned maxXBin = maxX >> k;
	unsigned minYBin = minY >> k;
	unsigned maxYBin = maxY >> k;
	maxXBin++;
	maxYBin++;
	if(maxXBin > xBins)
		maxXBin = xBins;
	if(maxYBin > yBins)
		maxYBin = yBins;
	for(int y = minYBin; y < maxYBin; y++) {
		for(int x = minXBin; x < maxXBin; x++) {
			vector<Particle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

//--------------------------------------------------------------
void ParticleSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		Particle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.mp_pos->x) >> k;
		yBin = ((unsigned) cur.mp_pos->y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

//--------------------------------------------------------------
void ParticleSystem::addRepulsionForce(const Particle& particle, float radius, float scale, bool doColor) {
	addRepulsionForce(particle.mp_pos->x, particle.mp_pos->y, radius, scale);
}

//--------------------------------------------------------------
void ParticleSystem::addRepulsionForce(float x, float y, float radius, float scale, bool doColor) {
	addForce(x, y, radius, scale, doColor);
}

//--------------------------------------------------------------
void ParticleSystem::addAttractionForce(const Particle& particle, float radius, float scale, bool doColor) {
	addAttractionForce(particle.mp_pos->x, particle.mp_pos->y, radius, scale, doColor);
}

//--------------------------------------------------------------
void ParticleSystem::addAttractionForce(float x, float y, float radius, float scale, bool doColor) {
	addForce(x, y, radius, -scale, doColor);
}

//--------------------------------------------------------------
void ParticleSystem::addForce(const Particle& particle, float radius, float scale, bool doColor) {
	addForce(particle.mp_pos->x, particle.mp_pos->y, radius, -scale, doColor);
}

//--------------------------------------------------------------
void ParticleSystem::addForce(float targetX, float targetY, float radius, float scale, bool doColor)
{
	float minX = targetX - radius;
	float minY = targetY - radius;
	float maxX = targetX + radius;
	float maxY = targetY + radius;
	if(minX < 0)
		minX = 0;
	if(minY < 0)
		minY = 0;
	unsigned minXBin = ((unsigned) minX) >> k;
	unsigned minYBin = ((unsigned) minY) >> k;
	unsigned maxXBin = ((unsigned) maxX) >> k;
	unsigned maxYBin = ((unsigned) maxY) >> k;
	maxXBin++;
	maxYBin++;
	if(maxXBin > xBins)
		maxXBin = xBins;
	if(maxYBin > yBins)
		maxYBin = yBins;
	float xd, yd, length, maxrsq;
#ifdef USE_INVSQRT
	float xhalf;
	int lengthi;
#else
	float effect;
#endif
	maxrsq = radius * radius;
	for(int y = minYBin; y < maxYBin; y++) {
		for(int x = minXBin; x < maxXBin; x++) {
			vector<Particle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				Particle& curParticle = *(curBin[i]);
				xd = curParticle.mp_pos->x - targetX;
				yd = curParticle.mp_pos->y - targetY;
				length = xd * xd + yd * yd;
				if(length > 0 && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= radius;
                    length = 1 / length;
                    length = (1 - length);
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curParticle.xf += xd;
                    curParticle.yf += yd;
					if (doColor)
						*curParticle.mp_color = m_color;
#else
                    length = sqrtf(length);
                    xd /= length;
                    yd /= length;
                    effect = (1 - (length / radius)) * scale;
                    curParticle.xf += xd * effect;
                    curParticle.yf += yd * effect;
					*curParticle.mp_color = m_color;
#endif
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ParticleSystem::update()
{
	int n = particles.size();
	for(int i = 0; i < n; i++)
	{
		particles[i].updatePosition(timeStep);
	}
}

//--------------------------------------------------------------
void ParticleSystem::draw()
{
	int n = particles.size();
	glPointSize(particleSize);
	glEnable(GL_POINT_SPRITE);
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
		particles[i].draw();
	glEnd();
	glDisable(GL_POINT_SPRITE);

}
