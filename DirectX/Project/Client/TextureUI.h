#pragma once
#include "ResInfoUI.h"

class TextureUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;         // �θ�Ŭ���� ResInfoUI�� update�� �����Ƿ� �θ��� �θ��� UI�� update�� ȣ��
    virtual void render_update() override;


public:
    TextureUI();
    ~TextureUI();
};

