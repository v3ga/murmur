//
//  animationGridsFF.cpp
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//



#include "animationGridsFF.h"
#include "animationWaveShader.h"
#include "globals.h"

//--------------------------------------------------------------
AnimationGridsFF::AnimationGridsFF(string name) : Animation(name)
{
	m_meshGridRatio 	= 0.9f;
	m_bCreateMeshGrid 	= true;
	m_meshGridColumns	= 30;
	m_meshGridRows		= 30;
	m_bSquare			= false;
	m_w					= 0;
	m_h					= 0;
	m_bReloadShader 	= false;
	m_bDebug			= false;
	m_amplitude			= 200.0f;
	m_rotationX			= 0.0f;
	m_blurAmount		= 10.0f;
	m_blurNbPasses		= 2;
	m_colorFromDeviceLuminance = 0.5f;
	m_speed				= 500;
	m_torsion			= 0.0f;
 	m_torsionFactor		= 1.0f;
	m_torsionRelaxation = 0.9f;
 
	m_properties.add( new classProperty_int("columns",2,150,&m_meshGridColumns ) );
	m_properties.add( new classProperty_int("rows",2,50,&m_meshGridRows ) );
	m_properties.add( new classProperty_float("scale",0.2f,2.0f,&m_meshGridRatio ) );
	m_properties.add( new classProperty_int("speed",500,1500,&m_speed ) );
	m_properties.add( new classProperty_float("amplitude",0,500,&m_amplitude ) );
	m_properties.add( new classProperty_float("rotation",-180,180,&m_rotationX ) );
	m_properties.add( new classProperty_float("blurAmount",1,30,&m_blurAmount ) );
	m_properties.add( new classProperty_int("blurPasses",1,5,&m_blurNbPasses ) );
	m_properties.add( new classProperty_float("luminance",0,1,&m_colorFromDeviceLuminance ) );
	m_properties.add( new classProperty_float("torsionRelaxation",0.5f,1.0f,&m_torsionRelaxation ) );
	m_properties.add( new classProperty_float("torsionFactor",0,1,&m_torsionFactor ) );
	m_properties.add( new classProperty_float("torsionRadius",0,1,&m_torsionRadius ) );
	
	m_imgTorsion.loadImage("Images/degrade_radial_0.2.png");

	//m_fboTorsionScale = 0.25f;
	
	loadShader();
	m_blur.loadShaders(ofToDataPath("Shaders"));
	
	mp_animShaderWave = new AnimationShaderWave("gridsFF_wave");
	mp_animShaderWave->M_loadShader("wave");
}

//--------------------------------------------------------------
AnimationGridsFF::~AnimationGridsFF()
{
	delete mp_animShaderWave;
}

//--------------------------------------------------------------
void AnimationGridsFF::createUICustom()
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->addToggle("debug", &m_bDebug);
		mp_UIcanvas->addButton("reload shader", &m_bReloadShader);
        mp_UIcanvas->addToggle("colorFromDevice", &m_isColorFromDevice);
		addUISlider(m_properties.getFloat("luminance"));

		addUISlider(m_properties.getInt("columns"));
		addUISlider(m_properties.getInt("rows"));
        mp_UIcanvas->addToggle("square cells", &m_bSquare);
		addUISlider(m_properties.getFloat("scale"));
		addUISlider(m_properties.getInt("speed"));
		addUISlider(m_properties.getFloat("amplitude"));
		addUISlider(m_properties.getFloat("rotation"));
		addUISlider(m_properties.getFloat("blurAmount"));
		addUISlider(m_properties.getInt("blurPasses"));
		addUISlider(m_properties.getFloat("torsionRelaxation"));
		addUISlider(m_properties.getFloat("torsionFactor"));
		addUISlider(m_properties.getFloat("torsionRadius"));
	}
}

//--------------------------------------------------------------
void AnimationGridsFF::VM_enter()
{
	setDrawBackground();
	if (m_bResetOnEnter)
	{
		if (mp_animShaderWave)
		{
			mp_animShaderWave->m_bResetOnEnter = m_bResetOnEnter; // force flag here
			mp_animShaderWave->VM_enter();
		}

		m_fbo.begin();
		ofBackground(0);
		m_fbo.end();
	}
}


