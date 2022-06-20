#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Script/CScriptMgr.h>

#include <Engine/CParticleSystem.h>

void CSceneSaveLoad::SaveScene(CScene* _pScene, const wstring& _strSceneFilePath)
{
    // ���ҽ� ������� ����
    CResMgr::GetInst()->SaveChangedRes();

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"wb");

    assert(pFile);
    if (nullptr == pFile)
        return;

    // Scene �̸� ����
    _pScene->SaveToScene(pFile);

    // Scene �� Layer ���� ����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer �̸� ����
        CLayer* pLayer = _pScene->GetLayer(i);
        pLayer->SaveToScene(pFile);

        // Layer �� �������� Root ������Ʈ ���� ����
        vector<CGameObject*>& vecRootObj = pLayer->GetRootObjects();
        size_t iObjCount = vecRootObj.size();
        fwrite(&iObjCount, sizeof(size_t), 1, pFile);

        // �� ��Ʈ������Ʈ ����
        for (size_t i = 0; i < vecRootObj.size(); ++i)
        {
            SaveGameObject(vecRootObj[i], pFile);
        }
    }


    fclose(pFile);
}

void CSceneSaveLoad::SaveGameObject(CGameObject* _pObj, FILE* _pFile)
{
    // �̸�, ����, ������Ʈ ����
    _pObj->SaveToScene(_pFile);

    // Script ����
    //const vector<CScript*>& vecScript = _pObj->GetScripts();


    // Child Object
    const vector<CGameObject*>& vecChild = _pObj->GetChild();
    size_t iChildCount = vecChild.size();
    fwrite(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _pFile);
    }

}

CScene* CSceneSaveLoad::LoadScene(const wstring& _strSceneFilePath)
{
    CScene* pLoadScene = new CScene;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"rb");

    assert(pFile);
    if (nullptr == pFile)
        return nullptr;

    // Scene �̸� �ҷ�����
    pLoadScene->LoadFromScene(pFile);

    // Scene �� Layer ���� �ҷ�����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer �� �̸� 
        CLayer* pLayer = pLoadScene->GetLayer(i);
        pLayer->LoadFromScene(pFile);

        // Layer ���� ������Ʈ ����
        size_t iObjCount = 0;
        fread(&iObjCount, sizeof(size_t), 1, pFile);

        // Root ������Ʈ
        for (size_t j = 0; j < iObjCount; ++j)
        {
            CGameObject* pLoadObj = LoadGameObject(pFile);
            pLoadScene->AddObject(pLoadObj, i);
        }
    }

    fclose(pFile);

    return pLoadScene;
}

CGameObject* CSceneSaveLoad::LoadGameObject(FILE* _pFile)
{
    CGameObject* pLoadObj = new CGameObject;

    // �̸�, ����, ������Ʈ �ҷ�����
    pLoadObj->LoadFromScene(_pFile);

    // Script �ҷ�����    


    size_t iChildCount = 0;
    fread(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        CGameObject* pChild = LoadGameObject(_pFile);
        pLoadObj->AddChild(pChild);
    }

    return pLoadObj;
}
