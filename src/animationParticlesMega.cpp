//
//  animationParticlesMega.cpp
//  murmur
//
//  Created by Julien on 17/05/13.
//
//

#include "animationParticlesMega.h"


//--------------------------------------------------------------
AnimationParticlesMega::AnimationParticlesMega(string name) : Animation(name)
{
    m_isCLInit = false;
}

//--------------------------------------------------------------
void AnimationParticlesMega::createParticles()
{
    if (m_resolution.x>0.0 && m_resolution.y>0.0 && !m_isCLInit)
    {
        opencl.setupFromOpenGL();
        
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            Particle &p = particles[i];
            p.vel.set(0, 0);
            p.mass = ofRandom(0.5, 1);
            particlesPos[i].set(ofRandomWidth(), ofRandomHeight());
        }
        
        glGenBuffersARB(1, vbo);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float2) * NUM_PARTICLES, particlesPos, GL_DYNAMIC_COPY_ARB);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        
        opencl.loadProgramFromFile("Shaders/Particle.cl");
        kernelUpdate = opencl.loadKernel("updateParticle");

        clMemParticles.initBuffer(sizeof(Particle) * NUM_PARTICLES, CL_MEM_READ_WRITE, particles);
        clMemPosVBO.initFromGLObject(vbo[0]);
        
        kernelUpdate->setArg(0, clMemParticles.getCLMem());
        kernelUpdate->setArg(1, clMemPosVBO.getCLMem());
        kernelUpdate->setArg(2, mousePos);
        kernelUpdate->setArg(3, dimensions);
        
        m_isCLInit = true;
    
    }
        
}


//--------------------------------------------------------------
void AnimationParticlesMega::VM_enter()
{
}

//--------------------------------------------------------------
void AnimationParticlesMega::VM_update(float dt)
{
    dimensions.x = m_resolution.x;
    dimensions.y = m_resolution.y;
    
    mousePos.x = m_anchor.x;
    mousePos.y = m_anchor.y;
	
	kernelUpdate->setArg(2, mousePos);
	kernelUpdate->setArg(3, dimensions);
	kernelUpdate->run1D(NUM_PARTICLES);
}

//--------------------------------------------------------------
void AnimationParticlesMega::VM_draw(float w, float h)
{
    ofBackground(0, 0, 0);
    m_resolution.set(w,h);
    createParticles();

    glPointSize(1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	opencl.finish();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	glColor3f(1, 0, 0);
	string info = "fps: " + ofToString(ofGetFrameRate()) + "\nnumber of particles: " + ofToString(NUM_PARTICLES);
	ofDrawBitmapString(info, 20, 20);

}

//--------------------------------------------------------------
void AnimationParticlesMega::VM_exit()
{
    
}

//--------------------------------------------------------------
void AnimationParticlesMega::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
    m_anchor.set(x, y);
    if (pDevicePacket)
    {
    }
}


