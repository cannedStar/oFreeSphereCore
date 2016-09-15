#include "ofApp.h"

//Custom Shader (copied from ofGLProgrammableRenderer::defaultVertexShader, 
//with eliminated projection and modelviewprojection matrices)
//We can eliminate these because open frameworks sets uniforms only if they are found
//om::vertexDisplacement
static const string omCustomVertexShader = om::glslVersion  + om::vertexDisplacement + R"(

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec4 colorVarying;
out vec2 texCoordVarying;
vec3 normalVarying;

void main() {
  colorVarying = color;
  normalVarying = normal;
  texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
  gl_Position = ( projectionMatrix * modelViewMatrix * position );//omVertexDisplacement
}
  
)";

static const string ofDefaultVertexShader = R"(
#version 150
uniform mat4 projectionMatrix; 
uniform mat4 modelViewMatrix; 
uniform mat4 textureMatrix; 
uniform mat4 modelViewProjectionMatrix; 

in vec4 position; 
in vec4 color; 
in vec3 normal; 
in vec2 texcoord; 

out vec4 colorVarying; 
out vec2 texCoordVarying; 
vec3 normalVarying; 

void main() { 
	colorVarying = color; 
  normalVarying = normal;
	texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy; 
	gl_Position = projectionMatrix * modelViewMatrix * position;
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


//CUSTOM ADAPTER FUNCTIONS

template<>
void om::ShaderProgram::Uniform1f<ofShader>(const ofShader& s, std::string name, float val){
  s.setUniform1f(name,val);
}

template<>
void om::ShaderProgram::Uniform1i<ofShader>(const ofShader& s, std::string name, float val){
  s.setUniform1i(name,val);
}

//--------------------------------------------------------------
void ofApp::setup(){
  
  ofDisableSmoothing();
  ofDisableAntiAliasing();
  ofBackground(255, 0, 0);

  camera.setPosition(camX, camY, camZ);
  

  om::Render::Settings s;
  s.configPath = om::al::configPath(om::OF);
  s.cubeMapRes = 128;
  s.winWidth = ofGetWidth();
  s.winHeight = ofGetHeight();
  s.near = 10;
  s.stereoMode = om::MONO;


  myShader.setupShaderFromSource(GL_VERTEX_SHADER, omCustomVertexShader);//ofDefaultVertexShader);
  myShader.setupShaderFromSource(GL_FRAGMENT_SHADER, ofDefaultFragmentShader);
  myShader.bindDefaults(); //bind attributes to default locations
  myShader.linkProgram(); //link compiled programs
  
  render.init(s);

  
  printf("SETUP\n");

}

//--------------------------------------------------------------
void ofApp::update(){
  float t = ofGetElapsedTimef();
  lookX = sinf(t * 0.1);
  lookZ = cosf(t * 0.1);
  //camera.lookAt(ofVec3f(0, 0, 0), up_vec);
}

//--------------------------------------------------------------
void ofApp::draw(){
    auto ofr = std::static_pointer_cast<ofGLProgrammableRenderer>(ofGetCurrentRenderer());
 
     // render.tmpbegin();
      camera.begin();
      OM_RENDER_BEGIN(render, myShader );

      //om::ShaderProgram::Begin( myShader );
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
    //  om::ShaderProgram::End( myShader );

     //render.tmpend();
  
  
     OM_RENDER_END(render, myShader);
      camera.end();

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
