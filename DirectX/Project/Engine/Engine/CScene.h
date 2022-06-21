#pragma once
#include "CEntity.h"

//  Scene�� �������� Layer�� ������ Layer�� �ش� Layer�� �ִ� Object���� update�Ѵ�.
#include "CSceneFile.h"

class CLayer;
class CGameObject;

class CScene :
    public CEntity
{
private:
    CLayer* m_arrLayer[MAX_LAYER];
    SCENE_STATE     m_eSceneState;  //���� Scene�� ���·� ������ Play Ȥ�� Stop �� �� ����.
    wstring         m_strResKey;
  

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();


public:
    void SetLayerName(int _iLayerIdx, const wstring& _strName);
    void AddObject(CGameObject* _pRootObj, const wstring& _strLayerName);
    void AddObject(CGameObject* _pRootObj, int _iLayerIdx);   //Layer�� �ִ� vector[Idx]�� ������Ʈ�� �߰�
    CLayer* GetLayer(int _iIdx) { assert(!(_iIdx < 0 || MAX_LAYER <= _iIdx));  return m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

    void SetResKey(const wstring& _strKey) { m_strResKey = _strKey; }
    Ptr<CSceneFile> GetSceneFile();

    SCENE_STATE GetSceneState() { return m_eSceneState; }
    void SetSceneState(SCENE_STATE _eState);


private:
    int GetLayerIdxFromName(const wstring& _strName); //���̾��� �ε����� �����´�

    CLONE(CScene)

public:
    CScene();
    ~CScene();
};

