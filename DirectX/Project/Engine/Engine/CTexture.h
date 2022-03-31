#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>  //여러 포맷(확장자)들을 이미지로 압축해제를 도와주는 라이브러리

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // 파일로부터 로딩한 텍스쳐 데이터의 시스템 메모리 관리
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU에 로딩된 텍스쳐 데이터를 관리하는 인터페이스
    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D를 ShaderResource 용도로 전달하는 중간 매개체

public:
    virtual int Load(const wstring& _strFilePath);

public:
    CTexture();
    ~CTexture();
};