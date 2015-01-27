#include "ofMain.h"
#include "testApp.h"

#if MURMUR_MULTI_WINDOWS
	#include "ofxMultiGLFWWindow.h"
	#include "ofGLProgrammableRenderer.cpp"
#else
	#include "ofAppGlutWindow.h"
#endif


int main()
{
	#if MURMUR_MULTI_WINDOWS

    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(3,2); // must be set

    glfw.windowCount = 2;
	ofSetupOpenGL(&glfw,1024,900,OF_WINDOW);



	#else

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 900, /*OF_FULLSCREEN*/OF_WINDOW);			// <-------- setup the GL context

	#endif

	ofRunApp(new testApp());
}

