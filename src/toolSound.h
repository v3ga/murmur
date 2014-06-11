//
//  toolSound.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"

class toolSound : public tool
{
	public:

		toolSound				(toolManager* parent);

	
		void					createControlsCustom();
		void					update				();
		void					handleEvents		(ofxUIEventArgs& e);
};
