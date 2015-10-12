//
//  ParticleSystem.cpp
//  murmur
//
//  Created by Julien on 02/10/2015.
//
//

#include "ParticleSystem.h"

//--------------------------------------------------------------
ParticleSystem::ParticleSystem() :
timeStep(1) {
}

//--------------------------------------------------------------
void ParticleSystem::setup(int width, int height, int k)
{
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
					if (doColor)
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
