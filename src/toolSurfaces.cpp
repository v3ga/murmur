//
//  toolSurfaces.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolSurfaces.h"
#include "toolDevices.h"
#include "toolScene.h"
#include "globals.h"
#include "testApp.h"
#include "ofxHomographyHelper.h"
#include "ofxMultiGLFWWindow.h"


//--------------------------------------------------------------
toolSurfaces::toolSurfaces(toolManager* parent, Surface* surface) : tool("Surfaces", parent)
{
	mp_lblSurfaceActivity 	= 0;
	mp_surfaceMain			= surface;
	mp_mask					= 0;
	mp_maskUI 				= 0;
	m_isDrawHandles			= false;
	mp_windowCurrent		= 0;
	m_alpha					= 0.0f;
	m_quadWarpingHandleShifting = 1.0f;
	m_devicePointShifting	= 1.0f;
	
	setView					(VIEW_NORMAL);
	
	m_properties.add( new classProperty_float("alpha", 0.0f,1.0f, &m_alpha) );

    setMidiName("tool Surfaces");
}

//--------------------------------------------------------------
void toolSurfaces::setup()
{
    int x = 0;   // center on screen.
    int y = 0;   // center on screen.
    int w = ofGetWidth();
    int h = ofGetHeight();
	
	m_quadWarping.setup(&DATA->m_font);
}

//--------------------------------------------------------------
void toolSurfaces::saveData()
{
	tool::saveData();
	m_quadWarping.save(getQuadWarpingPathFile());
}

//--------------------------------------------------------------
void toolSurfaces::loadData()
{
	tool::loadData();
	m_quadWarping.load(getQuadWarpingPathFile());
}

//--------------------------------------------------------------
string toolSurfaces::getQuadWarpingPathFile()
{
   return mp_toolManager->m_relPathData + "/" + "Surfaces_quadWarping.xml";
}

//--------------------------------------------------------------
void toolSurfaces::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float widthDefault = 320;
		float dim = 16;
	
	    mp_canvas->addWidgetDown	( new ofxUILabel("Surfaces", OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown	( new ofxUISpacer(widthDefault, 2));


		ofxUITextInput* pTeWSurface = new ofxUITextInput("wSurface", "4" , 40, dim,0,0,fontType);
		pTeWSurface->setAutoClear(false);
		pTeWSurface->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);

		ofxUITextInput* pTeHSurface = new ofxUITextInput("hSurface", "3" , 40, dim,0,0,fontType);
		pTeHSurface->setAutoClear(false);
		pTeHSurface->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);


    	mp_canvas->addWidgetDown	( new ofxUILabel("Dimensions", fontType) );
    	mp_canvas->addWidgetRight 	( pTeWSurface );
    	mp_canvas->addWidgetRight	( new ofxUILabel("x", fontType) );
    	mp_canvas->addWidgetRight	( pTeHSurface );
		
		

		ofxUITextInput* pTeWFbo = new ofxUITextInput("wFboSurface", "1000" , 40, dim,0,0,fontType);
		pTeWFbo->setAutoClear(false);
		pTeWFbo->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);

		ofxUITextInput* pTeHFbo = new ofxUITextInput("hFboSurface", "1000" , 40, dim,0,0,fontType);
		pTeHFbo->setAutoClear(false);
		pTeHFbo->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);


    	mp_canvas->addWidgetDown	( new ofxUILabel("FBO", fontType) );
    	mp_canvas->addWidgetRight 	( pTeWFbo );
    	mp_canvas->addWidgetRight	( new ofxUILabel("x", fontType) );
		mp_canvas->addWidgetRight	( pTeHFbo );
		mp_canvas->addWidgetDown	( new ofxUIIntSlider("quality", 0 , ofFbo::maxSamples(), 0, widthDefault, dim));

    	mp_canvas->addWidgetDown	(new ofxUISlider("alpha", 0.0f, 1.0f, &m_alpha, widthDefault, dim ));

    	mp_canvas->addWidgetDown	(new ofxUIToggle("target", false, dim, dim));
    	mp_canvas->addWidgetRight	(new ofxUISlider("target line w", 1.0f, 8.0f, 2.0f, 100, dim ));

	    mp_canvas->addWidgetDown	( new ofxUIToggle("enable mask", false, dim, dim) );
		mp_canvas->addImage			( "mask", 0, widthDefault, 50);
		ofxUITextInput* lblPathMask = new ofxUITextInput("pathMask", "", widthDefault);
    	mp_canvas->addWidgetDown	( lblPathMask );
		lblPathMask->setVisible(false);

	    mp_canvas->addWidgetDown	( new ofxUILabel("Mapping", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown	( new ofxUISpacer(widthDefault, 2));

	    mp_canvas->addWidgetDown	(new ofxUIToggle("syphon", false, dim, dim));
	    mp_canvas->addWidgetDown	(new ofxUIToggle("quad warping", false, dim, dim));
	    mp_canvas->addWidgetDown	(new ofxUIToggle("draw quad warping", false, dim, dim));
	    mp_canvas->addWidgetDown	(new ofxUIButton("reset quad warping", false, dim*4, dim));

	    mp_canvas->addWidgetDown	(new ofxUISlider("quad warping handle shifting", 1.0f, 10.0f, &m_quadWarpingHandleShifting, widthDefault, dim ));
	    mp_canvas->addWidgetDown	(new ofxUISlider("device point shifting", 1.0f, 10.0f, &m_devicePointShifting, widthDefault, dim ));
	
		mp_canvas->autoSizeToFitWidgets();
	
	}
}

