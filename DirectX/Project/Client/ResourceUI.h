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
    void Reset();

private:
    void ItemClicked(DWORD_PTR _dwNode);
    void ItemDBClicked(DWORD_PTR _dwNode);
public:
    ResourceUI();
    ~ResourceUI();
};

