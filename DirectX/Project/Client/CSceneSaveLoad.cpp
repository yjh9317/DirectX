#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Script/CScriptMgr.h>


void CSceneSaveLoad::SaveScene(CScene* _pScene, const wstring& _strSceneFilePath)
{
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
    const vector<CScript*>& vecScript = _pObj->GetScripts();

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

        // Layer ���� ������Ʈ ����
        size_t iObjCount = 0;
        fread(&iObjCount, sizeof(size_t), 1, pFile);

        // Root ������Ʈ
        for (size_t i = 0; i < iObjCount; ++i)
        {
            CGameObject* pLoadObj = LoadGameObject(pFile);
            pLayer->AddObject(pLoadObj);
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



    return pLoadObj;
}
