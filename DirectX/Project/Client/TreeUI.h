#pragma once
#include "UI.h"

class TreeUI :
    public UI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeUI();
    ~TreeUI();
};

