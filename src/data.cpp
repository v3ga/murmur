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
 
    m_font.loadFont("Fonts/FOLDERAL.TTF", 10/*, true, false, true, 0.03, 0*/);
}
