#include "ofApp.h"


float camX = 0, camY = 0, camZ = 0;
float lookX = 0, lookY = 0, lookZ = 0;
ofVec3f up_vec(0, 1, 0);

om::Render render;
ofCamera cam_capture;
ofIcoSpherePrimitive icoSphere(5, 1);

//--------------------------------------------------------------
void ofApp::setup(){
  ofDisableSmoothing();
  ofDisableAntiAliasing();
  ofBackground(255, 0, 0);
  cam_capture.setPosition(camX, camY, camZ);
  
  om::Render::Settings s;
  s.configPath = om::al::configPath(om::OF);
  s.cubeMapRes = 128;
  s.winWidth = ofGetWidth();
  s.winHeight = ofGetHeight();
  s.near = 10;
  s.stereoMode = om::MONO;
  render.init(s);
}

//--------------------------------------------------------------
void ofApp::update(){
  float t = ofGetElapsedTimef();
  lookX = sinf(t * 0.1);
  lookZ = cosf(t * 0.1);
  cam_capture.lookAt(ofVec3f(lookX, lookY, lookZ), up_vec);
}

//--------------------------------------------------------------
void ofApp::draw(){
  auto ofr = ofGetCurrentRenderer();
  ofr->matrixMode(OF_MATRIX_MODELVIEW);
  ofr->pushMatrix();
  ofr->loadViewMatrix(cam_capture.getModelViewMatrix());
  
  render.begin();
  for (int eye = 0; eye < render.isStereo() + 1; eye++) {
    float parallax = render.eyeSep() * (eye - 0.5);
    for (int face = 0; face < 6; face++) {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                             render.cubeMap[eye].id(), 0);
      glClearColor(0.0, face / 6.0, (6.0 - face) / 6.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      render.captureShader.begin();
      render.captureShader.uniform1i("omni_face", face);
      render.captureShader.uniform1f("omni_eyeSep", parallax);
      render.captureShader.uniform1f("lighting", 0.0);
      render.captureShader.uniform1f("texture", 0.0);
      
      for (int x = -5; x <= 5; x++)
        for (int y = -5; y <= 5; y++)
          for (int z = -5; z <= 5; z++) {
            int r = 255 * (x + 5) / 10;
            int g = 255 * (y + 5) / 10;
            int b = 255 * (z + 5) / 10;
            ofPushMatrix();
            ofTranslate(40 * x, 40 * y, 40 * z);
            ofSetColor(r, g, b);
            ofr->draw(icoSphere, OF_MESH_FILL);
            ofPopMatrix();
          }
      
      render.captureShader.end();
    }
  }
  ofr->matrixMode(OF_MATRIX_MODELVIEW);
  ofr->popMatrix();
  render.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
