#pragma once
#include "CComponent.h"

#include "CMesh.h"      //���ø��� ������ �ƴ� �������� ��������� �ʿ�
#include "CMaterial.h"

// ������ ������Ʈ�� �� ������Ʈ���� �ϳ��� ���� �ִ� �����̹Ƿ� �߰� �θ�� CRenderComponent�� ����
// �ٸ� ������Ʈ�鵵 ������ �޽��� �������ϱ� ������ MeshRender�� �ִ� ������ �Լ��� ����� ������

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;

    Ptr<CMaterial>  m_pMtrl;        // ���� ������� ����
    Ptr<CMaterial>  m_pSharedMtrl;  // ���� ����
    Ptr<CMaterial>  m_pDynamicMtrl; // ������ ������ ����

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

