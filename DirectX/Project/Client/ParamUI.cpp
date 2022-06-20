#include "pch.h"
#include "ParamUI.h"

#include "ImGui/imgui.h"
#include <Engine/CResMgr.h>
#include "CImGuiMgr.h"



int ParamUI::KeyCount = 0;


int ParamUI::Param_Int(const string& _strName, const int* _pInOut)
{
	// 파라미터 이름	
	ImGui::Text(_strName.c_str());
	ImGui::SameLine(100);

	char szKey[255] = {};
	sprintf_s(szKey, 255, "##Param%d", KeyCount++);

	int data = *_pInOut;
	ImGui::InputInt(szKey, &data);

	return data;
}

float ParamUI::Param_Float(const string& _strName, const float* _pInOut)
{
	// 파라미터 이름	
	ImGui::Text(_strName.c_str());
	ImGui::SameLine(100);

	char szKey[255] = {};
	sprintf_s(szKey, 255, "##Param%d", KeyCount++);

	float data = *_pInOut;
	ImGui::InputFloat(szKey, &data);

	return data;
}

Vec2 ParamUI::Param_Vec2(const string& _strName, const Vec2* _pInOut)
{
	// 파라미터 이름	
	ImGui::Text(_strName.c_str());
	ImGui::SameLine(100);

	char szKey[255] = {};
	sprintf_s(szKey, 255, "##Param%d", KeyCount++);

	float arrFloat2[2] = { _pInOut->x, _pInOut->y };

	ImGui::InputFloat2(szKey, arrFloat2);

	return Vec2(arrFloat2[0], arrFloat2[1]);
}

Vec4 ParamUI::Param_Vec4(const string& _strName, const Vec4* _pInOut)
{
	// 파라미터 이름
	ImGui::Text(_strName.c_str());
	ImGui::SameLine(100);

	char szKey[255] = {};
	sprintf_s(szKey, 255, "##Param%d", KeyCount++);

	Vec4 data = *_pInOut;

	ImGui::InputFloat4(szKey, data);

	return data;
}

bool ParamUI::Param_Tex(const string& _strName, CTexture* _pCurTex, UI* _pInst, DBCLKED _pFunc)
{
	ImGui::Text(_strName.c_str());
	ImGui::SameLine(100);

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

	ImTextureID texid = 0;
	if (nullptr != _pCurTex)
	{
		texid = (ImTextureID)_pCurTex->GetSRV().Get();
	}

	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	bool DragDropSuccess = false;	// DragDrop 체크
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource")) // Accept~함수는 마우스클릭을 놓았을 때 Payload를 반환
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));	//payload에 저장했던 데이터를 dwData에 복사
			CRes* pRes = (CRes*)dwData;
			if (RES_TYPE::TEXTURE == pRes->GetResType())	//Texture 리소스만 허용
			{
				static string strKey;
				strKey = string(pRes->GetKey().begin(), pRes->GetKey().end());

				CImGuiMgr::GetInst()->AddDelegate(tUIDelegate{ _pInst , _pFunc , (DWORD_PTR)strKey.c_str() });	//Delegate 추가
				DragDropSuccess = true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (DragDropSuccess)
	{
		return true;	// true값이 반환되어야 MaterialUI에서 Tex번호를 저장.
	}


	ImGui::SameLine();


	// List UI 활성화
	char szKey[255] = {};
	sprintf_s(szKey, 255, "##Param%d", KeyCount++);

	if (ImGui::Button(szKey, Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Texture List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(_pInst, _pFunc);

		return true;
	}

	return false;
}

