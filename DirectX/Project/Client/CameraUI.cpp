#include "pch.h"
#include "CameraUI.h"


#include <Engine/CCamera.h>

CameraUI::CameraUI()
    : ComponentUI("Camera", COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::update()
{
}

void CameraUI::render_update()
{
    ComponentUI::render_update();

    CCamera* pCameraCom = GetTargetObject()->Camera();

    // PROJECTION TYPE
    ImGui::Text("Projection Type");
    ImGui::SameLine();

    PROJ_TYPE eType = pCameraCom->GetProjType();

    const char* szProjType[2] = { "ORTHOGRAPHIC", "PERSPECTIVE" };

    if (ImGui::BeginCombo("##ProjType", szProjType[(int)eType]))
    {
        for (int i = 0; i < 2; i++)
        {
            if (ImGui::Selectable(szProjType[i]))
            {
                pCameraCom->SetProjType((PROJ_TYPE)i);
            }
        }

        ImGui::EndCombo();
    }

}
