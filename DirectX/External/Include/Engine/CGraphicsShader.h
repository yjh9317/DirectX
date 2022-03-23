#pragma once
#include "CShader.h"
class CGraphicsShader : //��� ���̴��� �����ϴ� ������ Ŭ�������� ��� ���
    public CShader
{
private:
    static vector<D3D11_INPUT_ELEMENT_DESC> g_vecLayout; // ���� ���̾ƿ��� ��� ��ü���� ����� �� �ֵ��� ����(static)
    static UINT g_iOffset;

public:
    static void AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc);

private:
    ComPtr<ID3DBlob>                m_VSBlob;   
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;           //���ؽ�
    ComPtr<ID3D11HullShader>        m_HS;           //��
    ComPtr<ID3D11DomainShader>      m_DS;           //������
    ComPtr<ID3D11GeometryShader>    m_GS;           //������Ʈ��
    ComPtr<ID3D11PixelShader>       m_PS;           //�ȼ�

    ComPtr<ID3D11InputLayout>       m_InputLayout;  //������ ���� ����

    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;    //�������� ���� ����

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFunc);
    //void CreateHullShader();
    //void CreateDomainShader();
    //void CreateGeometryShader();
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFunc);

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }
    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_eTopology; }

    virtual void UpdateData() override;


public:
    CGraphicsShader();
    ~CGraphicsShader();
};

