#pragma once
#include "CComponent.h"

#include "CMesh.h"      //템플릿은 선언이 아닌 직접적인 헤더참조가 필요
#include "CMaterial.h"

class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh> m_pMesh;
    Ptr<CMaterial> m_pMtrl;

public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
    void SetMaterial(Ptr<CMaterial> _pMtrl) { m_pMtrl = _pMtrl; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }

public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)

public:
    CMeshRender();
    ~CMeshRender();
};

