#pragma once
#include "ResInfoUI.h"

class TextureUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;         // 부모클래스 ResInfoUI의 update가 없으므로 부모의 부모인 UI의 update가 호출
    virtual void render_update() override;


public:
    TextureUI();
    ~TextureUI();
};