//--------------------------------------------------------------
Surface* toolSurfaces::getSurfaceForDevice(Device* pDevice)
{
    // TODO : iterate through a liste of surfaces to find the surface that has the device attached to it
    return mp_surfaceMain;
}

//--------------------------------------------------------------
Surface* toolSurfaces::getSurfaceForDeviceCurrent()
{
	toolDevices*	pToolDevices = (toolDevices*) mp_toolManager->getTool("Devices");
	if (pToolDevices && pToolDevices->mp_deviceManager)
	    return getSurfaceForDevice( pToolDevices->mp_deviceManager->getDeviceCurrent() );
	return 0;
}

//--------------------------------------------------------------
void toolSurfaces::onSurfaceModified(Surface* pSurface)
{
	toolScene* pToolScene = (toolScene*)mp_toolManager->getTool("Scene");

	if(pToolScene && pToolScene->getScene() && pSurface)
    {
   		vector<Device*>& listDevices = pSurface->getListDevices();
   		vector<Device*>::iterator it = listDevices.begin();
   		for ( ; it != listDevices.end() ; ++it)
   		{
			pToolScene->getScene()->updatePositionNodeSurface(*it, pSurface);
		
/*			DeviceNode* pDeviceNode = pToolScene->getScene()->getDeviceNode(*it);
            SurfaceNode* pSurfaceNode = pToolScene->getScene()->getSurfaceNode(pSurface);
			if (pDeviceNode && pSurfaceNode)
            	pDeviceNode->setPositionNodeSurface( pSurfaceNode->getGlobalPositionDevicePointSurface(pDeviceNode->getDevice()) );
*/
	   }
	}
}

//--------------------------------------------------------------
void toolSurfaces::update()
{
	handleMidiMessages();

    if (mp_surfaceMain)
	{
		mp_surfaceMain->setLayerAlpha(m_alpha);
		mp_surfaceMain->setMask(mp_mask); // TEMP here?
        mp_surfaceMain->renderOffscreen(GLOBALS->mp_app->isShowDevicePointSurfaces);
		mp_surfaceMain->publishSyphon();
    }
	
	m_quadWarping.update();
	
}


//--------------------------------------------------------------
void toolSurfaces::computeRectSurfaceOff()
{
	if (mp_surfaceMain)
	{
		ofFbo& offSurface = mp_surfaceMain->getOffscreen();
		if (mp_windowCurrent)
		{

			int width, height;
			glfwGetWindowSize(mp_windowCurrent, &width, &height);
			m_rectScreen.set(0,0,width, height);
		}
		else
			m_rectScreen.set(0,0,ofGetWidth(),ofGetHeight());

		m_rectSurfaceOff.set(0,0,offSurface.getWidth(),offSurface.getHeight());
		m_rectSurfaceOff.scaleTo(m_rectScreen);
		m_rectSurfaceOff.alignTo(m_rectScreen);
	}
}

