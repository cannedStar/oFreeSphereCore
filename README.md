Here we are testing the OmniRender library against Open Frameworks
using the GL version 3,2 core profile.

We need to design a shader that incorporates the omnirender
pipeline, and also is compatible with Open Frameworks calls such
as `uploadCurrentMatrix()` in `ofGLProgrammableRenderer.cpp` (line 785)

The `currentShader` in that method refers to a shader that has been passed to the `::bind` method.
If we bind our own with `::bind` this sets `usingCustomShader` to true, and then calls `uploadMatrices`

This includes 4 matrices, 

		uniform mat4 projectionMatrix;
		uniform mat4 modelViewMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;

But the `setUniformMatrix4f` function of ofShader checks to see whether such names exist before
attempting to upload data to the gpu.  So we can stick with just

		uniform mat4 modelViewMatrix

Similary, Open Frameworks' 4 input vertex attributes are only set if they exist:		

		in vec4  position;
		in vec2  texcoord;
		in vec4  color;
		in vec3  normal;

But they need be bound as in `ofShader::bindDefaults()`:

		bindAttribute(ofShader::POSITION_ATTRIBUTE,::POSITION_ATTRIBUTE);
		bindAttribute(ofShader::COLOR_ATTRIBUTE,::COLOR_ATTRIBUTE);
		bindAttribute(ofShader::NORMAL_ATTRIBUTE,::NORMAL_ATTRIBUTE);
		bindAttribute(ofShader::TEXCOORD_ATTRIBUTE,::TEXCOORD_ATTRIBUTE);

So need to call 

    	glBindAttribLocation(program,location,name.c_str());

Then create and enable vertex array object, and pointer to offsets in memory.  This is handled by 
`ofVBO::bind()` which does a flurry of tests to see where we are in the process.  `bind()` is called 
every time an instance of an `ofVBOmesh` is drawn.  If VAO's are supported, then we do not need to
enable and point specific attributes every time (just the first time, checked for with `vaoChanged`).

So, is the `enable()` method of the ofVBO::VertexAttribute class sufficient to find our current bound
shader vertex attributes? 
