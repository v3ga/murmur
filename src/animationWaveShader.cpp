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
ShaderWave::ShaderWave()
{
	m_colorWhite.set(1.0f);
	m_colorDevice.set(1.0f,0.0f,0.0f);
	m_colorTarget = m_color = m_colorWhite;
	m_fColor = 0.8f;
	
	m_bUpdateTexture = false;
	
}


//--------------------------------------------------------------
void ShaderWave::update(float dt)
{
//	m_color += (m_colorTarget-m_color)*0.8f*dt;
	float f = m_fColor*dt;
	m_color.r += (m_colorTarget.r-m_color.r)*f;
	m_color.g += (m_colorTarget.g-m_color.g)*f;
	m_color.b += (m_colorTarget.b-m_color.b)*f;
}

//--------------------------------------------------------------
void ShaderWave::setSize(int w)
{
	if (w<100) w=100;

	if(m_imgSoundInput.width != w)
	{
		m_imgSoundInput.getPixelsRef().allocate(w,1,1);
		ofFloatPixels& data = m_imgSoundInput.getPixelsRef();

		int nb = data.size();
		int nbChannels = data.getNumChannels();
		nb /= nbChannels;
        for (int i=nb-1;i>=0;i--)
		{
        	data[i*nbChannels] 		= 0.0f;
           // data[i*nbChannels+1] 	= 0.0f;
           // data[i*nbChannels+2] 	= 0.0f;
		}
	}
}

//--------------------------------------------------------------
void ShaderWave::clearSoundInput()
{
//	m_imgSoundInput.clear();
	ofFloatPixels& data = m_imgSoundInput.getPixelsRef();
	int nb = data.size();
	for (int i=0;i<nb;i++)
	{
		data[i] = 0.0f;
	}
}



//--------------------------------------------------------------
AnimationShaderWave::AnimationShaderWave(string name) : Animation(name)
{
    m_volume = 0.0f;
	m_waveIntensity = 0.5f;
	m_isBlend = true;
	m_isColor = false;
	m_waveVolumeTexture = 1200;

	
	loadColors();
	m_properties.add( new classProperty_float("intensity", 0.0f, 1.0f, &m_waveIntensity) );
}

//--------------------------------------------------------------
AnimationShaderWave::~AnimationShaderWave()
{
	map<string,ShaderWave*>::iterator it = m_mapShaderWaves.begin();
	for ( ; it != m_mapShaderWaves.end() ; ++it)
		delete it->second;
}