//--------------------------------------------------------------
void toolSurfaces::draw()
{
	computeRectSurfaceOff();

	if (m_view == VIEW_NORMAL)
	{
        if (mp_surfaceMain)
		{

			ofClear(70,70,70);
            ofSetColor(255,255,255,255);
            mp_surfaceMain->getOffscreen().draw(m_rectSurfaceOff.getX(),m_rectSurfaceOff.getY(),m_rectSurfaceOff.getWidth(),m_rectSurfaceOff.getHeight());
            if (GLOBALS->mp_app->isShowDevicePointSurfaces)
			{
                mp_surfaceMain->drawDevicePointSurface(m_rectSurfaceOff);
            }
        }
	}
	else if (m_view == VIEW_QUADWARPING)
	{
		if (mp_surfaceMain)
		{
			ofClear(0);
            ofSetColor(255,255,255,255);
			ofPushMatrix();
				ofMultMatrix( m_quadWarping.findTransformMatrix(m_rectSurfaceOff) );
				mp_surfaceMain->getOffscreen().draw(0,0,m_rectSurfaceOff.getWidth(),m_rectSurfaceOff.getHeight());
			ofPopMatrix();
		
           if (GLOBALS->mp_app->isShowDevicePointSurfaces)
		   {
				ofMatrix4x4 m = m_quadWarping.getTransformMatrix(m_rectSurfaceOff);
				mp_surfaceMain->drawDevicePointSurface(m_rectSurfaceOff, &m);
			}
		
			if (m_isDrawHandles)
		   		m_quadWarping.draw();
		}
	}
}


//--------------------------------------------------------------
void toolSurfaces::setView(int which)
{
	m_view = which;
	if (m_view == VIEW_NORMAL){
		m_quadWarping.disableMouseEvents();
	}
	else if (m_view == VIEW_QUADWARPING){
		m_quadWarping.unselectHandle();
		m_quadWarping.enableMouseEvents();
	}
	
}


