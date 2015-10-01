//
//  animationComposition.cpp
//  murmur
//
//  Created by Julien on 25/06/2015.
//
//

#include "animationComposition.h"
#include "animationsFactory.h"
#include "globals.h"

//--------------------------------------------------------------
AnimationComposition::AnimationComposition(string name) : Animation(name)
{
	m_bAllocateRenderBuffers = true;
	m_bRenderNormal = false;
	
	mp_radioBlending = 0;
	mp_radioCompositions=0;
}

//--------------------------------------------------------------
AnimationComposition::~AnimationComposition()
{
	vector<AnimationCompoConfiguration*>::iterator it = m_compositions.begin();
	for ( ; it != m_compositions.end(); ++it)
		delete *it;
	m_compositions.clear();
}

//--------------------------------------------------------------
void AnimationComposition::createUICustom()
{
	if (mp_UIcanvas)
	{
		mp_UIcanvas->addToggle("renderNormal", &m_bRenderNormal);
		
		mp_UIcanvas->addWidgetDown(new ofxUILabel("Blending", OFX_UI_FONT_MEDIUM));
    	mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));
		
		vector<string> blendingNames;
		blendingNames.push_back("multiply");
//		blendingNames.push_back("difference");
		mp_radioBlending = new ofxUIRadio("radioBlending",  blendingNames, OFX_UI_ORIENTATION_HORIZONTAL, 16, 16);

		mp_UIcanvas->addWidgetDown(mp_radioBlending);

		mp_UIcanvas->addWidgetDown(new ofxUILabel("Compositions", OFX_UI_FONT_MEDIUM));
    	mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));

		vector<string> compositionNames;
		for (int i=0; i< m_compositions.size(); i++)
			compositionNames.push_back(m_compositions[i]->m_name);

		mp_radioCompositions = new ofxUIRadio("radioCompositions", compositionNames, OFX_UI_ORIENTATION_VERTICAL, 16, 16);
		mp_UIcanvas->addWidgetDown(mp_radioCompositions);


	}
}

//--------------------------------------------------------------
void AnimationComposition::loadMidiSettings()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->loadMidiSettings();
}

//--------------------------------------------------------------
void AnimationComposition::loadConfiguration(string filename)
{
	OFAPPLOG->begin("AnimationComposition::loadConfiguration("+filename+")");
	Animation::loadConfiguration(filename);
	OFAPPLOG->end();
}


//--------------------------------------------------------------
void AnimationComposition::setRenderNormal(bool is)
{
	m_bRenderNormal = is;
	Surface* pSurface = GLOBALS->getSurfaceMain();


	for (int i=0;i<m_animations.size(); i++){
		m_animations[i]->setDrawBackground(!is);
	}


	if (m_bRenderNormal)
	{
	 	if (pSurface)
			pSurface->setRenderOffscreenCb(0,0);

	}
	else
	{
	 	if (pSurface)
			pSurface->setRenderOffscreenCb((Surface::FRenderOffscreen)onSurfaceRenderOffscreen, this);
	}
}


//--------------------------------------------------------------
Animation* AnimationComposition::getAnimation(string name)
{
   return GLOBALS->getSurfaceMain()->getAnimationManager().M_getAnimationByName(name);
}

