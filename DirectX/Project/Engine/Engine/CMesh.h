#pragma once
#include "CRes.h"
class CMesh :       // 정점을 이용한 도형,물체(리소스로 취급해서 사용)
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    ComPtr<ID3D11Buffer>    m_IB;

    D3D11_BUFFER_DESC      m_tVBDesc;
    D3D11_BUFFER_DESC      m_tIBDesc;

    UINT                    m_iVtxCount;
    UINT                    m_iIdxCount;



    //기본적으로 버퍼와 맵핑되는 시스템메모리 쪽도 관리해준다.
    //시스템 메모리쪽에 있는 데이터가 소실되면 gpu메모리도 손상될 수 있기 때문

    void* m_pVtxSys;
    void* m_pIdxSys;

public:
    Vtx* GetVtxSysMem() { return (Vtx*)m_pVtxSys; }


public:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
    virtual void UpdateData() override;

    int Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount); //버퍼 생성

    void render();
    void render_particle(UINT _iCount);

public:
    CMesh();
    ~CMesh();
};

