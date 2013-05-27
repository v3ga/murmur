//
//  animationWaveShader.cpp
//  murmur
//
//  Created by Julien on 16/05/13.
//
//

#include "animationWaveShader.h"
#include "soundManager.h"

//--------------------------------------------------------------
AnimationShaderWave::AnimationShaderWave(string name) : Animation(name)
{
    m_volume = 0.0f;
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_enter()
{
    m_imgSoundInput.getPixelsRef().allocate(600, 1, 1);
    M_loadShader("wave");
    //printf(">>>> %d\n", m_imgSoundInput.getPixelsRef().size());
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_update(float dt)
{
    SoundManager::instance()->setVolumeSoundMainNormalized(m_volume);
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_draw(float w, float h)
{
//    m_anchor.set(m_anchorNorm.x*w, m_anchorNorm.y*h);
//    printf("(%.3f,%.3f)-",m_anchor.x,m_anchor.y);
    
    ofBackground(0, 0, 0);
    ofSetColor(255);
//    m_imgSoundInput.draw(0,0,w,h/2);

    m_imgSoundInput.bind();
    m_shader.begin();
    m_shader.setUniform2f("resolution", w, h);
    m_shader.setUniform2f("anchor", m_anchor.x, m_anchor.y);
    m_shader.setUniform1f("texSize", m_imgSoundInput.getWidth());
    m_shader.setUniformTexture("texWave", m_imgSoundInput.getTextureReference(), 0);

    glBegin(GL_QUADS);
        glVertex3f(0,0,0);
        glVertex3f(w,0,0);
        glVertex3f(w,h,0);
        glVertex3f(0,h,0);
    glEnd();
    
    
    m_shader.end();
    m_imgSoundInput.unbind();

}

//--------------------------------------------------------------
void AnimationShaderWave::VM_exit()
{
    
}

//--------------------------------------------------------------
void AnimationShaderWave::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
    m_anchor.set(x, y);
    if (pDevicePacket)
    {
        m_volume = pDevicePacket->m_volume;
        
        // Shift
        ofFloatPixels& data = m_imgSoundInput.getPixelsRef();
        int nb = data.size();
        for (int i=nb-1;i>=1;i--)
            data[i] = data[i-1];
        data[0] = pDevicePacket->m_volume;
        m_imgSoundInput.update();
    }
}