//--------------------------------------------------------------
void AnimationComposition::addAnimation(string name)
{
	// limit to 2 animations for the moment
	//if (m_animations.size()<2)
	{
		Animation* pAnimation = AnimationsFactory::create(name);
		if (pAnimation){
			pAnimation->setDrawBackground(false);
			m_animations.push_back(pAnimation);
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::setBlending(string name)
{
	if (name == "multiply")
	{
		m_shader.load("Shaders/animCompo.vert", "Shaders/animCompoMultiply.frag");
	}
	else
	if (name == "difference")
	{
		m_shader.load("Shaders/animCompo.vert", "Shaders/animCompoDifference.frag");
	}
	else
	{
		setRenderNormal(true);
	}
}

//--------------------------------------------------------------
void AnimationComposition::setComposition( string name )
{
	vector<AnimationCompoConfiguration*>::iterator it = m_compositions.begin();
	AnimationCompoConfiguration* pComposition = 0;
	for ( ; it != m_compositions.end(); ++it)
	{
		if ((*it)->m_name == name){
			pComposition = *it;
			break;
		}
	}
	
	if (pComposition)
	{
		if (m_animations.size()>0)
		{
			if (m_animations[0]) m_animations[0]->setDrawBackground();
			if (m_animations[1]) m_animations[1]->setDrawBackground();
		}
		m_animations.clear();


		Animation* pAnim1 = getAnimation(pComposition->m_animationName1);
		Animation* pAnim2 = getAnimation(pComposition->m_animationName2);

		if (pAnim1 && pAnim2)
		{
			m_animations.push_back( pAnim1 );
			m_animations.push_back( pAnim2 );
			
			pAnim1->setDrawBackground(true);
			pAnim2->setDrawBackground(false);
			
			pAnim1->loadConfiguration(pComposition->m_animationConfig1);
			pAnim2->loadConfiguration(pComposition->m_animationConfig2);
		}
	}
	
}
//--------------------------------------------------------------
void AnimationComposition::readSurfaceSettings(ofxXmlSettings& settings)
{
	OFAPPLOG->begin("AnimationComposition::readSurfaceSettings()");
	int nbCompositions = settings.getNumTags("composition");
	OFAPPLOG->println("- nb compositions="+ofToString(nbCompositions));

	for (int i=0;i<nbCompositions;i++)
	{
		string name = settings.getAttribute("composition", "name", "",i);
		settings.pushTag("composition",i);
		int nbAnimations = settings.getNumTags("animation");
		if (nbAnimations == 2 && name!="")
		{
			string anim1 	= settings.getAttribute("animation", "name", "",0);
			string config1 	= settings.getAttribute("animation", "configuration", "",0);

			string anim2 	= settings.getAttribute("animation", "name", "",1);
			string config2 	= settings.getAttribute("animation", "configuration", "",1);

			m_compositions.push_back( new AnimationCompoConfiguration(name,anim1,config1,anim2,config2) );

			OFAPPLOG->println("- new composition ["+name+"] with "+anim1+" ("+config1+") + "+anim2+" ("+config2+")" );

		}
		settings.popTag();
	}

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void AnimationComposition::loadProperties(string id)
{

	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->loadProperties( id+"_"+(*it)->m_name );

	Animation::loadProperties(id);
}

//--------------------------------------------------------------
void AnimationComposition::VM_enter()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_enter();
 
	Surface* pSurface = GLOBALS->getSurfaceMain();
 	if (pSurface)
	{
		pSurface->setRenderOffscreenCb((Surface::FRenderOffscreen)onSurfaceRenderOffscreen, this);
		ofAddListener(pSurface->m_eventOffscreenChanged, this, &AnimationComposition::onSurfaceOffscreenChanged);

	}
	
	allocateRenderBuffers();
	if (m_compositionCurrent!="") setComposition(m_compositionCurrent);
}

//--------------------------------------------------------------
void AnimationComposition::VM_update(float dt)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_update(dt);

	if (m_bAllocateRenderBuffers)
	{
		m_bAllocateRenderBuffers=false;
		Surface* pSurface = GLOBALS->getSurfaceMain();
		if (pSurface)
		{
			float w = pSurface->getOffscreen().getWidth();
			float h = pSurface->getOffscreen().getHeight();
			int nbSamples = pSurface->m_fboNbSamples;
		
			m_fboAnimation1.allocate(w,h, GL_RGBA,nbSamples);
			m_fboAnimation2.allocate(w,h, GL_RGBA,nbSamples);
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::VM_draw(float w, float h)
{
	if ( m_compositionCurrent != m_compositionWanted )
	{
		m_compositionCurrent = m_compositionWanted;
		setComposition(m_compositionCurrent);
	}

	if (m_bRenderNormal)
	{
		drawBackground(0);
		vector<Animation*>::iterator it = m_animations.begin();
		for ( ; it != m_animations.end(); ++it)
		{
			ofPushStyle();
			ofPushMatrix();
	 		(*it)->VM_draw(w,h);
			ofPopMatrix();
			ofPopStyle();
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::VM_exit()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	{
	 	(*it)->setDrawBackground();
	 	(*it)->VM_exit();
	}

	Surface* pSurface = GLOBALS->getSurfaceMain();
 	if (pSurface)
		pSurface->setRenderOffscreenCb(0,0);
}


//--------------------------------------------------------------
void AnimationComposition::onNewPacket(DevicePacket* pDevice, string deviceId, float x, float y)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
		(*it)->onNewPacket(pDevice,deviceId,x,y);
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceRenderOffscreen(Surface* pSurface, AnimationComposition* pThis)
{
	if (pThis->m_bRenderNormal == false)
	{
		ofFbo& fboFinal = pSurface->getOffscreen();
	
		if (pThis->m_animations.size()==2 && pThis->m_animations[0] && pThis->m_animations[1] && pThis->m_fboAnimation1.isAllocated() && pThis->m_fboAnimation2.isAllocated())
		{
			pThis->m_fboAnimation1.begin();
			ofBackground(0);
			pThis->m_animations[0]->VM_draw(pThis->m_fboAnimation1.getWidth(), pThis->m_fboAnimation1.getHeight());
			pThis->m_fboAnimation1.end();

			pThis->m_fboAnimation2.begin();
			ofBackground(0);
			pThis->m_animations[1]->VM_draw(pThis->m_fboAnimation2.getWidth(), pThis->m_fboAnimation2.getHeight());
			pThis->m_fboAnimation2.end();

			fboFinal.begin();

			pThis->m_shader.begin();
			pThis->m_shader.setUniformTexture("tex1", pThis->m_fboAnimation1.getTextureReference(), 1);
			pThis->m_shader.setUniformTexture("tex2", pThis->m_fboAnimation2.getTextureReference(), 2);
			pThis->m_fboAnimation1.draw(0,0);
			pThis->m_shader.end();

			fboFinal.end();

		}
		else
		{
			fboFinal.begin();
			ofBackground(0);
			fboFinal.end();
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceOffscreenChanged(ofVec2f& dimensions)
{
	allocateRenderBuffers();
}

//--------------------------------------------------------------
void AnimationComposition::allocateRenderBuffers()
{
	m_bAllocateRenderBuffers = true;
}

//--------------------------------------------------------------
void AnimationComposition::handleMidiMessages()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it){
	 	(*it)->handleMidiMessages();
	}
}

//--------------------------------------------------------------
void AnimationComposition::newMidiMessage(ofxMidiMessage& eventArgs)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it){
	 	(*it)->newMidiMessage(eventArgs);
	}
}

//--------------------------------------------------------------
void AnimationComposition::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);
	
	
	string name = e.getName();
	if (name == "renderNormal")
	{
		setRenderNormal(e.getToggle()->getValue());
	}
	else
	{
		if (e.getKind() == OFX_UI_WIDGET_TOGGLE)
		{
			if (name == "multiply" || name == "difference"){
				setBlending(name);
			}
			else if (isNameComposition(name))
			{
				if (e.getToggle()->getValue()>0)
				{
					OFAPPLOG->println("- setting composition "+name);
					m_compositionWanted = name;
				}
			}
		}
	}
}


//--------------------------------------------------------------
bool AnimationComposition::isNameComposition(string name)
{
	vector<AnimationCompoConfiguration*>::iterator it = m_compositions.begin() ;
	for ( ; it!=m_compositions.end(); ++it)
	{
		if ( (*it)->m_name == name)
			return true;
	}
	return false;
}




