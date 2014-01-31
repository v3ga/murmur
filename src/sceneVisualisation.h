//
//  sceneVisualisation.h
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "data.h"
#include "scene.h"
#include "deviceNode.h"
#include "surfaceNode.h"
#include "silhouetteNode.h"


class SceneVisualisation : public Scene
{
    public:
        ~SceneVisualisation             ();
    
		void		loadSettings		();

        void        createDeviceNode    ( Device* pDevice, SurfaceNode* );
        void        addDeviceNode       ( DeviceNode*, SurfaceNode* pSurfaceNode);
        void        addSurfaceNode      ( SurfaceNode* );
		
		ofVec3f		getPositionForSilhouette(string deviceId);

    
        SurfaceNode*   getSurfaceNode   ( Surface* );
        DeviceNode*    getDeviceNode    ( Device* );

        void        setup               ();
        void        update              (float dt);
        void        draw                ();
        void        keyPressed          (int key);
        void        mousePressed        (int x, int y);
        void        mouseDragged        (int x, int y);
        void        mouseReleased       (int x, int y);
	
 
    
    protected:
		ofxXmlSettings					m_settings;

        ofCamera                        m_cam;
        float                           m_camRadius,m_camRadiusTarget,m_camRadiusTargetDragStart;
        float                           m_camAngle,m_camAngleTarget,m_camAngleTargetDragStart;
        ofVec3f                         m_camPosition;
        void                            updateCamPosition           ();
    
        bool                            m_mouseDrag;
        ofVec2f                         m_mousePointDragStart;
        ofVec2f                         m_mousePointDrag;
    
        ofNode                          m_world;

        vector<SurfaceNode*>            m_listSurfaceNodes;
        vector<DeviceNode*>             m_listDeviceNodes;
        vector<SilhouetteNode*>         m_listSilhouetteNodes;
};