#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }
// C##type -> Ctype을 의미

class CComponent;
class CTransform;
class CCollider2D;
class CAnimator2D;
class CCamera;

class CMeshRender;
class CTileMap;
class CParticleSystem;
class CScript;
class CRenderComponent;

class CGameObject :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecChild;     //자식오브젝트
    vector<CScript*>        m_vecScript;    // 스크립트를 관리하는 벡터

    CComponent*         m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*   m_pRenderComponent; 

    CGameObject*            m_pParent;
    int                     m_iLayerIdx; // 게임 오브젝트 소속 레이어 인덱스
    bool                    m_bActive;
    bool                    m_bDead;

public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();


private:
    void active();
    void deactive();

public:
    CGameObject* GetParent() { return m_pParent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    // Deregister ==> 등록 취소(등록->미등록)
    // Unregister ==> 등록 안됨(등록 x == 등록->미등록, 애초에 등록된적 없음)
    void Deregister();
    void DisconnectBetweenParent();
    void Activate();
    void Deactivate();

    bool IsDead() { return m_bDead; }
    bool IsActive() { return m_bActive; }
    bool IsAncestor(CGameObject* _pObj);

public:
    void AddChild(CGameObject* _pChild);
    void AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    void Destroy();

	GET_COMPONENT(Transform, TRANSFORM)
	GET_COMPONENT(MeshRender, MESHRENDER)
	GET_COMPONENT(Camera, CAMERA)
	GET_COMPONENT(Collider2D, COLLIDER2D)
	GET_COMPONENT(Animator2D, ANIMATOR2D)
	GET_COMPONENT(TileMap, TILEMAP)
	GET_COMPONENT(ParticleSystem, PARTICLESYSTEM)


    const vector<CScript*>& GetScripts() { return m_vecScript; }
    CScript* GetScript(UINT _iIdx);                     //스크립트의 인덱스로 가져옴
    CScript* GetScriptByName(const wstring& _strName);  //스크립트의 이름을 따로 설정하고 그 이름으로 찾는다

    int GetLayerIndex() { return m_iLayerIdx; }


    template<typename T>
    T* GetScript(); 

public:
    CLONE(CGameObject)

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    ~CGameObject();

    friend class CEventMgr;
    friend class CScene;
    friend class CLayer;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        // RTTI인 dynamic_cast로 찾을 경우, 만약 내가 찾고자 하는 스크립트 A와 A를 상속받는 스크립트 B가 있을 때
        // B도 찾고자 하는 스크립트를 상속받았으므로 A가 아닌 B가 찾아와 질 수도 있다.
        // 이 문제점은 Script에서 const int로 고유값을 받아 구분
        T* pScript = dynamic_cast<T*>(m_vecScript[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
