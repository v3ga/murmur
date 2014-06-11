//
//  toolVideo.cpp
//  murmur
//
//  Created by Julien on 06/06/2014.
//
//

#include "toolVideo.h"

//--------------------------------------------------------------
toolVideo::toolVideo(toolManager* parent) : tool("Video", parent)
{

}

//--------------------------------------------------------------
void toolVideo::setup()
{
	//vidRecorder.setVideoCodec("mpeg4");
    //vidRecorder.setVideoBitrate("800k");
	//vidRecorder.setAudioCodec("mp3");
	//vidRecorder.setAudioBitrate("192k");
	//vidRecorder.setFfmpegLocation( ofFilePath::getAbsolutePath("Video/ffmpeg", true) );
	//ofLog() << ofFilePath::getAbsolutePath("Video/ffmpeg", true);
	//vidRecorder.setup("test.mov", ofGetWidth(), ofGetHeight(), 30);
}

//--------------------------------------------------------------
void toolVideo::update()
{
		//recordingImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        //vidRecorder.addFrame(recordingImage.getPixelsRef());
}

//--------------------------------------------------------------
/*void testApp::videoSaved(ofVideoSavedEventArgs& e){
	// the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
	if(e.error.empty())
	{
		ofSystem("open " + e.videoPath);
	}
	else {
	   	ofLogError("videoSavedEvent") << "Video save error: " << e.error;
	}

//		vidRecorder.close();
		ofLog() << vidRecorder.getMoviePath();

}*/
