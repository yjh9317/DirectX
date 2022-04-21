#pragma once
#include "CComponent.h"

#include "CMesh.h"      //템플릿은 선언이 아닌 직접적인 헤더참조가 필요
#include "CMaterial.h"

// 렌더링 컴포넌트는 각 오브젝트마다 하나씩 갖고 있는 형태이므로 중간 부모로 CRenderComponent로 관리
// 다른 오브젝트들도 재질과 메쉬를 가져야하기 때문에 MeshRender에 있는 변수와 함수를 여기로 가져옴

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;

    Ptr<CMaterial>  m_pMtrl;        // 현재 사용중인 재질
    Ptr<CMaterial>  m_pSharedMtrl;  // 공유 재질
    Ptr<CMaterial>  m_pDynamicMtrl; // 별도로 가지는 재질

public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetSharedMaterial(Ptr<CMaterial> _pMtrl);


    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();


public:
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};

