#pragma once
#include "UI.h"

class TreeUI;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;       // ���ҽ��� �����ϱ� ���� Ʈ��UI�� �ּ�

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ResourceUI();
    ~ResourceUI();
};

