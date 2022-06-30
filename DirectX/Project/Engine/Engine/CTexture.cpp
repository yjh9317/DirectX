 #include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
    : CRes(RES_TYPE::TEXTURE)
{
}


CTexture::~CTexture()
{
}


int CTexture::Load(const wstring& _strFilePath)
{
    // Texture File을 System Memory에 로드
    // 어떤 포맷(확장자) 파일이든 압축을 해제해서 픽셀 데이터를 받아오는 함수

    wchar_t szExt[25] = {};

    //확장자명을 szExt에 저장
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 25);

    HRESULT hr = S_OK;
    int iErr = 0;

    if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS")) // dds
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);


    }
    else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))    //tga
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }
    else // WIC : Window Image Component (png, jpg, jpeg ,bmp ....) 
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (CheckFail(hr))
        return E_FAIL;

    // System memory에 불러와진 픽셀 데이터를 GPU에 보낸다.

    // Device , 이미지, 이미지 개수 , 이미지에 대한 정보, 최종적으로 받을 쉐이더 리소스 뷰

    // 원래라면 리소스는 View가 있어야 생성이 가능하지만 DirectXTex라는 라이브러리 함수에서 리소스와 View를 동시에 생성해주는 함수를 사용

    hr = CreateShaderResourceView(DEVICE, m_Image.GetImages() //Texture 2D, ShaderResourceView를 생성하는 함수
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_pSRV.GetAddressOf());


    if (CheckFail(hr))
        return E_FAIL;


    m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf()); // Texture2D에서 ResourceView를 가져오는 함수

    m_pTex2D->GetDesc(&m_tDesc);

    return S_OK;
}

int CTexture::Save(const wstring& _strFilePath)
{
    CaptureTexture(DEVICE, CONTEXT, m_pTex2D.Get(), m_Image);
    //SaveToWICFile(m_Image, WIC_FLAGS_FORCE_RGB, nullptr, _strFilePath.c_str());

    CRes::Save(_strFilePath);
    return S_OK;
}

void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag)
{
    // Texture 만들기
    
    m_tDesc.Width = _iWidth;
    m_tDesc.Height = _iHeight;
    m_tDesc.MipLevels = 1;
    m_tDesc.ArraySize = 1;

    m_tDesc.CPUAccessFlags = 0;
    m_tDesc.BindFlags = _flag; // 받아온 인자로 쉐이더나 UAV로 설정
    m_tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

    
    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Format =_format;

    DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
    assert(m_pTex2D);

    if (D3D11_BIND_DEPTH_STENCIL & _flag) // Depth Stencil은 호환되지 않으므로 따로 플래그를 확인해서 View 생성
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else //플래그를 확인하고 각각 맞는 View를 생성
    {
        if (D3D11_BIND_RENDER_TARGET & _flag)  // 렌더타겟
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & _flag) //쉐이더 리소스
        {
            //SRV 생성 Shader Resource View
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), nullptr, m_pSRV.GetAddressOf());
            assert(m_pSRV);
        }

        if (D3D11_BIND_UNORDERED_ACCESS & _flag) //순서없는 접근
        {
            //UAV 생성 Unordered Access View
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D; //원본 리소스가 Texture 2D
            tUAVDesc.Texture2D.MipSlice = 0;

            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }



    
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _pTex2D)
{
    // 전달받은 Texture(_pTex2D)를 바탕으로 View를 생성
    m_pTex2D = _pTex2D;
    m_pTex2D->GetDesc(&m_tDesc); //Desc로 정보 확인

    if (D3D11_BIND_DEPTH_STENCIL & m_tDesc.BindFlags) // Depth Stencil은 호환되지 않으므로 따로 플래그를 확인해서 View 생성
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else //플래그를 확인하고 각각 맞는 View를 생성
    {
        if (D3D11_BIND_RENDER_TARGET & m_tDesc.BindFlags)  // 렌더타겟
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_tDesc.BindFlags) //쉐이더 리소스
        {
            //SRV 생성 Shader Resource View
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), nullptr, m_pSRV.GetAddressOf());
            assert(m_pSRV);
        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_tDesc.BindFlags) //순서없는 접근
        {
            //UAV 생성 Unordered Access View
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D; //원본 리소스가 Texture 2D
            tUAVDesc.Texture2D.MipSlice = 0;

            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }


}

// 레지스터 바인딩
void CTexture::UpdateData(UINT _PipelineStage, UINT _iRegisterNum)
{
    if (_PipelineStage & (UINT)PIPELINE_STAGE::VS)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::HS)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::DS)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::GS)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::PS)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}

void CTexture::UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource)
{
    if (_bShaderResource)
    {
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }
    else
    {
        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_pUAV.GetAddressOf(), &i);
    }
}

void CTexture::Clear(int _iRegisterNum)
{
    // 레지스터에 nullptr
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::ClearCS(int _iRegisterNum)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pUAV,&i);
    CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
}
