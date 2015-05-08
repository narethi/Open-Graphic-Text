#ifndef GRAPHIC_STRING_H
#define GRAPHIC_STRING_H

#include "stdafx.h"

class CharacterCatalogue;

/*
	The graphicString library is
	used to manage the set of strings that
	make up a string that is used to write
	to the screen
*/
class GraphicString
{
	private:
		CharacterCatalogue * _catalogue;
		string _currentString;
		float *	  _texture, * _color;
		int _width, _height, _characterSpacing;

		//_lineHeights and _numberOfLines are
		//stored for convience so that they
		//can be easily recorded as multi-lined
		//strings when making a text box
		int _numberOfLines = 0;
		vector<int> _lineHeights;
		vector<int> _lineWidths;
		void _generateTextureMap();
		
	public:
		GraphicString();
		GraphicString(CharacterCatalogue * catalogue);
		GraphicString(CharacterCatalogue * catalogue, int characterSpacing);
		GraphicString(CharacterCatalogue * catalogue, string currentString);
		GraphicString(CharacterCatalogue * catalogue, float * color);

		GraphicString(CharacterCatalogue * catalogue, float *color, int characterSpacing);
		GraphicString(CharacterCatalogue * catalogue, string currentString, float *color, int characterSpacing, bool rebuild = true);
		
		void changeCatalogue(CharacterCatalogue * catalogue);
		void changeString(string currentString);

		float * getTextureData(void);
		
		//This is data used specifically
		//if the strings are being handled
		//exculsively as strings
		int getWidth(void);
		int getHeight(void);

		//These are functions used if you
		//are using textbox functions
		//We may not need this
		int getWidth(Uint lineNumber);
		int getHeight(Uint lineNumber);

		//Size returns the size of the
		//null terminated string
		Uint size(void);

		//getChar returns a char from the
		//actual value 
		UChar getChar(Uint position);

		//This gets a pointer of the catalogue
		//for the textBox class
		CharacterCatalogue * getCatalogue(void);

		float * getColor(void);

		//Get linespacing is used for getting the
		//linespacing used in the string, this
		//is for the text box class
		int spacing(void) const;

		/*
			This is a convience function, since
			these are a type of string you should
			be able to use strings to set your 
			graphic string
		*/
		GraphicString operator=(string currentString)
		{
			_currentString = currentString;
			_generateTextureMap();
			return *this;
		}
		string getCurrentString(void) const;
};

#endif