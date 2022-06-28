#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CSceneSaveLoad.h>

ResourceUI::ResourceUI()
	: UI("Resource")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	m_TreeUI->UseFrame(true);
	m_TreeUI->UseDragDropOuter(true);

	AddChild(m_TreeUI);

	// Clicked Delegate ���
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&ResourceUI::ItemClicked);

	// DoubleClicked Deletage ���
	m_TreeUI->SetDoubleClickedDelegate(this, (CLICKED)&ResourceUI::ItemDBClicked);

	Reset();
}

ResourceUI::~ResourceUI()
{
}

void ResourceUI::update()
{
	// ���ҽ� ������� ����
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		CResMgr::GetInst()->SaveChangedRes();
	}

	UI::update();
}

void ResourceUI::render_update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Renew();
	}
}

void ResourceUI::Reset()
{
	// Content ���� ���� ���ҽ� �ε�
	Reload();

	Renew();
}

void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// ������ ��尡 ���ȴٸ� �ƹ��ϵ� ����.
	if (nullptr == pResource)
		return;

	// InspectorUI �� ����
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetResource(pResource);
}

void ResourceUI::ItemDBClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// ������ ��尡 ���ȴٸ� �ƹ��ϵ� ����.
	if (nullptr == pResource || pNode->GetParent()->GetName() != "SCENEFILE")
		return;

	// Scene �ε�
	// ���� Scene ����
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetSceneState(SCENE_STATE::STOP);

	// �ε��� Scene ������ ��� ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += pResource->GetRelativePath();

	CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
	CSceneMgr::GetInst()->ChangeScene(pNewScene);
}



void ResourceUI::Reload()
{
	// Content ���� ������ ��� ������ ����θ� �˾Ƴ���.
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());

	// Ű�� Ȯ��
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		// 1. ���ϸ��� ���ؼ� ���ҽ��� ������ �˾Ƴ���.
		RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);

		// 2. ����θ� Ű������ �ؼ� ���ҽ��Ŵ����� �ε��Ѵ�.
		switch (eType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESHDATA:

			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESH:

			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:

			break;
		case RES_TYPE::SCENEFILE:
			CResMgr::GetInst()->Load<CSceneFile>(m_vecResPath[i], m_vecResPath[i]);
			break;
		}
	}

	// 3. �ε��Ǿ��ִ� ���ҽ����� ���� ���� ���� Ȯ��
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);
		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			// File Exist üũ
			if (!filesystem::exists(strContentPath + pair.second->GetRelativePath()))
			{
				// 4. ������ ���ҽ��Ŵ������� �޸� ����
				MessageBox(nullptr, L"�������� ���� ��", L"���Ϻ��� ����", MB_OK);

				if (0 == pair.second->GetRefCount())
				{
					// ����
					tEventInfo info;
					info.eType = EVENT_TYPE::DELETE_RES;
					info.lParam = (DWORD_PTR)pair.second;
					CEventMgr::GetInst()->AddEvent(info);

					MessageBox(nullptr, L"���ҽ� �޸� ����", L"���Ϻ��� ����", MB_OK);
				}

				else
				{
					MessageBox(nullptr, L"��� ���� ���ҽ�\n �޸� ���� ����", L"���Ϻ��� ����", MB_OK);
				}
			}
		}
	}

}

void ResourceUI::Renew()
{
	m_TreeUI->Clear();

	// ���ҽ��� Tree �� ǥ��
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		// TreeUI �� DummyRoot �� ����ϱ� ������, ���ҽ� �׸� ������ ���� �ڽ����� ��
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString((RES_TYPE)i));

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			// �� ���ҽ� ������ �ش� ���ҽ� �׸� �ڽ����� ��
			m_TreeUI->AddTreeNode(pResNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second);
		}
	}
}

void ResourceUI::FindFileName(const wstring& _strFolderPath)
{
	wstring strContent = _strFolderPath + L"*.*";

	WIN32_FIND_DATA FindFileData = {}; //�˻��� ���ϼӼ��� ���� ������ ��� ����ü

	HANDLE hFind = nullptr;

	hFind = FindFirstFile(strContent.c_str(), &FindFileData);	// FindFirstFileFindFirstFile : ���� �˻��� �����Ѵ�. 
																// ���� �˻� ���ڿ��� �߸��Ǿ��ų� �ٸ� ������ �Լ� ������ �������� ��� INVALID_HANDLE_VALUE�� Define�� ������� ��ȯ�Ѵ�.
																// �������� ��� ���� �˻� �ڵ��� ��ȯ�ϰ� �� ��°�� ���޵Ǵ� ����ü�� ù ��°�� �߰ߵǴ� ������ ������ �־��ش�.

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindFileData))
	{
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (0 == wcscmp(FindFileData.cFileName, L".."))	// ���Ͼȿ� ������ �ʴ� ..���� ����
				continue;

			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
			continue;
		}

		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
		strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);

		m_vecResPath.push_back(strRelativePath);
	}

	FindClose(hFind);
}


RES_TYPE ResourceUI::GetResTypeFromExt(const wstring& _strExt)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_strExt.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50); //_wsplitpath_s : ��� �̸��� ���� ��ҷ� �����ϴ� �Լ�, �� �Լ��� �̿��� ������ Ȯ���ڸ��� �����´�.

	wstring strExt = szExt;

	if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpeg" == strExt || L".bmp" == strExt
		|| L".jpg" == strExt || L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".ogg" == strExt)
		return RES_TYPE::SOUND;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".scene" == strExt)
		return RES_TYPE::SCENEFILE;

	return RES_TYPE::END;
}