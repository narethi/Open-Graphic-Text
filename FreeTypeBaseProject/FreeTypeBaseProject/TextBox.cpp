#ifndef TEXT_BOX_M
#define TEXT_BOX_M

#include "TextBox.h"
#include "CharacterCatalogue.h"
#include "GraphicCharacter.h"
#include "GraphicString.h"

TextBox::TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, 
				 int bottom, int left, int height, int width, float * defaultColor) :
				 _characterSpacing(characterSpacing), _bottom(bottom), _left(left), 
				 _height(height), _width(width), _linePosition(0)
{
	if(defaultCatalogue->catalogueSize() == 0)
		defaultCatalogue->addFont(DEFAULT_FONT, 12);
	_catalogues.push_back(defaultCatalogue);
	_colors.push_back(defaultColor);
}

TextBox::TextBox() :
TextBox(new CharacterCatalogue(), DEFAULT_CHARACTER_SPACING)
{}

TextBox::TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, int height, int width) :
TextBox(defaultCatalogue, characterSpacing, DEFAULT_BOTTOM, DEFAULT_LEFT, height, width)
{}

TextBox::TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing, int bottom, int left, int height, int width) :
TextBox(defaultCatalogue, characterSpacing, bottom, left, height, width, new float[3])
{}

TextBox::TextBox(CharacterCatalogue * defaultCatalogue, int characterSpacing) :
TextBox(defaultCatalogue, characterSpacing, DEFAULT_HEIGHT, DEFAULT_WIDTH)
{}

void TextBox::concatenate(string line) {concatenate(line, 0);}
void TextBox::concatenate(string line, int catalogueID) {concatenate(line, catalogueID, _characterSpacing);}
void TextBox::concatenate(int catalogueID, int lineSpacing, string line) { concatenate(line, catalogueID, 0, lineSpacing); }
void TextBox::concatenate(string line, int catalogueID, int colorID)
{
	concatenate(line, catalogueID, colorID, _characterSpacing);
}

void TextBox::concatenate(string line, int catalogueID, int colorID, int lineSpacing)
{
	if(catalogueID >= _catalogues.size())
		catalogueID = _catalogues.size() - 1;
	if(colorID >= _colors.size())
		colorID = _colors.size() - 1;
	_strings.push_back(new GraphicString(_catalogues.at(catalogueID), line, _colors.at(colorID), lineSpacing, false));
	//This is only here for testing
	_rebuildTexture();
}

int TextBox::getLinePos(void) const
{
	return _linePosition;
}

void TextBox::addCatalogue(CharacterCatalogue * catalogue)
{
	catalogue->_changeID(_catalogues.size());
	_catalogues.push_back(catalogue);
}

void TextBox::addColor(float * color)
{
	_colors.push_back(color);
}

void TextBox::moveBox(int left, int bottom)
{
	_left = left;
	_bottom = bottom;
}

void TextBox::changeSize(int width, int height)
{
	_width = width;
	_height = height;
}

void TextBox::clearBox(void)
{
	//run through the loop, delete all
	//graphic strings and then clear
	//the vector that  was holding the strings
	for(int i = 0; i < _strings.size(); i++)
		delete[] _strings.at(i);
	_strings.clear();
}

void TextBox::incLine(void)
{
	_linePosition++;
}

void TextBox::decLine(void)
{
	_linePosition--;
}

