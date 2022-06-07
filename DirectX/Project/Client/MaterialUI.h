#pragma once
#include "ResInfoUI.h"

class MaterialUI :
    public ResInfoUI
{
private:
    TEX_PARAM   m_eSelectedTexParam;    // ���õ� Tex ��ȣ

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void TextureSelected(DWORD_PTR _ptr); // Delegate

public:
    MaterialUI();
    ~MaterialUI();
};

