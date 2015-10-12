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
	m_bAllocateRenderBuffers = false;
	m_bRenderNormal = false;
	m_bRenderNormalWanted = false;
	
	mp_radioCompositions=0;
	mp_lblCompositionInfo = 0;

	m_isLoadingConfiguration = false;
	mp_compositionCurrent = 0;
	
	ofFile fBlendFunctions("Shaders/blendFunctions.glsl");
	m_strFragBlendFunctions = fBlendFunctions.readToBuffer().getText();
	
	m_blendings.push_back("Add");
	m_blendings.push_back("Average");
	m_blendings.push_back("ColorDodge");
	m_blendings.push_back("Difference");
	m_blendings.push_back("Exclusion");
	m_blendings.push_back("Glow");
	m_blendings.push_back("HardLight");
	m_blendings.push_back("Luminosity");
	m_blendings.push_back("Multiply");
	m_blendings.push_back("Negation");
	m_blendings.push_back("Overlay");
	m_blendings.push_back("Screen");
	m_blendings.push_back("SoftLight");
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
		mp_UIcanvas->addWidgetDown(new ofxUILabel("Compositions", OFX_UI_FONT_MEDIUM));
    	mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));
		mp_lblCompositionInfo = new ofxUILabel("lblCompositionInfo", OFX_UI_FONT_SMALL);
    	mp_UIcanvas->addWidgetDown(mp_lblCompositionInfo);
	

		vector<string> compositionNames;
		for (int i=0; i< m_compositions.size(); i++)
			compositionNames.push_back(m_compositions[i]->m_name);

/*		mp_radioCompositions = new ofxUIRadio("radioCompositions", compositionNames, OFX_UI_ORIENTATION_VERTICAL, 16, 16);
		vector<ofxUIToggle *> toggles = mp_radioCompositions->getToggles();
		for (int i=0;i<toggles.size();i++) toggles[i]->setValue(false);
		mp_UIcanvas->addWidgetDown(mp_radioCompositions);
*/

		ofxUIDropDownList* pDropCompo = new ofxUIDropDownList("Compositions", compositionNames, 330,0,0,OFX_UI_FONT_SMALL);
		pDropCompo->setAutoClose(true);
		pDropCompo->setAllowMultiple(false);
		mp_UIcanvas->addWidgetDown( pDropCompo );
		pDropCompo->setShowCurrentSelected(true);
	
		
		mp_UIcanvas->addWidgetDown(new ofxUILabel("Blending", OFX_UI_FONT_MEDIUM));
    	mp_UIcanvas->addWidgetDown(new ofxUISpacer(330, 2));

		mp_UIcanvas->addToggle("renderNormal", &m_bRenderNormal);

		ofxUIDropDownList* pDropBlending = new ofxUIDropDownList("Blending", m_blendings, 330,0,0,OFX_UI_FONT_SMALL);
		pDropBlending->setAutoClose(true);
		pDropBlending->setAllowMultiple(false);
		mp_UIcanvas->addWidgetDown( pDropBlending );
		pDropBlending->setShowCurrentSelected(true);

		ofxUILabelButton* pBtnReload = new ofxUILabelButton("reloadShader", false, 330, 16, 0,0, OFX_UI_FONT_SMALL);
		mp_UIcanvas->addWidgetDown( pBtnReload );
		pBtnReload->getLabelWidget()->setLabel("Reload shader");

	}
}

//--------------------------------------------------------------
void AnimationComposition::updateUI()
{
	if (mp_lblCompositionInfo)
	{
		if (mp_compositionCurrent)
		{
			mp_lblCompositionInfo->setLabel(mp_compositionCurrent->m_animationName1+" ('"+mp_compositionCurrent->m_animationConfig1+"')"+" + "+mp_compositionCurrent->m_animationName2+" ('"+mp_compositionCurrent->m_animationConfig2+"')");
		}
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
	m_isLoadingConfiguration = true;
	OFAPPLOG->begin("AnimationComposition::loadConfiguration('"+filename+"')");
	Animation::loadConfiguration(filename);
	OFAPPLOG->end();
	m_isLoadingConfiguration = false;
   setRenderNormal(m_bRenderNormalWanted);
}


//--------------------------------------------------------------
void AnimationComposition::setRenderNormal(bool is)
{
	//if (is == m_bRenderNormal) return;
	
	m_bRenderNormal = is;

/*
	for (int i=0;i<m_animations.size(); i++){
		m_animations[i]->setDrawBackground(!is);
	}

*/

	Surface* pSurface = GLOBALS->getSurfaceMain();
	if (pSurface)
		pSurface->setRenderOffscreenCb(0,0);

	if (m_bRenderNormal == false)
	{
		allocateRenderBuffers();
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
	m_nameBlending = name;
	m_shader.setupShaderFromFile(GL_VERTEX_SHADER, "Shaders/animCompo.vert");
	m_shader.setupShaderFromSource(GL_FRAGMENT_SHADER, getShaderSourceWithBlendFunctions("Shaders/animCompo"+name+".frag"));
	m_shader.bindDefaults();
	m_shader.linkProgram();
}

//--------------------------------------------------------------
void AnimationComposition::setComposition( string name )
{
	OFAPPLOG->begin("AnimationComposition::setComposition("+name+")");
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
			if (m_animations[0]){
				m_animations[0]->VM_exit();
				m_animations[0]->setDrawBackground();
			}
			if (m_animations[1]){
				m_animations[1]->VM_exit();
				m_animations[1]->setDrawBackground();
			}
		}
		m_animations.clear();


		Animation* pAnim1 = getAnimation(pComposition->m_animationName1);
		Animation* pAnim2 = getAnimation(pComposition->m_animationName2);

		if (pAnim1 && pAnim2)
		{
			OFAPPLOG->println("- anim1 = "+pAnim1->m_name);
			OFAPPLOG->println("- anim2 = "+pAnim2->m_name);

			m_animations.push_back( pAnim1 );
			m_animations.push_back( pAnim2 );
			
			
			pAnim1->loadConfiguration(pComposition->m_animationConfig1);
			pAnim2->loadConfiguration(pComposition->m_animationConfig2);
			
			pAnim1->VM_enter();
			pAnim2->VM_enter();

			pAnim1->setDrawBackground(true);
			pAnim2->setDrawBackground(false);
			

		}
	}
	mp_compositionCurrent = pComposition;
	
	updateUI();
	
	OFAPPLOG->end();
	
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
m_isLoadingConfiguration = true;
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->loadProperties( id+"_"+(*it)->m_name );

	Animation::loadProperties(id);
m_isLoadingConfiguration = false;
	
}