void TextBox::_rebuildTexture(void)
{
	/*
		This is the way this will work

		first step
			- Figure out the height of the texture
			- subdivide all of the "strings" into
			  lines
			- store these lines in an n squared list
			- proceed to step 2
	*/
	//First set the line position to 0
	//This is used later, as we want the
	//text box to start on the first line
	_linePosition = 0;

	//Next we need the function variables
	//so that we can store the information
	//on a line by line basis

	//Lines is the physical lines of characters
	//They are STL vectors for convience
	vector<vector<GraphicCharacter *>> lines;

	//data is used to store the information
	//that is neccessary to draw correct size
	//font and color of each character
	vector<charData> data;

	//The next values are used to measure the
	//line heights and map where the top and
	//bottom of each line is so that they can
	//be kept consistent
	vector<int> tops, bottoms;
	_lineHeights.clear();

	GraphicCharacter * tempChar;
	vector<GraphicCharacter *> currentWord;
	vector<GraphicCharacter*> currentLine;
	int currentWidth = 0, wordWidth = 0, lineCount = 0;
	//Set the texture height to 0 so
	//that it can be added to as each
	//line is added
	_textureHeight = 0;

	//Initialize the tops and bottoms
	//add the first value to the list
	tops.push_back(0);
	bottoms.push_back(0);

	//Now loop through all of the characters
	//in the strings first detecting words
	//then detecting lines
	for(int i = 0; i < _strings.size(); i++)
	{
		data.push_back(charData(_strings.at(i)->size(), _strings.at(i)->spacing(), _strings.at(i)->getColor()));
		for(int j = 0; j < _strings.at(i)->size(); j++)
		{
			//Get the characters that from the graphic string and add it to the current word
			tempChar = _strings.at(i)->getCatalogue()->getCharacterByValue(_strings.at(i)->getChar(j));		
			currentWord.push_back(tempChar);
			//Add the width of the current character to the word
			wordWidth += tempChar->getPixelWidth() + data.at(i)._spacing;

			if(tempChar->getActualValue() == ' ' ||
				wordWidth >= _width ||
				(j == _strings.at(i)->size() - 1 && i == _strings.size() - 1))
			{
				int backOff = 0;
				//next check to make sure that 
				//the word is not bigger than
				//the box
				if(wordWidth >= _width)
				{
					//if the string exceeds the
					//size of the box break it
					//into separate strings
					backOff = 1;
					j--;
					//since the width of this
					//word is the size of a full
					//line the width of this word
					//doesn't need to be added to
					//currentWidth
					//wordWidth = 0;
				}
				for(int k = 0; k < currentWord.size() - backOff; k++)
					currentLine.push_back(currentWord.at(k));

				//Clear the word
				currentWord.clear();
				//add the wordWidth to the currentwidth
				//then reset the currentWidth so that it
				//is ready for the next word
				currentWidth += wordWidth;
				wordWidth = 0;
			}

			//First check to see if the word 
			//over takes the width of the box
			if(currentWidth + wordWidth >= _width || tempChar->getActualValue() == '\n')
			{
				//First push the line into the lines vector
				lines.push_back(currentLine);
				//Next we need to adjust the bottoms, and the tops
				for(int k = 0; k < currentLine.size(); k++)
				{
					tempChar =  currentLine.at(k);
					//If the word hits a space then it is done, add it to the line
					if(tops.at(lineCount) < tempChar->getPixelHeight())
						tops.at(lineCount) = tempChar->getPixelHeight();

					if(bottoms.at(lineCount) > int(tempChar->getTop()) - int(tempChar->getPixelHeight()))
						bottoms.at(lineCount) = int(tempChar->getTop()) - int(tempChar->getPixelHeight());
				}
				currentLine.clear();

				//If we have reached the edge of the
				//box then set the current width to the
				//size of the character that forced the wrap
				currentWidth = 0;
				_lineHeights.push_back(int(tops.at(lineCount)) - int(bottoms.at(lineCount)) + _lineSpacing);
				_textureHeight += _lineHeights.at(lineCount);
				bottoms.push_back(0);
				tops.push_back(0);
				lineCount++;
			}
		}

	}
	//Finally make sure that last word is included
	for(int k = 0; k < currentLine.size(); k++)
	{
		tempChar = currentLine.at(k);
		//If the word hits a space then it is done, add it to the line
		if(tops.at(lineCount) < tempChar->getPixelHeight())
			tops.at(lineCount) = tempChar->getPixelHeight();

		if(bottoms.at(lineCount) > int(tempChar->getTop()) - int(tempChar->getPixelHeight()))
			bottoms.at(lineCount) = int(tempChar->getTop()) - int(tempChar->getPixelHeight());
	}
	//Now that all of the strings are sorted we need to build the texture
	_lineHeights.push_back(int(tops.at(lineCount)) - int(bottoms.at(lineCount)) + _lineSpacing);
	_textureHeight += _lineHeights.at(lineCount);
	lines.push_back(currentLine);
	currentLine.clear();
	
	//Now we know how large the texture map is we
	//can finally make the texture array
	_textureMap = new float[4 * _width*_textureHeight];
	for(int i = 0; i < 4 * _width*_textureHeight; i++)
		_textureMap[i] = 0;

	//We need to add a few more variables
	//so that we can run through all of
	//characters stored in the box
	int dataPos = 0, placedChars = 0, xPos = 0;
	int tempWidth = 0, tempHeight = 0, tempTop = 0;
	int tempBottom = 0;
	UChar ** tempBuffer;
	int currentHeight = _textureHeight;

	//We need to loop through each line
	//we do this by going through the list
	//of lines and then each line
	for(int i = 0; i < lines.size(); i++)
	{
		currentWord = lines.at(i);
		tempBottom = bottoms.at(i);
		xPos = 0;

		currentHeight -= _lineHeights.at(i) + tempBottom;
		currentWidth = 0;
		
		for(int j = 0; j < currentWord.size(); j++)
		{

			if(currentWord.at(j)->getActualValue() == '\n')
			{
				//A line return is still a characer
				//so make sure that is recorded
				placedChars++;
				continue;
			}
			if(placedChars >= data.at(dataPos)._numberOfChars)
			{
				dataPos++;
				placedChars = 0;
			}
			//we now need the information
			//that is required to build 
			//up the texture
			tempChar = currentWord.at(j);

			//next check if it is a new line character
			//or if the added char exceeds the size of the box
			tempWidth = tempChar->getPixelWidth();
			currentWidth += tempWidth + data.at(dataPos)._spacing;
			tempHeight = tempChar->getPixelHeight();
			tempBuffer = tempChar->getBuffer();
			tempTop = tempHeight - tempChar->getTop();
			for(int j = 0; j < tempHeight; j++)
			{
				for(int k = 0; k < tempWidth; k++)
				{
					if(tempBuffer[j][k] != 0)
					{
						_textureMap[((j - tempBottom - tempTop + currentHeight)*(_width)+k + xPos) * 4] = data.at(dataPos)._color[0];
						_textureMap[((j - tempBottom - tempTop + currentHeight)*(_width)+k + xPos) * 4 + 1] = data.at(dataPos)._color[1];
						_textureMap[((j - tempBottom - tempTop + currentHeight)*(_width)+k + xPos) * 4 + 2] = data.at(dataPos)._color[2];
						_textureMap[((j - tempBottom - tempTop + currentHeight)*(_width)+k + xPos) * 4 + 3] += float(tempBuffer[j][k]) / 255;
					}
				}
			}
			xPos += tempWidth + data.at(dataPos)._spacing;
			placedChars++;
		}
	}
}

//These functions will be removed or changed later
//they are currently only being used for testing
float * TextBox::getTexture(void) {return _textureMap;}
int TextBox::getWidth(void) const {return _width;}
int TextBox::getTextureHeight(void) const {return _textureHeight;}
int TextBox::getHeight(void) const {return _height;}
int TextBox::getLeft(void) const {return _left;}
int TextBox::getBottom(void) const {return _bottom;}

/*
	getTextureCoords returns a 4 index
	array, that contains the top, right,
	bottom and left coords for the texture
	that is contained in the box
*/
float * TextBox::getTextureCoords(void)
{
	float * coords = new float[4];
	// top, right, bottom, left;
	//First get the current line height
	if(_linePosition >= _lineHeights.size()) _linePosition = 0;
	int currentHeight = 0;
	for(int i = 0; i < _linePosition - 1; i++)
		currentHeight += _lineHeights.at(i);
	coords[0] = 1 - (float(currentHeight) / float(_textureHeight));
	
	coords[1] = 1;
	coords[2] = 1.f - (float(_height + currentHeight) / float(_textureHeight));
	coords[3] = 0;
	return coords;
}

#endif