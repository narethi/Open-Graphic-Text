#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "stdafx.h"

class GraphicString;
class CharacterCatalogue;

//These are default values for textboxes
#define DEFAULT_FONT				"Arial.ttf"
#define DEFAULT_CHARACTER_SPACING	1
#define DEFAULT_BOTTOM				10
#define DEFAULT_LEFT				10
#define DEFAULT_WIDTH				1
#define DEFAULT_HEIGHT				1


enum positioning {relative, absolute};

/*
	Now that we have the characters,
	catalogues, and strings working we
	need to build a container to manage
	the strings of text
*/
class TextBox
{
	private:
		
	struct charData
	{
		Uint _numberOfChars;
		int _spacing;
		float * _color;
		charData()
		{
			_spacing = 0;
			_numberOfChars = 0;
			_color = new float[4];
		}
		charData(Uint numberOfChars, int spacing, float * color)
		{
			_numberOfChars = numberOfChars;
			_spacing = spacing;
			_color = color;
		}
	};
		
		/*
			The width, hieght, botom and left
			give information on the bootm left
			corner of the box, and the width
			and the height define the how big
			the box is
		*/
		int _width, _height, _bottom, _left;
		int _textureHeight;

		//strings is the list of 
		//all strings already compiled
		vector<GraphicString *> _strings;

		//This is the list of all available catalogues
		//The default catalogue is catalogue 0
		vector<CharacterCatalogue *> _catalogues;

		//This is the default character spacing
		//The user can specify a different 
		//spacing when adding new strings
		int _characterSpacing;

		//This the number of pixels between each line
		int _lineSpacing = 4;

		//This is the list of available
		//colors of the text, the default
		//color is at position 0
		vector<float *> _colors;

		//This is the map that stores all of
		//the texture information 
		//for the text box
		float * _textureMap;
		
		//This indicates which line the
		//boxes starts at
		int _linePosition;
		//lineheights are used to keep track
		//of how much to move the cursor
		//when reading through each line
		vector<int> _lineHeights;

		//This builds the texture for the
		//box that it will be in
		void _rebuildTexture(void);

	public:
		TextBox();
		TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing);
		TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, int height, int width);
		TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, int bottom, int left, int height, int width);
		TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, int bottom, int left, int height, int width, float * defaultColor);

		//concatenate adds a new 
		//string to the textbox
		void concatenate(string line);
		void concatenate(string line, int catalogueID);
		void concatenate(int catalogueID, int lineSpacing, string line);
		void concatenate(string line, int catalogueID, int colorID, int lineSpacing);
		void concatenate(string line, int catalogueID, int colorID);
		
		Uint getNumberOfLines(void) const;
		int getLinePos(void) const;

		void addCatalogue(CharacterCatalogue * catalogue);
		void addColor(float * color);

		void moveBox(int left, int bottom);
		void changeSize(int width, int height);

		//Clear box removes all current
		//text entries from the text box
		void clearBox(void);

		//incLine moves the cursor to the next line
		void incLine(void);
		//decLine moves the cursor to the previous line
		void decLine(void);

		//We need functions to deal 
		//with changing window sizes
		void changeWindowSize(int xSize, int ySize);


		//These are only for testing and will be removed
		//or changed later
		float * getTexture(void);

		int getWidth(void) const;
		int getHeight(void) const;
		int getLeft(void) const;
		int getBottom(void) const;

		int getTextureHeight(void) const;

		float * getTextureCoords(void);
};

#endif