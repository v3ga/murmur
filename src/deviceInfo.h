//
//  deviceInfo.h
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#pragma once
#include "ofMain.h"
#include "device.h"

/*
    Côté serveur, cette classe stocke les informations relatives au Device reçues via le réseau (OSC)
    On y associe une surface, espace physique de dessin auquel est associé ce device
*/

class Surface;
class DeviceInfo
{
    public:
        DeviceInfo          (Device*, Surface*);
    
        // Device
        Device*             mp_device;
        void                setDevice			(Device* pDevice){mp_device = pDevice;}
    
        // Surface
        Surface*            mp_surface;
        void                setSurface			(Surface* pSurface){mp_surface = pSurface;}
    
        // point attached to surface (normalized)
        ofVec2f             m_pointOnSurface;
        void                setPointOnSurface(float x, float y){m_pointOnSurface.set(x,y);}
};

class DeviceInfoManager
{
    public:
        DeviceInfoManager    (){};
        ~DeviceInfoManager   ();
    
    
        void                addDeviceInfo(Device*,Surface*);
        Surface*            getSurfaceForDevice(Device*);
    
        vector<DeviceInfo*> m_listDeviceInfos;
        vector<DeviceInfo*>::iterator m_itDeviceInfos;
};