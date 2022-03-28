#pragma once
#include "CEntity.h"

//  Scene은 여러가지 Layer를 가지고 Layer는 해당 Layer에 있는 Object들을 update한다.

class CLayer;
class CGameObject;

class CScene :          
    public CEntity
{
private:
    CLayer*      m_arrLayer[MAX_LAYER];

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();

    void render();

public:
    void SetLayerName(int _iLayerIdx,const wstring& _strName);
    void AddObject(CGameObject* _pObj, const wstring& _strLayerName);   
    void AddObject(CGameObject* _pObj, int _iLayerIdx);     //Layer에 있는 vector[Idx]에 오브젝트를 추가

private:
    int GetLayerIdxFromName(const wstring& _strName); //레이어의 인덱스를 가져온다


public:
    CScene();
    ~CScene();
};

