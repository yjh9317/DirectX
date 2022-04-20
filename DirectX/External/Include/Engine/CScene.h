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

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();

    void render();

public:
    void SetLayerName(int _iLayerIdx, const wstring& _strName);
    void AddObject(CGameObject* _pRootObj, const wstring& _strLayerName);
    void AddObject(CGameObject* _pRootObj, int _iLayerIdx);   //Layer에 있는 vector[Idx]에 오브젝트를 추가
    CLayer* GetLayer(int _iIdx) { assert(!(_iIdx < 0 || MAX_LAYER <= _iIdx));  return m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

private:
    int GetLayerIdxFromName(const wstring& _strName); //레이어의 인덱스를 가져온다

    CLONE(CScene)

public:
    CScene();
    ~CScene();
};

