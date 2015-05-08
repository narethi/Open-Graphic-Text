#ifndef GRAPHIC_CHARACTER_H
#define GRAPHIC_CHARACTER_H

#include "stdafx.h"

/*
	GraphicCharacter is an individual
	character that is stored as a series
	of data points neccesary to building
	textures to represent text
	
	The buffer data does not contain
	specific color information

*/
class GraphicCharacter
{
	private:
		//This is the part used
		//for the graphics

		//The ID is unique and is used
		//to locate the character
		//The width and height are used
		//for the actual graphical size
		Uint _ID, _width, _height, 
		//The top and left are important
		//for the kerning of the 
			_top, _left;
		//The buffer is used to hold the
		//alpha information of the font
		UChar ** _buffer;

		//The next section is used to
		//give the user debugging information

		//This is the name of the
		//stored character i.e. NULL, SOH etc.
		string _name;
		//This is the description of the
		//currently stored character
		string _description;
		//If the font is serif set to true
		bool _serif = false;
		ULong _fontSize = 12;
		UChar _actualValue;

	public:
	//protected:
		GraphicCharacter();
		GraphicCharacter(Uint ID, string name, UChar actualValue, 
						 ULong fontSize, Uint width, Uint height, 
						 Uint top, Uint left, string description, 
						 bool serif, UChar ** buffer);
		
		bool	isSerif(void)			const;
		Uint	getID(void)				const;
		Uint	getPixelWidth(void)		const;
		Uint	getPixelHeight(void)	const;
		Uint	getFontSize(void)		const;
		Uint	getTop(void)			const;
		Uint	getLeft(void)			const;
		string	getName(void)			const;
		string	getDescription(void)	const;
		UChar	getActualValue(void)	const;

		UChar ** getBuffer(void);
};

#endif