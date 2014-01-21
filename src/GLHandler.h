#pragma once
/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLHandler
{
public:
	// Shader handles 
	GLuint program;
	GLint mPositionHandle;
	GLint mColorHandle;
	GLint mTextureHandle;
	GLint mTextCordHandle;
	GLint mProjMatrixHandle;
	GLint mModelMatrixHandle;
	GLint mViewMatrixHandle;
	GLint mUseTextureHandle;

	// Matrix's
	glm::mat4 orthoMatrix;
	glm::mat4 camera3DMatrix;

	// Functions
	GLHandler();
	~GLHandler(void);
	// Call to load shaders 
	int setupShaders();

	// Call to set up gl for rendering
	void setupGL();
	// Call when done rendering 
	void endGL();
	// Set the gl flat color
	// This should be in the form of {R,G,B,A}
	void setFlatColor(const GLfloat*);
	
	// Set the Projection matrix for the shader. 
	// This is used to map the game to the screen, 
	// so use an orthographic matrix or a projection 
	// matrix.
	// use glm::mat4() for identity 
	void setProjectionMatrix(glm::mat4);
	
	// Set the View matrix for the shader. 
	// This is used as the camera for the game. 
	// use glm::mat4() for identity 
	void setViewMatrix(glm::mat4);

	// Set the model matrix for the shader. 
	// use glm::mat4() for identity 
	void setModelMatrix(glm::mat4);

	// Call to fix the ortho matrix if screen size has changed 
	void setOrthoMatrix(const float width,const float height);

	// Set the 3d camera matrix settings 
	// Camera location  : glm::vec(0,0,0)
	// Target           : glm::vec(0,0,0)
	void setCamera3DMatrix(const glm::vec3 cameraLocation, const glm::vec3 targetLocation, const float aspectRatio);

	// Call to turn on and off textures
	void toggleTextures(bool);
};