#pragma once
#include "UI.h"

class TreeUI;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;       // 리소스를 관리하기 위한 트리UI의 주소

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ResourceUI();
    ~ResourceUI();
};

