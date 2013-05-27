//
//  silhouetteNode.h
//  murmur
//
//  Created by Julien on 05/05/13.
//
//

#pragma once
#include "ofMain.h"


class SilhouetteNode : public ofNode
{
    public:
        SilhouetteNode(ofImage*, float height=1.70f);
    
        void            customDraw();
        float           getHeight(){return m_height;}
    
    protected:
        ofImage*        mp_image;
        float           m_height;
};
