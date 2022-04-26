#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
{

}

CTexture::~CTexture()
{
}


int CTexture::Load(const wstring& _strFilePath)
{
    // Texture File�� System Memory�� �ε�
    // � ����(Ȯ����) �����̵� ������ �����ؼ� �ȼ� �����͸� �޾ƿ��� �Լ�

    wchar_t szExt[25] = {};

    //Ȯ���ڸ��� szExt�� ����
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

    // System memory�� �ҷ����� �ȼ� �����͸� GPU�� ������.

    // Device , �̹���, �̹��� ���� , �̹����� ���� ����, ���������� ���� ���̴� ���ҽ� ��
    hr = CreateShaderResourceView(DEVICE, m_Image.GetImages() //Texture 2D, ShaderResourceView�� �����ϴ� �Լ�
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_pSRV.GetAddressOf());


    if (CheckFail(hr))
        return E_FAIL;


    m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf()); //Texture2D�� �޾ƿ��� �Լ�

    m_pTex2D->GetDesc(&m_tDesc);

    return S_OK;
}

void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag)
{
    // Texture �����
    
    m_tDesc.Width = _iWidth;
    m_tDesc.Height = _iHeight;
    m_tDesc.MipLevels = 0;
    m_tDesc.ArraySize = 1;

    m_tDesc.CPUAccessFlags = 0;
    m_tDesc.BindFlags = _flag; // �޾ƿ� ���ڷ� ���̴��� UAV�� ����
    m_tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

    
    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Format =_format;
    

    DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());

    assert(m_pTex2D);

    //SRV ���� Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
    tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
    tSRVDesc.Texture2D.MipLevels = 1;
    tSRVDesc.Texture2D.MostDetailedMip = 0;
    
    DEVICE->CreateShaderResourceView(m_pTex2D.Get(), nullptr, m_pSRV.GetAddressOf());
    assert(m_pSRV);

    //UAV ���� Unordered Access View
    D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
    tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D; //���� ���ҽ��� Texture 2D
    tUAVDesc.Texture2D.MipSlice = 0;
    
    DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
    
}

// �������� ���ε�
void CTexture::UpdateData(UINT _PipelineStage, int _iRegisterNum)
{
    if (_PipelineStage & (UINT)PIPELINE_STAGE::VS)
    {
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }

    if (_PipelineStage & (UINT)PIPELINE_STAGE::HS)
    {
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }

    if (_PipelineStage & (UINT)PIPELINE_STAGE::DS)
    {
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }

    if (_PipelineStage & (UINT)PIPELINE_STAGE::GS)
    {
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }

    if (_PipelineStage & (UINT)PIPELINE_STAGE::PS)
    {
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }
}

void CTexture::Clear(int _iRegisterNum)
{
    // �������Ϳ� nullptr
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}
