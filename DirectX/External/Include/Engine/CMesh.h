#pragma once
#include "CRes.h"
class CMesh :       // ������ �̿��� ����,��ü(���ҽ��� ����ؼ� ���)
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;       
    ComPtr<ID3D11Buffer>    m_IB;       

    D3D11_BUFFER_DESC      m_tVBDesc;   
    D3D11_BUFFER_DESC      m_tIBDesc;   

    UINT                    m_iVtxCount;    
    UINT                    m_iIdxCount;    



    //�⺻������ ���ۿ� ���εǴ� �ý��۸޸� �ʵ� �������ش�.
    //�ý��� �޸��ʿ� �ִ� �����Ͱ� �ҽǵǸ� gpu�޸𸮵� �ջ�� �� �ֱ� ����

    void*                   m_pVtxSys; 
    void*                   m_pIdxSys; 

public:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
    virtual void UpdateData() override;
    
    int Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount); //���� ����

    void render();

public:
    CMesh();
    ~CMesh();
};
