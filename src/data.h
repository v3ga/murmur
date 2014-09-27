//
//  data.h
//  murmur
//
//  Created by Julien on 30/04/13.
//
//

#pragma once
#include "ofMain.h"

#define DATA Data::instance()

class Data
{
    public:
        Data					();
        static Data*			instance();
        void                    load();

        ofImage                 m_imgSilhouette;
        ofImage                 m_imgLedLight; // TODO
        ofImage                 m_imgLogo;
    
        ofTrueTypeFont          m_fontHelvetica;
        ofTrueTypeFont          m_font;

 
    private:
        static Data*            smp_instance;
};