//--------------------------------------------------------------
void AnimationShaderWave::VM_enter()
{
    M_loadShader("wave");
	setDrawBackground();
	if (m_bResetOnEnter)
	{
		map<string,ShaderWave*>::iterator it = m_mapShaderWaves.begin();
		for ( ; it != m_mapShaderWaves.end() ; ++it)
			it->second->clearSoundInput();
	}
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_update(float dt)
{
	updateUIVolume();

	map<string, ShaderWave*>::iterator it = m_mapShaderWaves.begin();
	for ( ; it!=m_mapShaderWaves.end(); ++it )
	{
		it->second->update(dt);
	}
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_draw(float w, float h)
{
//    m_anchor.set(m_anchorNorm.x*w, m_anchorNorm.y*h);
//    printf("(%.3f,%.3f)-",m_anchor.x,m_anchor.y);
    
	drawBackground(0);

    ofSetColor(255);
//	ofDisableAlphaBlending();
	if (m_isBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);
	}

	map<string,ShaderWave*>::iterator it = m_mapShaderWaves.begin();
	for ( ; it != m_mapShaderWaves.end() ; ++it)
	{
		ShaderWave* pShaderWave = it->second;
		if (pShaderWave->m_bUpdateTexture)
		{
			pShaderWave->m_imgSoundInput.update();
			pShaderWave->m_bUpdateTexture = false;
	//							glUseProgram(0);
  //      ofPtr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
//		renderer->currentShader = 0;
//		renderer->beginDefaultShader();

		}


	    pShaderWave->m_imgSoundInput.bind();
    	m_shader.begin();
    	m_shader.setUniform2f("resolution", 1.2f*w, 1.2f*h);
    	m_shader.setUniform2f("anchor", pShaderWave->m_anchor.x, pShaderWave->m_anchor.y);
    	m_shader.setUniform1f("texSize", pShaderWave->m_imgSoundInput.getWidth());
    	m_shader.setUniform1f("nbWaves", (float)m_mapShaderWaves.size());
    	m_shader.setUniform1f("intensityWave", m_waveIntensity);
    	m_shader.setUniformTexture("texWave", pShaderWave->m_imgSoundInput.getTextureReference(), 0);

 		ofRect(0,0,w,h);
    
    	m_shader.end();
    	pShaderWave->m_imgSoundInput.unbind();

	}
	if (m_isBlend)
	{
//	   glDisable(GL_BLEND);
	}

}

//--------------------------------------------------------------
void AnimationShaderWave::VM_exit()
{
    
}


//--------------------------------------------------------------
void AnimationShaderWave::onVolumAccumEvent(string deviceId)
{
	Animation::onVolumAccumEvent(deviceId);
}

//--------------------------------------------------------------
void AnimationShaderWave::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{

    if (pDevicePacket)
	{
		accumulateVolume(pDevicePacket->m_volume, deviceId);


		map<string,ShaderWave*>::iterator it = m_mapShaderWaves.find(deviceId);
		ShaderWave* pShaderWave=0;

		if ( it == m_mapShaderWaves.end()){
			pShaderWave = new ShaderWave();
			pShaderWave->setSize(m_waveVolumeTexture);

	   		m_mapShaderWaves[deviceId] = pShaderWave;
		}
		else{
			pShaderWave = it->second;
		}

		Device* pDevice = getDevice(deviceId);

		if (pShaderWave && pDevice)
		{
			pShaderWave->m_anchor.set(x, y);
		    pShaderWave->m_volume = pDevicePacket->m_volume;
			
			if (m_isColorFromDevice)
			{
				pShaderWave->m_color = pDevicePacket->m_color;
			}
			else if (m_isColor)
			{

				VolumeAccum* pVolumAccum = m_mapDeviceVolumAccum[deviceId];
				if (pVolumAccum)
				{
					if (pVolumAccum->m_valueMean >= m_volValuesMeanTh)
					{
						//ofLog() << deviceId << " here+++";
						if (m_mapVolMeanAbove[deviceId] == false)
						{
							m_mapVolMeanAbove[deviceId] = true;

							
							ofColor color = chooseRandomColor();

							pShaderWave->m_fColor = 0.9f;
							pShaderWave->m_colorTarget = ofFloatColor( (float)color.r/255.0f,  (float)color.g/255.0f, (float)color.b/255.0f);
							
							ofLog() << " - changing color for '"+deviceId+"' = "+ofToString(pShaderWave->m_colorTarget);
						}
					}
					else
					{
					//ofLog()  << deviceId << " here--";
						if (m_mapVolMeanAbove[deviceId] == true)
						{
							m_mapVolMeanAbove[deviceId] = false;


							pShaderWave->m_fColor = 0.2f;
							pShaderWave->m_colorTarget = pShaderWave->m_colorWhite;
						
							ofLog() << " - changing color for '"+deviceId+"' = "+ofToString(pShaderWave->m_colorTarget);
						}
					
					}
	
				}

			}
			else
			{
				pShaderWave->m_color.set(1.0f);
			}


	        ofFloatPixels& data = pShaderWave->m_imgSoundInput.getPixelsRef();
    	    int nb = data.size();
			int nbChannels = data.getNumChannels();
			nb /= nbChannels;
        	for (int i=nb-1;i>=1;i--)
			{
            	data[i*nbChannels] 		= data[(i-1)*nbChannels];
            	//data[i*nbChannels+1] 	= data[(i-1)*nbChannels+1];
            	//data[i*nbChannels+2] 	= data[(i-1)*nbChannels+2];
			}
			
        	data[0] = pShaderWave->m_color.r * pDevicePacket->m_volume;
        	//data[1] = pShaderWave->m_color.g * pDevicePacket->m_volume;
        	//data[2] = pShaderWave->m_color.b * pDevicePacket->m_volume;

        	//pShaderWave->m_imgSoundInput.update();
        	pShaderWave->m_bUpdateTexture = true;
		}
	}
}


//--------------------------------------------------------------
void AnimationShaderWave::createUICustom()
{
    if (mp_UIcanvas)
    {
		mp_UIcanvas->addIntSlider("sizeTexture", 400,1500, &m_waveVolumeTexture);
		addUISlider( m_properties.getFloat("intensity") );
//        mp_UIcanvas->addSlider("intensity", 0.0f, 1.0f, &m_waveIntensity);
        mp_UIcanvas->addToggle("debug enable blend", &m_isBlend);
        mp_UIcanvas->addToggle("colorFromDevice", &m_isColorFromDevice);
        mp_UIcanvas->addToggle("color", &m_isColor);

    }
}

//--------------------------------------------------------------
void AnimationShaderWave::setSizeVolumeTexture()
{
	map<string,ShaderWave*>::iterator it = m_mapShaderWaves.begin();
	for (; it!=m_mapShaderWaves.end(); ++it)
	{
		it->second->setSize(m_waveVolumeTexture);
	}
}


//--------------------------------------------------------------
void AnimationShaderWave::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);
	
	string name = e.getName();
	if (name == "sizeTexture")
	{
		m_waveVolumeTexture = ((ofxUIIntSlider*)e.widget)->getScaledValue();
		setSizeVolumeTexture();
	}
}



