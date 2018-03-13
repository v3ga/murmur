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
	
	m_bUpdateTexture = true;
	
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
		m_imgSoundInput.getPixelsRef().allocate(w,1,3);
		ofFloatPixels& data = m_imgSoundInput.getPixelsRef();

		int nb = data.size();
		int nbChannels = data.getNumChannels();
		nb /= nbChannels;
        for (int i=nb-1;i>=0;i--)
		{
		   data[i*nbChannels] 		= 0.0f;
           data[i*nbChannels+1] 	= 0.0f;
           data[i*nbChannels+2] 	= 0.0f;
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
	m_bHandlePitch		= true;
	
	loadColors();
	loadPropertiesDefinition();

	m_properties.add( new classProperty_float("intensity", 0.0f, 1.0f, &m_waveIntensity) );
	m_properties.add( new classProperty_float("ampSine", getPropDef("ampsine:min", 0.0f), getPropDef("ampsine:max", 100.0f), &m_waveAmpSine) );
	m_properties.add( new classProperty_float("freqSine", getPropDef("freqsine:min", 1.0f), getPropDef("freqsine:max", 30.0f), &m_waveFreqSine) );
	m_properties.add( new classProperty_float("freqCosine", getPropDef("freqcosine:min", 1.0f), getPropDef("freqcosine:max", 3.0f), &m_waveFreqCosine) );
	m_properties.add( new classProperty_float("volRetainPitch", 0.0f, 1.0f, &m_volRetainPitch) );


	m_time = 0.0f;
	m_pitchCurrent = 0.0f;
	m_pitchRelax = 0.5f;
	m_volRetainPitch = 0.3f;
	mp_sliderAmpSine = 0;
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
		m_time = 0.0f;
	}
}

//--------------------------------------------------------------
void AnimationShaderWave::VM_update(float dt)
{
	m_time += dt;

	updateUIVolume();
	if (hasPitch())
	{
		m_pitchRelax = 0.1;
		m_pitchCurrent += (m_pitchLast - m_pitchCurrent)*m_pitchRelax;
//		m_pitchCurrent = m_pitchLast;
//		if (mp_sliderAmpSine) mp_sliderAmpSine->setValue( m_pitchCurrent*m_waveAmpSine );
	}
	else
	{
		m_pitchCurrent = 1.0f;
	}


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
//	ofSetColor(0,255,0);
//	ofDrawBitmapString( ofToString(m_pitchCurrent) + " / "+ ofToString(m_pitchLast), 20,20 );

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
		}

	    pShaderWave->m_imgSoundInput.bind();
    	m_shader.begin();
    	m_shader.setUniform2f("resolution", 1.2f*w, 1.2f*h);
    	m_shader.setUniform2f("anchor", pShaderWave->m_anchor.x, pShaderWave->m_anchor.y);
    	m_shader.setUniform1f("texSize", pShaderWave->m_imgSoundInput.getWidth());
//    	m_shader.setUniform1f("nbWaves", (float)m_mapShaderWaves.size());
    	m_shader.setUniform1f("ampSine", m_pitchCurrent*m_waveAmpSine);
    	m_shader.setUniform1f("freqSine", (int)m_waveFreqSine);
    	m_shader.setUniform1f("freqCosine", m_waveFreqCosine);
    	m_shader.setUniform1f("intensityWave", m_waveIntensity);
    	m_shader.setUniform1f("time", m_time);
    	m_shader.setUniformTexture("texWave", pShaderWave->m_imgSoundInput.getTextureReference(), 0);

    	ofSetColor(255,255);
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
//		accumulateVolume(pDevicePacket->m_volume, deviceId);
		accumulateVolumeAndPitch(pDevicePacket->m_volume, pDevicePacket->m_pitch, deviceId);
//		ofLog() << pDevicePacket->m_volume;

		if (pDevicePacket->m_volume > m_volRetainPitch)
		{
			m_pitchLast = pDevicePacket->m_pitch;
		}
/*			m_pitchLast = getVolumAccumForDevice(deviceId)->m_valuePitchMean;
		if (m_pitchLast < 0.5f)
			m_pitchRelax = 0.05;
		else
			m_pitchRelax = 0.99;
*/
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
            	data[i*nbChannels+1] 	= data[(i-1)*nbChannels+1];
            	data[i*nbChannels+2] 	= data[(i-1)*nbChannels+2];
			}
			
        	data[0] = pShaderWave->m_color.r * pDevicePacket->m_volume;
        	data[1] = pShaderWave->m_color.g * pDevicePacket->m_volume;
        	data[2] = pShaderWave->m_color.b * pDevicePacket->m_volume;

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
		mp_sliderAmpSine =  addUISlider( m_properties.getFloat("ampSine") );
		addUISlider( m_properties.getFloat("freqSine") );
		addUISlider( m_properties.getFloat("freqCosine") );
		addUISlider( m_properties.getFloat("volRetainPitch") );


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



