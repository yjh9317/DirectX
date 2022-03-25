#pragma once
#include "CEntity.h"

//  Scene�� �������� ���̾ ���� ���·� Scene�� ������
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
    void AddObject(CGameObject* _pObj, const wstring& _strLayerName);   //
    void AddObject(CGameObject* _pObj, int _iLayerIdx);     //Layer�� �ִ� vector[Idx]�� ������Ʈ�� �߰�

private:
    int GetLayerIdxFromName(const wstring& _strName); //���̾��� �ε����� �����´�


public:
    CScene();
    ~CScene();
};