//--------------------------------------------------------------
void AnimationGridsFF::VM_update(float dt)
{
	updateUIVolume();

	vector<Device*>& devices = GLOBALS->mp_deviceManager->m_listDevices;
		
	Device* pDevice = 0;
	vector<Device*>::iterator it = devices.begin();
	for ( ; it != devices.end(); ++it)
	{
		pDevice = *it;
		getDeviceTorsion( pDevice->getID() )->m_relaxation = m_torsionRelaxation; // update directly
		getDeviceTorsion( pDevice->getID() )->update( dt );
	}
	
	if (mp_animShaderWave)
		mp_animShaderWave->VM_update(dt);

	if (m_fbo.isAllocated() && m_w>0)
	{
		m_fbo.begin();
		if (mp_animShaderWave)
			mp_animShaderWave->VM_draw(m_w,m_h);
		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		ofRect(0,0,m_w,m_h);
		ofPopStyle();
		m_fbo.end();
		m_blur.apply(m_fbo,m_blurAmount,m_blurNbPasses);
	}
}

//--------------------------------------------------------------
void AnimationGridsFF::VM_draw(float w, float h)
{
	if (w!=m_w && h!=m_h)
	{
		m_w = w;
		m_h = h;

		m_fbo.allocate(w,h);
		m_blur.allocate(w,h);

		m_bCreateMeshGrid = true;
	}

	if (m_bCreateMeshGrid)
	{
		computeMesh(w,h);
		m_bCreateMeshGrid = false;
	}

	drawBackground(0);

	if (m_bDebug)
		m_fbo.draw(0,0,w,h);
	
	if (m_blur.get().isAllocated())
	{
		for (int i=0 ; i<20; i++)
		{
			m_devicePos[i]=0.0f;
			m_deviceTorsion[i]=0.0f;
		}

		int index=0;
		vector<Device*>& devices = GLOBALS->mp_deviceManager->m_listDevices;
		
		Device* pDevice = 0;
		vector<Device*>::iterator it = devices.begin();
		for ( ; it != devices.end(); ++it)
		{
			pDevice = *it;
			if (pDevice->isGenerative())
			{
				m_devicePos[2*index] 	= pDevice->m_pointSurface.x*m_fbo.getWidth()-w/2;
				m_devicePos[2*index+1]	= pDevice->m_pointSurface.y*m_fbo.getHeight()-h/2;
				
				m_deviceTorsion[index]	= m_torsionFactor * getDeviceTorsion( pDevice->getID() )->m_value * TWO_PI;

				index++;
			}
			else
			{
				m_devicePos[2*index] 	= 0.0f;
				m_devicePos[2*index+1]	= 0.0f;

				m_deviceTorsion[index] 	= 0.0f;
			}
		}
	
		if (pDevice)
		{
			m_shader.begin();
			m_shader.setUniform1f("amplitude", m_amplitude);
			m_shader.setUniform1i("colorFromDevice", m_isColorFromDevice ? 1 : 0);
			m_shader.setUniform1f("luminance", m_colorFromDeviceLuminance);
			m_shader.setUniformTexture("tex0", m_blur.get().getTextureReference(), 0);
			m_shader.setUniform2fv("devicePos", m_devicePos, index*2);
			m_shader.setUniform1i("devicePosNb", index);
			m_shader.setUniform1fv("torsion", m_deviceTorsion, index);
			m_shader.setUniform1f("torsionRadius", m_torsionRadius * min(w,h));

			ofPushMatrix();
			ofTranslate(w/2,h/2);
			ofRotateX(m_rotationX);
			m_meshGrid.draw();
			ofPopMatrix();
			m_shader.end();
		}

	}
}

