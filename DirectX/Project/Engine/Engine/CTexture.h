#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h> //여러 포맷(확장자)들을 이미지로 압축해제를 도와주는 라이브러리

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
    ComPtr<ID3D11Texture2D>             m_pTex2D;   // GPU 에 로딩된 텍스쳐 데이터를 관리하는 인터페이스
    ComPtr<ID3D11ShaderResourceView>    m_pSRV;     // Texture2D 를 ShaderResource 용도로 전달하는 중간 매개체
    D3D11_TEXTURE2D_DESC                m_tDesc;    // Load함수 안의 CreateShaderResourceView함수로 Desc가 채워졌고 거기서 가져와서 저장.

protected:
    virtual int Load(const wstring& _strFilePath);

public:
    // ScartchImage에서도 가져올 수 있지만 gpu메모리에 만들어서 다이렉트로 사용하면 없을수도 있음
    float Width() { return (float)m_tDesc.Width; }
    float Height() { return (float)m_tDesc.Height; }

public:
    // PIPELINE_STAGE
    void UpdateData(UINT _PipelineStage, int _iRegisterNum);
    static void Clear(int _iRegisterNum);

public:
    CTexture();
    ~CTexture();
};

