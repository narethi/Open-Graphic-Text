#ifndef GRAPHIC_STRING_M
#define GRAPHIC_STRING_M

#include "GraphicString.h"
#include "CharacterCatalogue.h"
#include "GraphicCharacter.h"

GraphicString::GraphicString() :
GraphicString(new CharacterCatalogue(),"", new float[3], 2)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue) :
GraphicString(catalogue, "", new float[3], 2)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue, int characterSpacing) :
GraphicString(catalogue, "", new float[3], characterSpacing)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue, string currentString) :
GraphicString(catalogue, currentString, new float[3], 2)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue, float *color, int characterSpacing) :
GraphicString(catalogue, "", color, characterSpacing)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue, float * color) :
GraphicString(catalogue, "", color, 2)
{}

GraphicString::GraphicString(CharacterCatalogue * catalogue, string currentString, float * color, int characterSpacing, bool rebuild) :
_catalogue(catalogue), _currentString(currentString), _color(color), _width(0), _height(0), _characterSpacing(characterSpacing)
{
	//When building a graphic string we need to first
	//check to make sure we aren't building a blank string
	//if we are return so the user can add string data later
	if(_currentString.size() == 0) return;
	//Otherwise generate the texturemap
	if(rebuild) _generateTextureMap();
}

CharacterCatalogue * GraphicString::getCatalogue(void)
{
	return _catalogue;
}

UChar GraphicString::getChar(Uint position)
{
	if(position >= _currentString.size())
		return 0;
	return _currentString[position];
}

Uint GraphicString::size(void)
{
	return _currentString.size();
}

/*
	The specific purpose of this function
	is to get the width of each line of a
	string so that it can be inserted into
	a textbox
*/
int GraphicString::getWidth(Uint lineNumber)
{
	if(lineNumber >= _numberOfLines)
		return _lineWidths.at(_lineWidths.size() - 1);
	return _lineWidths.at(lineNumber);
}

float * GraphicString::getColor(void)
{
	return _color;
}

int GraphicString::spacing(void) const
{
	return _characterSpacing;
}

/*
The specific purpose of this function
is to get the width of each line of a
string so that it can be inserted into
a textbox
*/
int GraphicString::getHeight(Uint lineNumber)
{
	if(lineNumber >= _numberOfLines)
		return _lineHeights.at(_lineWidths.size() - 1);
	return _lineHeights.at(lineNumber);
}

/*
	GenerateTextureMap, takes the entire actual
	string, then generates a texture buffer 
	texture buffers need to be able to handle special
	characters and line returns
*/
void GraphicString::_generateTextureMap()
{
	//The first stage is to collect the 
	//characters that belong in the string
	vector<GraphicCharacter *> temporaryChars;
	GraphicCharacter * tempChar;
	_height = _width = 0;
	//Set the numberOfLines to 1 to start
	_numberOfLines = 1;
	//next clear the line heights list
	_lineHeights.clear();
	_lineWidths.clear();
	vector<int> bottoms;
	vector<int> tops;
	bottoms.push_back(0);
	tops.push_back(0);
	int xPos = 0, currentHeight = 0, count = 0;
	for(int i = 0; i < _currentString.size(); i++)
	{
		//First collect the characters from the catalogue
		tempChar = _catalogue->getCharacterByValue(_currentString[i]);
		//Check to see if the current character is a line return
		//if the character is a line return 
		temporaryChars.push_back(tempChar);
		//Check for the new line
		if(tempChar->getActualValue() == '\n')
		{
			//If the new line exists then
			//update the number of lines
			_numberOfLines++;
			//then update the _lineWidths
			_lineWidths.push_back(_width);
			//then update the _lineHieghts
			_lineHeights.push_back((tops.at(count) - bottoms.at(count)));
			//then set the _height to 0 so that
			//the new line can have its one height
			bottoms.push_back(0);
			tops.push_back(0);
			//reset the width so that 
			//the next line starts at 0
			_width = 0;
			count++;
			//Continue to the next char
			continue;
		}

		//Next add its width to the _width of the string
		_width += tempChar->getPixelWidth() + _characterSpacing;
		//next check the height, replace it if 
		//the char is taller than the last
		if(tops.at(count) < tempChar->getPixelHeight()) 
			tops.at(count) = tempChar -> getPixelHeight();
		if(bottoms.at(count) > int(tempChar->getTop()) - int(tempChar->getPixelHeight()))
			bottoms.at(count) = int(tempChar->getTop()) - int(tempChar->getPixelHeight());
	}
	_lineHeights.push_back(tops.at(count) - bottoms.at(count));
	_lineWidths.push_back(_width);
	for(int i = 0; i < _lineHeights.size(); i++)
	{
		_height += _lineHeights.at(i);
		if(_lineWidths.at(i) >= _width)
			_width = _lineWidths.at(i);
	}
	//Now that we have the size of the string
	//we need to build the texture buffer
	delete[] _texture;
	_texture = new float[_height*_width * 4];
	//set up the temporary variables so that
	//we can keep track of how tall the characters
	//are and where the tops are
	int tempWidth, tempHeight, tempTop;
	count = 0;
	currentHeight = _height - _lineHeights.at(0);
	//Next with the texture buffer built
	//we need to populate the texture buffer
	UChar ** tempBuffer;
	for(int i = 0; i < temporaryChars.size(); i++)
	{
		//first get the char
		tempChar = temporaryChars.at(i);
		//next check if it is a new line character
		if(tempChar->getActualValue() == '\n')
		{
			xPos = 0;
			count++;
			currentHeight -= _lineHeights.at(count);
			continue;
		}
		tempWidth	= tempChar->getPixelWidth();
		tempHeight	= tempChar->getPixelHeight();
		tempBuffer	= tempChar->getBuffer();
		tempTop		= tempHeight - tempChar->getTop();
		for(int j = 0; j < tempHeight; j++)
		{
			for(int k = 0; k < tempWidth; k++)
			{
				_texture[((j - bottoms.at(count) - tempTop + currentHeight)*(_width) + k + xPos) * 4]	   = _color[0];
				_texture[((j - bottoms.at(count) - tempTop + currentHeight)*(_width) + k + xPos) * 4 + 1] = _color[1];
				_texture[((j - bottoms.at(count) - tempTop + currentHeight)*(_width) + k + xPos) * 4 + 2] = _color[2];
				_texture[((j - bottoms.at(count) - tempTop + currentHeight)*(_width) + k + xPos) * 4 + 3] = float(tempBuffer[j][k]) / 255;
			}
		}
		xPos += tempWidth + _characterSpacing;
	}
}

void GraphicString::changeString(string currentString)
{
	_currentString = currentString;
	_generateTextureMap();
}

float * GraphicString::getTextureData(void)
{
	return _texture;
}

int GraphicString::getWidth(void)
{
	return _width;
}

int GraphicString::getHeight(void)
{
	return _height;
}

string GraphicString::getCurrentString(void) const
{
	return _currentString;
}
#endif