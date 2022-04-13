#pragma once
#include "CComponent.h"

#include "CMesh.h"      //���ø��� ������ �ƴ� �������� ��������� �ʿ�
#include "CMaterial.h"

// SharedMtrl,DynamicMtrl

// ������Ʈ���� �ڱ⸸�� ������ ������ �ִ� ���·� ����� ������(�޸𸮵� �׷��� ũ�� ����) ����ȭ�� ���ؼ� �׷��� ������ ����.
// ���������������ο��� �߻��ϴ� Draw call�� ���̱�����(cpu�� gpu���� �޸� ��������� ŭ)
// �������� �� �������� ���� ������ IA�ܰ迡�� ���̴��� ���� ����, Material(������۸� �ű�� �������� ���ε�)��  ������Ʈ �ϳ� ȣ���� ������ �غ��������.
// 
// 2D������ ���� ���͸���� ������ �� ������ �� �� ����.
// 3D������ instancing�� ���� A�� B�� ���ӿ�����Ʈ�� �׸��� ������ ���������ΰ����� �ִµ� ���� ���͸����� ����ϰ�������
// �߰��� ���������� ����ϴ� ������Ʈ���� ��� �ϰ�ó���ϴ� ���� instancing(�޽��� ���ƾ���)

class CMeshRender :
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
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();

public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)

public:
    CMeshRender();
    CMeshRender(const CMeshRender& _meshrender);
    ~CMeshRender();
};

