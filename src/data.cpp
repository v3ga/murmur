//
//  data.cpp
//  murmur
//
//  Created by Julien on 30/04/13.
//
//

#include "data.h"

//--------------------------------------------------------------
Data* Data::smp_instance = 0;

//--------------------------------------------------------------
Data::Data()
{
}

//--------------------------------------------------------------
Data* Data::instance(){
	if (smp_instance == 0){
		smp_instance = new Data();
	}
	return smp_instance;
}

//--------------------------------------------------------------
void Data::load()
{
    m_imgSilhouette.loadImage("Images/silhouette.png");
    m_imgSilhouette.setUseTexture(true);
    m_imgLogo.loadImage("Images/murmur_logo.png");
    m_imgLogo.setUseTexture(true);
    
    //m_fontHelvetica.loadFont("Fonts/Helvetica.ttf", 0.1/*, true, false, true, 0.03, 0*/);

    //lt (without dpi), non-full char set, anti aliased, 96 dpi:
	//bool 		loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
}
