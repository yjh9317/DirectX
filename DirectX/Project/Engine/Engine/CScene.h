#pragma once
#include "CEntity.h"

//  Scene�� �������� Layer�� ������ Layer�� �ش� Layer�� �ִ� Object���� update�Ѵ�.

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
    void AddObject(CGameObject* _pRootObj, int _iLayerIdx);   //Layer�� �ִ� vector[Idx]�� ������Ʈ�� �߰�
    CLayer* GetLayer(int _iIdx) { assert(!(_iIdx < 0 || MAX_LAYER <= _iIdx));  return m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

private:
    int GetLayerIdxFromName(const wstring& _strName); //���̾��� �ε����� �����´�

    CLONE(CScene)

public:
    CScene();
    ~CScene();
};