//--------------------------------------------------------------
void toolSurfaces::handleEvents(ofxUIEventArgs& e)
{
    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
	if (pSurfaceCurrent == 0) return;

	toolScene* pToolScene = (toolScene*)mp_toolManager->getTool("Scene");
	if (pToolScene == 0) return;

    string name = e.widget->getName();
	
	if (name == "wSurface")
    {
		SurfaceNode * pSurfaceNodeCurrent = pToolScene->getSurfaceNode(pSurfaceCurrent);
		float wSurface = atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
		pSurfaceNodeCurrent->setWidth(wSurface);
		pSurfaceNodeCurrent->setPosition(-0.5f*wSurface, pSurfaceNodeCurrent->getPosition().y, pSurfaceNodeCurrent->getPosition().z);
  
		onSurfaceModified(pSurfaceCurrent);
	}
    else if (name == "hSurface")
    {
		SurfaceNode * pSurfaceNodeCurrent = pToolScene->getSurfaceNode(pSurfaceCurrent);
		pSurfaceNodeCurrent->setHeight( atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() ) );
		
		onSurfaceModified(pSurfaceCurrent);
	}
    else if (name == "wFboSurface")
    {
		int wPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );;
		pSurfaceCurrent->setDimensions(wPixels,pSurfaceCurrent->getHeightPixels(),pSurfaceCurrent->getQuality());
	}
    else if (name == "hFboSurface")
    {
		int hPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
		pSurfaceCurrent->setDimensions(pSurfaceCurrent->getWidthPixels(),hPixels,pSurfaceCurrent->getQuality());
	}
	else if (name == "quality")
	{
		int fboQuality = pSurfaceCurrent->getQuality();
		int quality = ((ofxUIIntSlider*)e.widget)->getScaledValue();
		if (quality != fboQuality)
			pSurfaceCurrent->setDimensions(pSurfaceCurrent->getWidthPixels(),pSurfaceCurrent->getHeightPixels(), quality);
	}
    else if (name == "syphon")
	{
		pSurfaceCurrent->setPublishSyphon( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "target")
	{
		pSurfaceCurrent->setRenderTarget( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "target line w")
	{
		pSurfaceCurrent->setRenderTargetLineWidth( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else if (name == "enable mask")
	{
		pSurfaceCurrent->setDrawMask( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "pathMask")
	{
		string value = ((ofxUITextInput *) e.widget)->getTextString();
		if ( loadMask( value ) )
			updateMaskUI( value );
	}
	else if (name == "quad warping")
	{
		bool value = ((ofxUIToggle *) e.widget)->getValue();
		if (value){
			setView(VIEW_QUADWARPING);
		}else{
			setView(VIEW_NORMAL);
		}
	}
	else if (name == "draw quad warping")
	{
		m_isDrawHandles = ((ofxUIToggle *) e.widget)->getValue();
		if (m_isDrawHandles)
			m_quadWarping.enableMouseEvents();
		else
			m_quadWarping.disableMouseEvents();
	}
	else if (name == "reset quad warping")
	{
	 	if (e.getButton()->getValue())
			m_quadWarping.resetHandles();
	}
}

//--------------------------------------------------------------
void toolSurfaces::mousePressed(int x, int y, int button)
{
	toolDevices*	pToolDevices 	= (toolDevices*) mp_toolManager->getTool("Devices");
	toolScene* 		pToolScene 		= (toolScene*)mp_toolManager->getTool("Scene");
	
	if (pToolDevices == 0 || pToolDevices->mp_deviceManager == 0 || pToolScene == 0) return;

	Device*  pDeviceCurrent 	= pToolDevices->mp_deviceManager->getDeviceCurrent();
	Surface* pSurfaceCurrent  	= this->getSurfaceForDevice(pDeviceCurrent);


	bool bUpdateScene = false;

	if (m_view == VIEW_NORMAL)
	{
		if (GLOBALS->mp_app->isShowDevicePointSurfaces)
		{

		   if (pDeviceCurrent && pSurfaceCurrent)
		   {
				computeRectSurfaceOff();
		
		   		float dx = x - m_rectSurfaceOff.getX();
		   		float dy = y - m_rectSurfaceOff.getY();
		   
		   		float xNorm = ofClamp(dx / m_rectSurfaceOff.getWidth(),0.0f,1.0f);
		   		float yNorm = ofClamp(dy / m_rectSurfaceOff.getHeight(),0.0f,1.0f);
	   
		   		pDeviceCurrent->setPointSurface(xNorm, yNorm);
			
				bUpdateScene = true;
		   }
	   }
	}
	else if (m_view == VIEW_QUADWARPING)
	{
		bool isOverHandle = false;
		for (int i=0;i<4;i++)
		{
			if (m_quadWarping.m_handles[i].isMouseOver()){
				isOverHandle = true;
				break;
			}
		}
		if (isOverHandle == false)
		{
			m_quadWarping.unselectHandle();
			
//			m_ptClickWarp.set(x,y);
//			m_ptClick = m_quadWarping.getPointInSquareNormalized( m_ptClickWarp );
		}
	}

	if (bUpdateScene && pToolScene->getScene())
	{
		pToolScene->getScene()->updatePositionNodeSurface( pDeviceCurrent, pSurfaceCurrent );
	}
}

//--------------------------------------------------------------
bool toolSurfaces::keyPressed(int key)
{
	if (m_view == VIEW_QUADWARPING)
	{
		if (m_quadWarping.hasSelectedHandle())
		{
			if (key == OF_KEY_LEFT) 			{m_quadWarping.moveSelectedHandle( ofVec2f(-m_quadWarpingHandleShifting, 0.0f) );	return true;}
			if (key == OF_KEY_RIGHT)			{m_quadWarping.moveSelectedHandle( ofVec2f( m_quadWarpingHandleShifting, 0.0f) );	return true;}
			if (key == OF_KEY_UP)				{m_quadWarping.moveSelectedHandle( ofVec2f( 0.0f, -m_quadWarpingHandleShifting) );	return true;}
			if (key == OF_KEY_DOWN)				{m_quadWarping.moveSelectedHandle( ofVec2f( 0.0f, m_quadWarpingHandleShifting) );	return true;}
		}
		else
		{
			toolDevices*	pToolDevices 	= (toolDevices*) 	mp_toolManager->getTool("Devices");
			toolScene* 		pToolScene 		= (toolScene*)		mp_toolManager->getTool("Scene");

			if (pToolDevices && pToolDevices->mp_deviceManager && pToolScene)
			{
				Device*  pDeviceCurrent 	= pToolDevices->mp_deviceManager->getDeviceCurrent();
				Surface* pSurfaceCurrent  	= this->getSurfaceForDevice(pDeviceCurrent);

				if (pDeviceCurrent && pSurfaceCurrent)
				{
					ofVec2f delta;
					if (key == OF_KEY_LEFT) 			{ delta.set(-m_devicePointShifting, 0.0f); }
					if (key == OF_KEY_RIGHT)			{ delta.set( m_devicePointShifting, 0.0f); }
					if (key == OF_KEY_UP)				{ delta.set( 0.0f, -m_devicePointShifting); }
					if (key == OF_KEY_DOWN)				{ delta.set( 0.0f, m_devicePointShifting); }

					pDeviceCurrent->m_pointSurface += ofVec2f( delta.x / pSurfaceCurrent->getOffscreen().getWidth(), delta.y / pSurfaceCurrent->getOffscreen().getHeight() );
					

					pToolScene->getScene()->updatePositionNodeSurface( pDeviceCurrent, pSurfaceCurrent );
					return true;
				}
			
			}
		
		}
	}
	
	return false;
}

//--------------------------------------------------------------
void toolSurfaces::windowResized(int w, int h)
{
	m_quadWarping.windowResized(w,h);
}

//--------------------------------------------------------------
void toolSurfaces::windowResized(int wold, int hold, int w, int h)
{
	m_quadWarping.windowResized(wold,hold,w,h);
}



//--------------------------------------------------------------
void toolSurfaces::dragEvent(ofDragInfo dragInfo)
{
	int nbFiles = dragInfo.files.size();
	if( nbFiles > 0 )
	{

		ofFile fileSrc( dragInfo.files[0] );
		if (fileSrc.exists())
		{
			string fileDstPath =  "Images/Surfaces/" + fileSrc.getFileName();

			bool reload = false;
			if (ofToDataPath( fileDstPath, true) == dragInfo.files[0])
			{
				reload = true;
			}
			else
			{
				if ( ofFile::copyFromTo(fileSrc.getAbsolutePath(), fileDstPath, true, true) )
				{
					reload = true;
				}
			}

			if ( reload )
			{
				if ( loadMask(fileDstPath) )
				{
					updateMaskUI(fileDstPath);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void toolSurfaces::updateMaskUI(string path)
{
	if (mp_canvas == 0) return;
	
	ofxUIImage* pMaskUI = (ofxUIImage*) mp_canvas->getWidget("mask");
	ofxUITextInput* pMaskPathUI = (ofxUITextInput*) mp_canvas->getWidget("pathMask");
	if (mp_mask && pMaskUI && pMaskPathUI)
	{
		pMaskPathUI->setTextString(path);

		if (mp_maskUI == 0)
		{
			mp_maskUI = new ofImage(mp_mask->getPixelsRef());
			mp_maskUI->resize( (int)pMaskUI->getRect()->getWidth(), (int)pMaskUI->getRect()->getHeight() );
			pMaskUI->setImage(mp_maskUI);
		}

	}
}


//--------------------------------------------------------------
bool toolSurfaces::loadMask(string path)
{
	mp_mask = new ofImage();
	if (mp_mask->loadImage(path))
	{
		delete mp_maskUI;
		mp_maskUI = 0;
		return true;
	}
	else{
		delete mp_mask;
		mp_mask = 0;
		
	}
	
	return false;
}



