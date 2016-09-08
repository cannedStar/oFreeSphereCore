#include "ofApp.h"

//Custom Shader (copied from ofGLProgrammableRenderer::defaultVertexShader, 
//with eliminated projection and modelviewprojection matrices)
//We can eliminate these because open frameworks sets uniforms only if they are found
//om::vertexDisplacement
static const string omCustomVertexShader = om::glslVersion  + R"(

  uniform mat4 modelViewMatrix;
  uniform mat4 textureMatrix;

  uniform vec4 color;

  in vec4  position;
  //in vec2  texcoord;
 // in vec3  normal;

  out vec4 colorVarying;
  //out vec2 texCoordVarying;
  //out vec4 normalVarying;

  void main()
  {
    colorVarying = color;
    //texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
    //gl_Position =  omVertexDisplacement(position);
    gl_Position = position;
  }
  
)";

static const string ofDefaultVertexShader = R"(
#version 150
uniform mat4 projectionMatrix; 
uniform mat4 modelViewMatrix; 
uniform mat4 textureMatrix; 
uniform mat4 modelViewProjectionMatrix; 

in vec4 position; 
in vec2 texcoord; 
in vec4 color; 
in vec3 normal; 

out vec4 colorVarying; 
out vec2 texCoordVarying; 
out vec4 normalVarying; 

void main() { 
	colorVarying = color; 
	texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy; 
	gl_Position = modelViewProjectionMatrix * position; 
}
)";


static const string omCustomFragmentShader = om::glslVersion + R"(
  in vec4 colorVarying;
  out vec4 frag_color;
  void main() {

    frag_color = colorVarying;

  }

)";

static const string ofDefaultFragmentShader = R"(
#version 150
uniform float usingTexture; 
uniform float usingColors; 
uniform vec4 globalColor; 
in float depth; 
in vec4 colorVarying; 
in vec2 texCoordVarying; 
out vec4 fragColor;
void main(){ 
	fragColor = globalColor; 
}
)";

float camX = 0, camY = 0, camZ = 5;
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
  //render.init(s);
  render.captureShader.loadString( ofDefaultVertexShader, ofDefaultFragmentShader );


  printf("position %d\n", render.captureShader.attributeLocation("position"));
  printf("texcoord %d\n", render.captureShader.attributeLocation("texcoord"));
  printf("color %d\n", render.captureShader.attributeLocation("color"));
  printf("normal %d\n", render.captureShader.attributeLocation("normal"));
  
  printf("SETUP\n");

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
  ofr->loadViewMatrix(cam_capture.getModelViewMatrix());

//  static int x = 1;
//  if (x==1){
//   auto ofr = std::static_pointer_cast<ofGLProgrammableRenderer>(ofGetCurrentRenderer());
//   auto vs = ofr -> getCurrentShader().getShaderSource(GL_VERTEX_SHADER);
//   auto fs = ofr -> getCurrentShader().getShaderSource(GL_FRAGMENT_SHADER);
//
//   printf("%s\n%s\n", vs.c_str(), fs.c_str());
//  
//    x++;
//  }
  
 // OM_RENDER_BEGIN(render)
 //     render.captureShader.begin();
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
 //     render.captureShader.end();

//  OM_RENDER_END(render)      
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
