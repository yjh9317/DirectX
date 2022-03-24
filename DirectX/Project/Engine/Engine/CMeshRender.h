#pragma once
#include "CComponent.h"

class CMeshRender :
    public CComponent
{
private:
    class CMesh*             m_pMesh;
    class CGraphicsShader*   m_pShader;
    
public:
    void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
    void SetShader(CGraphicsShader* _pShader) { m_pShader = _pShader; }

    CMesh* GetMesh() { return m_pMesh; }
    CGraphicsShader* GetShader() { return m_pShader; }

public:
    virtual void finalupdate() override;
    virtual void render() override;


public:
    CMeshRender();
    ~CMeshRender();
};

