//
//  surfaceNode.h
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#pragma once

#include "ofMain.h"
#include "surface.h"

class SurfaceNode : public ofNode
{
    public:
        SurfaceNode         (Surface* pSurface, float w, float h );

    
        void                    customDraw        ();
        float                   getWidth(){return m_width;}
        float                   getHeight(){return m_height;}
        ofVec3f                 getPositionCenter();
        ofVec3f                 getGlobalPositionDevicePointSurface(Device*);
        Surface*                getSurface(){return mp_surface;}
    
    protected:
        Surface*               mp_surface;
        float                  m_width;     // meters
        float                  m_height;    // meters 
};
