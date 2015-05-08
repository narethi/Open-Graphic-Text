#ifndef GRAPHIC_CHARACTER_M
#define GRAPHIC_CHARACTER_M

#include "GraphicCharacter.h"

GraphicCharacter::GraphicCharacter() :
GraphicCharacter(0, "emptyCharacter", '\0', 0, 0, 0, 0, 0, "This character has not been initialized", false, NULL)
{}

GraphicCharacter::GraphicCharacter(Uint ID, string name, UChar actualValue, ULong fontSize, 
								   Uint width, Uint height, Uint top, Uint left, 
								   string description, bool serif, UChar ** buffer) :
_ID(ID), _name(name), _actualValue(actualValue), _fontSize(fontSize), 
_width(width), _height(height), _top(top), _left(left), 
_description(description), _serif(serif), _buffer(buffer)
{}

bool GraphicCharacter::isSerif(void) const {return _serif;}
Uint GraphicCharacter::getID(void) const {return _ID;}
Uint GraphicCharacter::getPixelWidth(void) const {return _width;}
Uint GraphicCharacter::getPixelHeight(void) const {return _height;}
Uint GraphicCharacter::getFontSize(void) const {return _fontSize;}
Uint GraphicCharacter::getTop(void)	const { return _top;}
Uint GraphicCharacter::getLeft(void) const {return _left;}
string GraphicCharacter::getName(void) const {return _name;}
string GraphicCharacter::getDescription(void) const {return _description;}
UChar  GraphicCharacter::getActualValue(void) const {return _actualValue;}
UChar ** GraphicCharacter::getBuffer(void) {return _buffer;}

#endif