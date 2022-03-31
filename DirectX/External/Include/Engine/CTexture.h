#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>  //���� ����(Ȯ����)���� �̹����� ���������� �����ִ� ���̺귯��

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
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU�� �ε��� �ؽ��� �����͸� �����ϴ� �������̽�
    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D�� ShaderResource �뵵�� �����ϴ� �߰� �Ű�ü

public:
    virtual int Load(const wstring& _strFilePath);

public:
    CTexture();
    ~CTexture();
};