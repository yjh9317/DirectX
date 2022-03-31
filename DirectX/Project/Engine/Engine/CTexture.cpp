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
        hr = LoadFromTGAFile(_strFilePath.c_str(),nullptr, m_Image);
    }
    else // WIC : Window Image Component (png, jpg, jpeg ,bmp ....) 
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }
    
    if (CheckFail(hr))
        return E_FAIL;
    
    // System memory에 불러와진 픽셀 데이터를 GPU에 보낸다.
    
    // Device , 이미지, 이미지 개수 , 이미지에 대한 정보, 최종적으로 받을 쉐이더 리소스 뷰
    hr = CreateShaderResourceView(DEVICE,m_Image.GetImages() //Texture 2D, ShaderResourceView를 생성하는 함수
        ,m_Image.GetImageCount()
        ,m_Image.GetMetadata()
        ,m_pSRV.GetAddressOf());


    if (CheckFail(hr))
        return E_FAIL;


    m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf()); //Texture2D를 받아오는 함수


    
    return S_OK;
}
