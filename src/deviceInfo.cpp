//
//  deviceInfo.cpp
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#include "deviceInfo.h"

//--------------------------------------------------------------
DeviceInfo::DeviceInfo(Device* pDevice, Surface* pSurface)
{
    //m_description.set(id,nbLeds,distLEDs);
    setDevice(pDevice);
    setSurface(pSurface);
}

//--------------------------------------------------------------
DeviceInfoManager::~DeviceInfoManager()
{
    m_itDeviceInfos = m_listDeviceInfos.begin();
    for (;m_itDeviceInfos != m_listDeviceInfos.end(); ){
        delete *m_itDeviceInfos;
        m_itDeviceInfos = m_listDeviceInfos.erase(m_itDeviceInfos);
    }
}

//--------------------------------------------------------------
void DeviceInfoManager::addDeviceInfo(Device* pDevice,Surface* pSurface)
{
    m_listDeviceInfos.push_back(new DeviceInfo(pDevice,pSurface));
}

//--------------------------------------------------------------
Surface* DeviceInfoManager::getSurfaceForDevice(Device* pDevice)
{
    m_itDeviceInfos = m_listDeviceInfos.begin();
    for (;m_itDeviceInfos != m_listDeviceInfos.end(); ){
        if ((*m_itDeviceInfos)->mp_device == pDevice)
            return (*m_itDeviceInfos)->mp_surface;
    }
    return 0;
}


