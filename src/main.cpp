#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main()
{
	ofAppGlutWindow window;
//	ofSetupOpenGL(ofxFensterManager::get(), 400, 400, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 1024, 900, /*OF_FULLSCREEN*/OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 1061, 524, /*OF_FULLSCREEN*/OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new testApp());

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	// ofRunFensterApp(new testApp());
}

