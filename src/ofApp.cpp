#include "ofApp.h"

/// It is generally preferable to use your frameworks own shader class for creating the modified shader
/// because calling YourFrameWork::Shader::bind() method may do funky things (as in Open Frameworks)
/// which require asking for current render context, etc.
///
/// Steps: 1. Create Custom ofShader with omVertexDisplacement function
///        2. Define Adaptor Functions for ShaderProgram::Uniform1f and ShaderProgram::Uniform1i
///                  (note for other frameworks we may also need adaptors for ShaderProgram::Begin and ShaderProgram::End)
///        3. Initialize Custom ofShader and Om::Render object
///        4. Bracket draw routine with OM_RENDER_BEGIN(render,shader) and OM_RENDER_END(render,shader) macros
///
/// --GG


// 1. Custom Shader (modified from ofGLProgrammableRenderer::defaultVertexShader)
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
  gl_Position = omVertexDisplacement(modelViewMatrix * position );//omVertexDisplacement
}
  
)";

// Same as typical of default fragment shader
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


///2. DEFINE ADAPTER FUNCTIONS (used by Om::Render class to specify how user's shader class binds uniform variables)
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
  
  /// SET UP CUSTOM SHADER
  myShader.setupShaderFromSource(GL_VERTEX_SHADER, omCustomVertexShader);
  myShader.setupShaderFromSource(GL_FRAGMENT_SHADER, ofDefaultFragmentShader);
  myShader.bindDefaults(); //bind attributes to default locations (e.g. "position" is bound to 0)
  myShader.linkProgram();  //link compiled programs

  /// INITIALIZE om::Render
  om::Render::Settings s;
  s.configPath = om::al::configPath(om::OF);
  s.cubeMapRes = 1024;
  s.winWidth = ofGetWidth();
  s.winHeight = ofGetHeight();
  s.near = 10;
  s.stereoMode = om::MONO;
  render.init(s);

}

//--------------------------------------------------------------
void ofApp::update(){}

//--------------------------------------------------------------
void ofApp::draw(){
    auto ofr = std::static_pointer_cast<ofGLProgrammableRenderer>(ofGetCurrentRenderer());
 
      camera.begin();
  
      ///3. BRACKET DRAW METHODS WITH OM_RENDER_BEGIN and OM_RENDER_END MACROS
      OM_RENDER_BEGIN(render, myShader);

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
