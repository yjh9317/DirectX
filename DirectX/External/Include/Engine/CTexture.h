#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h> //���� ����(Ȯ����)���� �̹����� ���������� �����ִ� ���̺귯��

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // ���Ϸκ��� �ε��� �ؽ��� �������� �ý��� �޸� ����
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU �� �ε��� �ؽ��� �����͸� �����ϴ� �������̽�

    // �ϳ��� �ؽ��ķ� �뵵�� �°� View�� ����
    // SRV,UAV,RTV �� ���� ȣȯ�� ������ DSV�� ȣȯ���� ����. ������ �� DSV�� ���� ����Ѵ�, ���̾��� ����

    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D �� ShaderResource �뵵�� �����ϴ� �߰� �Ű�ü
    ComPtr<ID3D11UnorderedAccessView>   m_pUAV;     // Texture2D�� RwTexture �뵵�� �����ϴ� View
    ComPtr<ID3D11RenderTargetView>      m_pRTV;     // Texture2D �� RenderTarget �뵵�� ����
    ComPtr<ID3D11DepthStencilView>      m_pDSV;     // Texture2D �� DepthStencil �뵵�� ���� 
   
    D3D11_TEXTURE2D_DESC                m_tDesc;    // Load�Լ� ���� CreateShaderResourceView�Լ��� Desc�� ä������ �ű⼭ �����ͼ� ����.

protected:
    virtual int Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight,DXGI_FORMAT _format,UINT _flag); // ����,����,�ȼ�����,�÷��� .��ü������ �̹���Create
    void Create(ComPtr<ID3D11Texture2D> _pTex2D);

public:
    // ScartchImage������ ������ �� ������ gpu�޸𸮿� ���̷�Ʈ�� �ؽ��ĸ� ����� ����ϸ� �������� ����
    float Width() { return (float)m_tDesc.Width; }
    float Height() { return (float)m_tDesc.Height; }

    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_pSRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_pUAV; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_pRTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_pDSV; }
    
public:
    // PIPELINE_STAGE
    void UpdateData(UINT _PipelineStage, int _iRegisterNum);

    void UpdateData_CS(int _iRegisterNum); //Compute Shader���� �������Ϳ� ���ε��� �Լ�
    static void Clear(int _iRegisterNum);
    static void ClearCS(int _iRegisterNum);

public:
    CTexture();
    ~CTexture();

    friend class CResMgr;
};

