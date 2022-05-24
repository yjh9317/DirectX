#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(Vec2(0.f, 100.f));
}

TransformUI::~TransformUI()
{
}

void TransformUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;

	CTransform* pTransform = pTargetObj->Transform();

	m_vRelativePos = pTransform->GetRelativePos();
	m_vRelativeScale = pTransform->GetRelativeScale();
	m_vRelativeRot = pTransform->GetRelativeRotation();
}

void TransformUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CTransform* pTrans = pTargetObject->Transform();
	Vec3 vPos = pTrans->GetRelativePos();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRotation();
	vRot.ToDegree(); // 현재 회전은 라디안단위 이므로 각도로 바꿔서 수정하기 편하게 하고 세팅할때는 다시 라디안으로 설정 (toRadian)


	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::Text("Relative Position");
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", vPos);
	pTrans->SetRelativePos(vPos);

	ImGui::Text("Relative Scale   ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Scale", vScale);
	pTrans->SetRelativeScale(vScale);

	ImGui::Text("Relative Rotation");
	ImGui::SameLine();
	ImGui::InputFloat3("##Rotation", vRot);
	vRot.ToRadian();
	pTrans->SetRelativeRotation(vRot);
}