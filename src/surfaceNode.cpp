//
//  surfaceNode.cpp
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#include "surfaceNode.h"
#include "data.h"

//--------------------------------------------------------------
SurfaceNode::SurfaceNode(Surface* pSurface, float w, float h) :
mp_surface(pSurface),
m_width(w),
m_height(h),
ofNode()
{
    
}

//--------------------------------------------------------------
ofVec3f SurfaceNode::getPositionCenter()
{
//    return getGlobalTransformMatrix() * ofVec3f(0.5f*getWidth(),0.5f*getHeight(),0.0f);
    return getGlobalPosition()+ofVec3f(0.5f*getWidth(),0.5f*getHeight(),0.0f); // TODO : check this ? 
}

//--------------------------------------------------------------
ofVec3f SurfaceNode::getGlobalPositionDevicePointSurface(Device* pDevice)
{
    return getGlobalOrientation() * (getPosition() + ofVec3f(pDevice->m_pointSurface.x*getWidth(),getHeight()-pDevice->m_pointSurface.y*getHeight(),0.0f));
}

//--------------------------------------------------------------
void SurfaceNode::customDraw()
{
    ofPushStyle();
    ofNoFill();
    ofColor(255,255,255,255);
    ofRectangle(0.0f,0.0f,m_width,m_height);
    if (mp_surface)
    {
        ofPushMatrix();
        ofScale(1,-1);
        mp_surface->getOffscreen().draw(0,-m_height,m_width,m_height);
//        Data::instance()->m_fontHelvetica.drawString("Animation.js", -m_height, m_height);
        ofPopMatrix();

    }
    ofPopStyle();
}



