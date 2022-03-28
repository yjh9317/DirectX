#pragma once
#include "CShader.h"



class CGraphicsShader : //모든 셰이더가 동작하는 과정을 클래스에서 묶어서 사용 ,D12에서는 파이프라인 스테이트 오브젝트
    public CShader
{
private:
    static vector<D3D11_INPUT_ELEMENT_DESC> g_vecLayout; // 같은 레이아웃을 모든 객체에서 사용할 수 있도록 정적(static)
    static UINT g_iOffset;

public:
    static void AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc);

private:
    ComPtr<ID3DBlob>                m_VSBlob;   
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;           //버텍스
    ComPtr<ID3D11HullShader>        m_HS;           //훌
    ComPtr<ID3D11DomainShader>      m_DS;           //도메인
    ComPtr<ID3D11GeometryShader>    m_GS;           //지오메트리
    ComPtr<ID3D11PixelShader>       m_PS;           //픽셀


    RS_TYPE                         m_eRSType;      //래스터라이저 타입
    ComPtr<ID3D11InputLayout>       m_InputLayout;  //정점의 내부 구조
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;    //정점으로 만들 도형

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFunc);
    //void CreateHullShader();
    //void CreateDomainShader();
    //void CreateGeometryShader();
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFunc);

    void SetRSType(RS_TYPE _eType) { m_eRSType = _eType; }
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }
    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_eTopology; }

    virtual void UpdateData() override;


public:
    CGraphicsShader();
    ~CGraphicsShader();
};

