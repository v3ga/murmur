/*
 *  windowAnim.h
 *  murmur
 *
 *  Created by Julien on 04/02/13.
 *  Copyright 2013 2Roqs. All rights reserved.
 *
 */

#pragma once

#include "ofxFensterManager.h"

class windowAnim: public ofxFensterListener 
{
	public:
		~windowAnim() 
		{
			cout << "windowUI destroyed" << endl;
		}
		
		void setup();
		void draw();

//	ofEasyCam cam;


/*
		void keyReleased(int key, ofxFenster* window) {
			if(key==' ')
				ofxFensterManager::get()->deleteFenster(window);
			if(key == 'm'){ //this is to test set and get window position
				ofPoint winPos = window->getWindowPosition();
				window->setWindowPosition(winPos.x + 10, winPos.y);
			}
		}
*/
};