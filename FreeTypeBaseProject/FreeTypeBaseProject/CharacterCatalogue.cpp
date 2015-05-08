#ifndef CHARACTER_CATALOGUE_M
#define CHARACTER_CATALOGUE_M

#include "CharacterCatalogue.h"
#include "GraphicCharacter.h"

CharacterCatalogue::CharacterCatalogue(string fontName, Uint ID) :
_fontName(fontName), _ID(ID)
{

}

CharacterCatalogue::CharacterCatalogue() :
CharacterCatalogue("Unitialized Font", 0)
{

}

void CharacterCatalogue::addFont(string fontFile, Uint fontSize)
{
	//The following is the font library information
	FT_Library currentLibrary;
	FT_Face currentFace;

	FT_GlyphSlot	currentGlyph;
	FT_Error		currentErrorMessage;

	Uint currentWidth, currentHeight;
	//tempChar is what gets pushed into
	//the vector of chars
	GraphicCharacter * tempChar;
	//First load the free type library into memory
	currentErrorMessage = FT_Init_FreeType(&currentLibrary);
	//If there was an error, set last error (windows only)
	//then return without adding anything to the catalogue
	if(currentErrorMessage)
	{
		SetLastError(currentErrorMessage + 0xA0000000);
		return;
	}

	//Now that the library is loaded we need to start
	//adding the glyphs
	currentErrorMessage = FT_New_Face(currentLibrary, &fontFile[0], 0, &currentFace);
	//If there was an error, set last error (windows only)
	//then return without adding anything to the catalogue
	if(currentErrorMessage)
	{
		SetLastError(currentErrorMessage + 0xA0000000);
		return;
	}
	//The size of fonts is measured in 1/64 of a pixel, 
	//so we have to multiply the fontSize by 64 to get the
	//actual font size
	currentErrorMessage = FT_Set_Char_Size(currentFace, fontSize * 64, 0, 0, 0);
	//If there was an error, set last error (windows only)
	//then return without adding anything to the catalogue
	if(currentErrorMessage)
	{
		SetLastError(currentErrorMessage + 0xA0000000);
		return;
	}
	//Now that the face is set up, we need to set the current 
	//glyph to the current face's glyph
	currentGlyph = currentFace->glyph;
	//Next we run through all of the values in the font
	//and load in each character 
	//into the vector of graphic characters
	for(int i = 0; i < 255; i++)
	{
		//Now load all of the faces that can be found in the
		//font, if a value does not exist skip to the next one
		currentErrorMessage = FT_Load_Char(currentFace, i, FT_LOAD_RENDER);
		if(currentErrorMessage) continue;
		//If we are on the space then we need 
		//to do a new procedure since all spaces
		//are supposed to be handled by the text
		//library so we will start by making a
		//basic space for a catalogue
		//space is ascii value 32
		else if(i == 32)
		{
			int spaceWidth = fontSize / 10;
			(spaceWidth == 0 ? spaceWidth = 1 : NULL);
			UChar ** spaceBitmap = new UChar*[1];
			spaceBitmap[0] = new UChar[spaceWidth];
			for(int j = 0; j < spaceWidth; j++) spaceBitmap[0][j] = 0;
			tempChar = new GraphicCharacter(_characterList.size(), "" + i, (UChar)i, fontSize,
												spaceWidth, 1, 0,0, "" + i, false, spaceBitmap);
			_characterList.push_back(tempChar);
			continue;
		}
		tempChar = new GraphicCharacter(_characterList.size(), "" + i, (UChar)i, fontSize, 
										 currentGlyph->bitmap.width, currentGlyph -> bitmap.rows, currentGlyph-> bitmap_top, currentGlyph->bitmap_left, "" + i, 
										 false, _generateAlphaBuffer(&currentGlyph->bitmap, currentGlyph->bitmap_top));
		_characterList.push_back(tempChar);
	}
}

UChar ** CharacterCatalogue::_generateAlphaBuffer(FT_Bitmap * fontBitmap, FT_Int glyphTop)
{
	int xSize = fontBitmap->width;
	int ySize = fontBitmap->rows;
	UChar ** bitmap = new UChar*[ySize];
	for(int i = 0; i < ySize; i++) bitmap[i] = new UChar[xSize];

	for(int i = 0; i < ySize; i++)
		for(int j = 0; j < xSize; j++)
			bitmap[i][j] = 0;

	for(int i = 0; i < xSize; i++)
		for(int j = 0; j < ySize; j++)
			bitmap[ySize - j - 1][i] |= fontBitmap->buffer[(j)*xSize + (i)];
	return bitmap;
}

GraphicCharacter * CharacterCatalogue::getCharacterByID(Uint ID)
{
	if(_characterList.size() == 0) return new GraphicCharacter();
	if(ID >= _characterList.size()) return _characterList.at(_characterList.size() - 1);
	return _characterList.at(ID);
}

GraphicCharacter * CharacterCatalogue::getCharacterByValue(UChar selectedValue)
{
	if(_characterList.size() == 0)
		return new GraphicCharacter();
	int ID = 0;
	while(ID < _characterList.size() && _characterList.at(ID)->getActualValue() != selectedValue)
		ID++;
	return _characterList.at(ID);
}

Uint CharacterCatalogue::catalogueSize(void) const
{
	return _characterList.size();
}

void CharacterCatalogue::_changeID(Uint ID) {_ID = ID;}

#endif