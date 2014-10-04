//
//  quadWarping.cpp
//  murmur
//
//  Created by Julien on 01/07/2014.
//
//

#include "quadWarping.h"
#include "ofxHomographyHelper.h"
#include "ofxXmlSettings.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
quadWarpingHandle::quadWarpingHandle() : ofxMSAInteractiveObject()
{
	mp_parent = 0;
	mp_font = 0;
	m_isSelected = false;
}

//--------------------------------------------------------------
bool quadWarpingHandle::hitTest(int tx, int ty)
{
	return ((tx > x-width/2) && (tx < x + width/2) && (ty > y-height/2) && (ty < y + height/2));
}

//--------------------------------------------------------------
void quadWarpingHandle::setup(quadWarping* pParent, ofTrueTypeFont* pFont)
{
	mp_parent = pParent;
	mp_font = pFont;

	setSize(10, 10);
	disableAllEvents();
	enableMouseEvents();
}

//--------------------------------------------------------------
void quadWarpingHandle::update()
{
	
}

//--------------------------------------------------------------
void quadWarpingHandle::draw(int index)
{
	posNormalized.set( x/ofGetWidth(),  x/ofGetHeight());

	if (m_isSelected)
		ofSetColor(200,0,0);
	else
		ofSetColor(255);
	ofRect(x-width/2,y-height/2,width,height);
	if (mp_font){
		mp_font->drawString(ofToString(index), x+width/2+5,y);
	}
}

//--------------------------------------------------------------
void quadWarpingHandle::onPress(int x, int y, int button)
{
	dragDelta.set(x-this->x, y-this->y);
	if (mp_parent){
		mp_parent->selectHandle(this);
		m_isSelected = true;
	}
}

//--------------------------------------------------------------
void quadWarpingHandle::onDragOver(int x, int y, int button)
{
	setPos(x-dragDelta.x, y-dragDelta.y);
}

//--------------------------------------------------------------
void quadWarpingHandle::onDragOutside(int x, int y, int button)
{
	setPos(x-dragDelta.x, y-dragDelta.y);
}

//--------------------------------------------------------------
void quadWarpingHandle::onRelease(int x, int y, int button)
{

}

//--------------------------------------------------------------
void quadWarpingHandle::onReleaseOutside(int x, int y, int button)
{

}

//--------------------------------------------------------------
quadWarping::quadWarping()
{
	mp_handleSelected = 0;
}

//--------------------------------------------------------------
void quadWarping::disableMouseEvents()
{
	for (int i=0;i<4;i++)
	{
		m_handles[i].disableMouseEvents();
	}
}

//--------------------------------------------------------------
void quadWarping::enableMouseEvents()
{
	for (int i=0;i<4;i++)
	{
		m_handles[i].enableMouseEvents();
	}
}

//--------------------------------------------------------------
void quadWarping::selectHandle(quadWarpingHandle* p)
{
	for (int i=0;i<4;i++){
		m_handles[i].m_isSelected = false;
	}
	mp_handleSelected = p;
}


//--------------------------------------------------------------
void quadWarping::setup(ofTrueTypeFont* pFont)
{
	for (int i=0;i<4;i++)
	{
		m_handles[i].setup(this,pFont);
	}
}

//--------------------------------------------------------------
void quadWarping::update()
{
	for (int i=0;i<4;i++)
	{
		m_handles[i].update();
	}
}


//--------------------------------------------------------------
void quadWarping::draw()
{
	ofPushStyle();
	ofSetColor(255);
	for (int i=0;i<4;i++)
	{
		ofLine( m_handles[i].x,m_handles[i].y,m_handles[(i+1)%4].x,m_handles[(i+1)%4].y  );
	}

	for (int i=0;i<4;i++)
	{
		m_handles[i].draw(i);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void quadWarping::save(string pathFile)
{
	OFAPPLOG->begin("quadWarping::save('"+pathFile+"')");
    ofxXmlSettings settings;

    settings.addTag("points");
    settings.pushTag("points");
		for (int i=0;i<4;i++)
		{
		    settings.addTag("p");
			settings.setAttribute("p", "id", 	i, i);
			settings.setAttribute("p", "x", 	m_handles[i].x / ofGetWidth(), 	i);
			settings.setAttribute("p", "y", 	m_handles[i].y / ofGetHeight(), i);
		}
    settings.popTag();
	
	// ofLog() << "saving " << pathFile;
	settings.save(pathFile);
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void quadWarping::load(string pathFile)
{
	OFAPPLOG->begin("quadWarping::load('"+pathFile+"')");

    ofxXmlSettings settings;
	
	if ( settings.loadFile(pathFile.c_str()) )
    {
		settings.pushTag("points");
			for (int i=0;i<4;i++)
			{
				float xDefault = 0.0f;
				float yDefault = 0.0f;
				if (i==1){ xDefault = 1.0f; }
				if (i==2){ xDefault = 1.0f; yDefault = 1.0f;}
				if (i==3){ yDefault = 1.0f;}

				// settings.pushTag("p",i);
				float x = ofClamp(settings.getAttribute("p","x",xDefault,i), 0.0f, 1.0f);
				float y = ofClamp(settings.getAttribute("p","y",yDefault,i), 0.0f, 1.0f);
				
				OFAPPLOG->println(" ["+ofToString(i)+"] x="+ofToString(x)+";y="+ofToString(y));

				m_handles[i].setPos(
									x * ofGetWidth(),
									y * ofGetHeight()
									);
				// settings.popTag();
			}
		settings.popTag();
	}
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void quadWarping::moveSelectedHandle(ofVec2f delta)
{
	if (mp_handleSelected)
	{
		mp_handleSelected->setPos(mp_handleSelected->x + delta.x, mp_handleSelected->y + delta.y);
	}
}

//--------------------------------------------------------------
void quadWarping::unselectHandle()
{
	if (mp_handleSelected)
	{
		mp_handleSelected->m_isSelected = false;
		mp_handleSelected = 0;
	}
}


//--------------------------------------------------------------
void quadWarping::windowResized(int wold, int hold, int w, int h)
{
	for (int i=0;i<4;i++)
	{
		ofVec2f posNorm( m_handles[i].x/(float)wold, m_handles[i].y/(float)hold );
		m_handles[i].setPos( posNorm.x*(float)w, posNorm.y*(float)h );
	}
}

//--------------------------------------------------------------
void quadWarping::windowResized(int w, int h)
{
	ofLog() << w << "/" << h;
	for (int i=0;i<4;i++)
	{
		
	}
}

//--------------------------------------------------------------
float* quadWarping::findTransformMatrix(const ofRectangle& rect)
{
	float src[4][2];
	src[0][0] = 0;
	src[0][1] = 0;
	src[1][0] = 0 + rect.width;
	src[1][1] = 0;
	src[2][0] = 0 + rect.width;
	src[2][1] = 0 + rect.height;
	src[3][0] = 0;
	src[3][1] = 0 + rect.height;


	float dst[4][2];
	dst[0][0] = m_handles[0].x;
	dst[0][1] = m_handles[0].y;
	dst[1][0] = m_handles[1].x;
	dst[1][1] = m_handles[1].y;
	dst[2][0] = m_handles[2].x;
	dst[2][1] = m_handles[2].y;
	dst[3][0] = m_handles[3].x;
	dst[3][1] = m_handles[3].y;

	ofxHomographyHelper::findHomography(src,dst,m_matrixTransform);

	return m_matrixTransform;
}



