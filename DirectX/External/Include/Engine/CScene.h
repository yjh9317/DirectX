#pragma once
#include "CEntity.h"

//  Scene은 여러가지 Layer를 가지고 Layer는 해당 Layer에 있는 Object들을 update한다.

class CLayer;
class CGameObject;

class CScene :
    public CEntity
{
private:
    CLayer* m_arrLayer[MAX_LAYER];
    SCENE_STATE     m_eSceneState;  //현재 Scene의 상태로 게임을 Play 혹은 Stop 할 수 있음.

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();


public:
    void SetLayerName(int _iLayerIdx, const wstring& _strName);
    void AddObject(CGameObject* _pRootObj, const wstring& _strLayerName);
    void AddObject(CGameObject* _pRootObj, int _iLayerIdx);   //Layer에 있는 vector[Idx]에 오브젝트를 추가
    CLayer* GetLayer(int _iIdx) { assert(!(_iIdx < 0 || MAX_LAYER <= _iIdx));  return m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

    SCENE_STATE GetSceneState() { return m_eSceneState; }
    void SetSceneState(SCENE_STATE _eState) { m_eSceneState = _eState; }


private:
    int GetLayerIdxFromName(const wstring& _strName); //레이어의 인덱스를 가져온다

    CLONE(CScene)

public:
    CScene();
    ~CScene();
};

