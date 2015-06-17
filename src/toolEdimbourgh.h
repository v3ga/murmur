//
//  toolEdimbourgh.h
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#pragma once

#include "tool.h"
#include "classProperty.h"

class toolEdimbourgh : public tool
{
	public:
		toolEdimbourgh			(toolManager*);

		void					createControlsCustom	();
		void					setup					();
		void					update					();
		void					handleEvents			(ofxUIEventArgs& e);


 
	 	classProperties			m_properties;
};