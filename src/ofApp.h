#pragma once

#include "ofMain.h"

#define OM_OPENGL3
#include "OmniRender/include/omRender.hpp"

class ofApp : public ofBaseApp{

  float camX = 0, camY = 0, camZ = 5;
  float lookX = 0, lookY = 0, lookZ = 0;
  ofVec3f up_vec = ofVec3f(0, 1, 0);
  
  om::Render render;
  ofEasyCam camera;
  ofIcoSpherePrimitive icoSphere = ofIcoSpherePrimitive(5, 1);
  ofShader myShader;
  
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  
  
		
};
