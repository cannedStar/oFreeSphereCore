#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
  
  ofGLFWWindowSettings settings;
  
  // ofWindowSettings
  settings.width = 640;
  settings.height = 480;
  settings.windowMode = OF_WINDOW; // or OF_FULLSCREEN
  
  // ofGLWindowSettings
  settings.setGLVersion(3,2);
  settings.stereo = false;
  
  ofCreateWindow(settings);
  

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
