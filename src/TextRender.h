#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "GLHandler.h"
#include "FileHelper.h"
#include "VertCordGenerator.h"

#define TR_SPACE_LENGTH 30
#define TR_FONT_SIZE 72.0f
#define TR_FONT_BOTTOM_SPACE 5.0f
#define TR_CHARACTERS 68

class TextRender
{
	float letterWidth[68];
	
	// Drawing variables 
	GLushort indicies[6];
	GLfloat* verts;
	GLfloat* cords;
	GLint textureID;

	bool setup;

public:
	TextRender();
	~TextRender();

	// Must be called before the renderer can be used 
	void init();

	// Draw text to the screen with the following properties 
	void drawText(GLHandler mgl, char* text, float x, float y, float rotation, float fontSize);

	// Measure how long the text will be on screen
	float measureString(char* text, float size);

	// Get the index for the sent char
	int getCharIndex(char c);

	// Get the width of the sent character 
	float getLetterWidth(char c);

private:

	// Set the indexes based off a char
	void setIndicies(char c);
	
	// Setup the buffers for the font 
	void createFontBuffers();
};
