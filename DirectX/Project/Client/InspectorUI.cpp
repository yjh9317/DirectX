#include "pch.h"
#include "InspectorUI.h"

#include <Engine/CKeyMgr.h>

InspectorUI::InspectorUI()
	: UI("Inspector")
{

}

InspectorUI::~InspectorUI()
{
}

void InspectorUI::update()
{
	if (KEY_TAP(KEY::I))
	{
		if (IsActive())
			Deactivate();
		else
			Activate();
	}
}

void InspectorUI::render_update()
{

}