//--------------------------------------------------------------
void AnimationGridsFF::computeMesh(float w, float h)
{
	m_meshGrid.clear();
	m_meshGrid.setUsage(GL_STATIC_DRAW);
	m_meshGrid.setMode(OF_PRIMITIVE_LINES);
	m_meshGrid.enableColors();
	m_meshGrid.enableIndices();
	m_meshGrid.enableTextures();

	float wGrid = m_meshGridRatio*w;
	float hGrid = m_meshGridRatio*h;

	float wCell = wGrid/(float)m_meshGridColumns;
	float hCell = hGrid/(float)m_meshGridRows;

	if (m_bSquare)
	{
		hCell = wCell;
		m_meshGridRows = (int) (hGrid / hCell);
		hGrid = m_meshGridRows * hCell;
	}

	
	int nbPointsColumn 	= m_meshGridColumns+1;
	int nbPointsRows 	= m_meshGridRows+1;
	
	float ustep = w/(float)m_meshGridColumns;
	float vstep = h/(float)m_meshGridRows;
	
	// Vertices
	float y = -0.5f*hGrid;
	float v = 0.0f;
	for (int j=0;j<nbPointsRows;j++)
	{
		float x = -0.5f*wGrid;
		float u = 0.0f;
		for (int i=0;i<nbPointsColumn;i++)
		{
			m_meshGrid.addVertex( ofVec3f(x,y,0) );
			m_meshGrid.addTexCoord(ofVec2f(w/2+x,h/2+y));
			m_meshGrid.addColor(ofFloatColor(1.0f));
			
			x+=wCell;
			u+=ustep;
		}
		y+=hCell;
		v+=vstep;
	}
	
	// Indices
	for (int j=0;j<m_meshGridRows;j++)
	{
		for (int i=0;i<=m_meshGridColumns;i++)
		{
			m_meshGrid.addIndex(i 	+ j*nbPointsColumn);
			m_meshGrid.addIndex(i 	+ (j+1)*nbPointsColumn);
		}
	}

	for (int j=0;j<=m_meshGridRows;j++)
	{
		for (int i=0;i<m_meshGridColumns;i++)
		{
			m_meshGrid.addIndex(i 	+ j*nbPointsColumn);
			m_meshGrid.addIndex(i+1 + j*nbPointsColumn);
		}
	}
}

//--------------------------------------------------------------
void AnimationGridsFF::loadShader()
{
	m_shader.load(ofToDataPath("Shaders/gridsFF.vert"), ofToDataPath("Shaders/gridsFF.frag"));
}

//--------------------------------------------------------------
void AnimationGridsFF::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{

	accumulateVolume(pDevicePacket->m_volume, deviceId);

	getDeviceTorsion(deviceId)->set( pDevicePacket->m_volume );
	
	if (mp_animShaderWave)
		mp_animShaderWave->onNewPacket(pDevicePacket, deviceId, x,y);
}

//--------------------------------------------------------------
void AnimationGridsFF::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);

	string name = e.getName();
	if (name == "square cells" || name == "scale" || name == "columns" || name == "rows")
	{
		handlePropertyModified(name);
	}
	else if (name == "reload shader")
	{
		if (m_bReloadShader)
			loadShader();
	}
	else if (name == "colorFromDevice")
	{
		if (mp_animShaderWave)
			mp_animShaderWave->m_isColorFromDevice = m_isColorFromDevice;
	}
	else if (name == "speed")
	{
		if (mp_animShaderWave)
		{
			mp_animShaderWave->m_waveVolumeTexture = m_speed;
			mp_animShaderWave->setSizeVolumeTexture();
		}
	}
}

//--------------------------------------------------------------
void AnimationGridsFF::onPropertyMidiModified(classProperty* pProperty)
{
	if (pProperty)
	{
		handlePropertyModified(pProperty->m_name);
	}
}

//--------------------------------------------------------------
void AnimationGridsFF::handlePropertyModified(string name)
{
	if (name == "square cells" || name == "scale" || name == "columns" || name == "rows")
	{
		m_bCreateMeshGrid = true;
	}
}


//--------------------------------------------------------------
floatRelax*	AnimationGridsFF::getDeviceTorsion(string deviceId)
{
	map<string, floatRelax*>::iterator it =  m_deviceTorsionValues.find(deviceId);

	if (it == m_deviceTorsionValues.end())
	{
		m_deviceTorsionValues[deviceId] = new floatRelax();
	}

	return m_deviceTorsionValues[deviceId];
}




