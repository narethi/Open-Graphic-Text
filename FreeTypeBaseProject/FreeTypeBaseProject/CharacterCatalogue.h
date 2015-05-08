#ifndef CHARACTER_CATALOGUE_H
#define CHARACTER_CATALOGUE_H

#include "stdafx.h"

class GraphicCharacter;

class CharacterCatalogue
{
	private:
		//fontName is name used to identify the catalogue
		string _fontName;
		Uint _ID;
		vector<GraphicCharacter*> _characterList;
		UChar ** _generateAlphaBuffer(FT_Bitmap * currentBitMap, FT_Int glyphTop);
	
	protected:
		friend class TextBox;
		void _changeID(Uint ID);
	public:
		CharacterCatalogue();
		CharacterCatalogue(string fontName, Uint ID);

		//addFont loads up a font file
		//and populates the catalogue with all
		//of the characters in the order they 
		//appear with in the font file
		void addFont(string fontFile, Uint fontSize);
		void addCharacter(string fontFile, UChar specificValue, Uint fontSize);

		GraphicCharacter * getCharacterByID(Uint ID);
		GraphicCharacter * getCharacterByValue(UChar selectedValue);
		GraphicCharacter * getCharacterByName(string charName);

		Uint catalogueSize(void) const;
};

#endif