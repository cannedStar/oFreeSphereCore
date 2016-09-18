Here we are testing the OmniRender library against Open Frameworks
using the GL version 3,2 core profile.

The current strategy is to create a custom ofShader that incorporates the omnirender
vertex displacement function, and corresponding uniforms.  We then pass this and our
om::Render object into a macro OM_RENDER_BEGIN(render,shader). In that macro,
we call "Adaptor functions" such as `om::ShaderProgram::Begin<ofShader>(const ofShader& s)`
which have default definitions but can be overloaded for different Shader class types by a user.

We must use an ofShader so that when we bind it, it does the necessary behind the scene
communication with `ofGLProgrammableRenderer` and remains compatible with Open Frameworks calls such
as `uploadCurrentMatrix()` in `ofGLProgrammableRenderer.cpp` (line 785)

Some more notes on how open frameworks works follows:

The `currentShader` in that method refers to a shader that has been passed to the `::bind` method.
If we bind our own with `::bind` this sets `usingCustomShader` to true, and then calls `uploadMatrices`

This includes 4 matrices, 

		uniform mat4 projectionMatrix;
		uniform mat4 modelViewMatrix;
		uniform mat4 textureMatrix;
		uniform mat4 modelViewProjectionMatrix;

Which are loaded into in the `uploadMatrices()` method called from `bind( ofShader )`
But the `setUniformMatrix4f` function of ofShader checks to see whether such names exist before
attempting to upload data to the gpu.  So we can stick with just

		uniform mat4 modelViewMatrix

Similary, Open Frameworks' 4 input vertex attributes are only set if they exist:		

		in vec4  position;
		in vec2  texcoord;
		in vec4  color;
		in vec3  normal;

But their locations need be bound as in `ofShader::bindDefaults()`:

		bindAttribute(ofShader::POSITION_ATTRIBUTE,::POSITION_ATTRIBUTE);
		bindAttribute(ofShader::COLOR_ATTRIBUTE,::COLOR_ATTRIBUTE);
		bindAttribute(ofShader::NORMAL_ATTRIBUTE,::NORMAL_ATTRIBUTE);
		bindAttribute(ofShader::TEXCOORD_ATTRIBUTE,::TEXCOORD_ATTRIBUTE);

Which may be confusing: the first parameter is an integer and the second a string.
"position" is 0, "color" is 1, normal" is 2, and "tex" is 3

So need to call 

    	glBindAttribLocation(program,location,name.c_str());

Then create and enable vertex array object, and pointer to offsets in memory.  This is handled by 
`ofVBO::bind()` which does a flurry of tests to see where we are in the process.  `bind()` is called 
every time an instance of an `ofVBOmesh` is drawn.  If VAO's are supported, then we do not need to
enable and point specific attributes every time (just the first time, checked for with `vaoChanged`).