//--------------------------------------------------------------
void AnimationComposition::VM_enter()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_enter();

	
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
void AnimationComposition::VM_update(float dt)
{

	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_update(dt);
}

//--------------------------------------------------------------
void AnimationComposition::VM_drawBefore(float w, float h)
{
		//setRenderNormal(m_bRenderNormalWanted);

	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_drawBefore(w,h);

	if (m_bAllocateRenderBuffers && w>0 && h>0 && !m_fboAnimation1.isAllocated() && !m_fboAnimation2.isAllocated())
	{
		m_bAllocateRenderBuffers=false;
		Surface* pSurface = GLOBALS->getSurfaceMain();
		if (pSurface)
		{
			float w = pSurface->getOffscreen().getWidth();
			float h = pSurface->getOffscreen().getHeight();
			int nbSamples = pSurface->m_fboNbSamples;
			
			ofFbo::Settings settings;
			settings.width = (int)w;
			settings.height = (int)h;
			settings.internalformat = GL_RGB;
			settings.useDepth = false;
			settings.useStencil = false;
		
			m_fboAnimation1.allocate(settings);
			m_fboAnimation2.allocate(settings);
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::VM_draw(float w, float h)
{
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
void AnimationComposition::onNewPacket(DevicePacket* pDevice, string deviceId, float x, float y)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
		(*it)->onNewPacket(pDevice,deviceId,x,y);
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceRenderOffscreen(Surface* pSurface, AnimationComposition* pThis)
{
   ofFbo& fboFinal = pSurface->getOffscreen();

   //ofLog() << pThis->m_animations.size();

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
	   ofBackground(0);

	   pThis->m_shader.begin();
	   pThis->m_shader.setUniformTexture("tex1", pThis->m_fboAnimation1.getTextureReference(), 1);
	   pThis->m_shader.setUniformTexture("tex2", pThis->m_fboAnimation2.getTextureReference(), 2);
	   pThis->m_fboAnimation1.draw(0,0); // just to draw a quad on screen
	   pThis->m_shader.end();

	   fboFinal.end();

   }
   else
   {
	   fboFinal.begin();
	   ofBackground(255,0,0);
	   fboFinal.end();
   }
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceOffscreenChanged(ofVec2f& dimensions)
{
	//allocateRenderBuffers();
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
		if (!m_isLoadingConfiguration)
			setRenderNormal(e.getToggle()->getValue());
		else
			m_bRenderNormalWanted = e.getToggle()->getValue();
	}
	else
	if (name == "reloadShader")
	{
		if (e.getButton()->getValue()>0)
		{
			setBlending(m_nameBlending);
		}
	}
	else
	{
		// Not called on loading, only OFX_UI_WIDGET_LABELTOGGLE called
		if (e.getKind() == OFX_UI_WIDGET_DROPDOWNLIST)
		{
 	      	ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
    	    vector<ofxUIWidget *> &selected = ddlist->getSelected();
			vector<int>& selectedIndices = ddlist->getSelectedIndeces();
        	if (selectedIndices.size()==1)
			{
				if (name == "Blending")
					setBlending( selected[0]->getName() );
				else
				if (name == "Compositions")
					setComposition( selected[0]->getName() );
			}
		}
		else if (e.getKind() == OFX_UI_WIDGET_LABELTOGGLE)
		{
			//OFAPPLOG->println("- OFX_UI_WIDGET_LABELTOGGLE for "+name);

			if (isNameBlending(name))
			{
				if (e.getToggle()->getValue()>0)
				{
					setBlending(name);
					OFAPPLOG->println("- setting blending "+name);
				}
			}
			else if (isNameComposition(name))
			{
				if (e.getToggle()->getValue()>0)
				{
					OFAPPLOG->println("- setting composition "+name);
					setComposition(name);
				}
			}

		
		}
		else if (e.getKind() == OFX_UI_WIDGET_TOGGLE)
		{
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

//--------------------------------------------------------------
string AnimationComposition::getShaderSourceWithBlendFunctions(string filename)
{
	ofFile fileFrag(filename);
	return "#version 150\n\n"+m_strFragBlendFunctions+"\n\n"+fileFrag.readToBuffer().getText();
}

//--------------------------------------------------------------
bool AnimationComposition::isNameBlending(string name)
{
	vector<string>::iterator it = m_blendings.begin();
	for ( ; it!=m_blendings.end(); ++it)
	{
		if ( (*it) == name)
			return true;
	}
	return false;
}






