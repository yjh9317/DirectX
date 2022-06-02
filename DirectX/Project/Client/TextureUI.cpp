#include "pch.h"
#include "TextureUI.h"


TextureUI::TextureUI()
	: ResInfoUI("Texture", RES_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}


void TextureUI::update()
{
	// To Do




	ResInfoUI::update();	//update는 가장 마지막에
}

void TextureUI::render_update()
{
	ResInfoUI::render_update();	//render_update는 가장 먼저

	// To Do


}

