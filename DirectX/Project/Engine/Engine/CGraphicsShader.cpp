#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

#ifdef _DEBUG		// 디버그 모드
UINT g_iFlag = D3DCOMPILE_DEBUG;
#else				// 디버그 아닐 때
UINT g_iFlag = 0;
#endif

vector<D3D11_INPUT_ELEMENT_DESC> CGraphicsShader::g_vecLayout;		// 인풋 레이아웃 초기화


CGraphicsShader::CGraphicsShader()
	: m_eRSType(RS_TYPE::CULL_BACK)
	, m_eDSType(DS_TYPE::LESS)
	, m_eBSType(BS_TYPE::DEFAULT)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	// 기본 생성자가 없기에 처음에 Topology를 정해줘야 함.
{
}

CGraphicsShader::~CGraphicsShader()
{
}





int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFunc)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 버텍스 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "vs_5_0", g_iFlag, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 VertexShader 객체 만들기
	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateInputLayout(g_vecLayout.data(), (UINT)g_vecLayout.size()
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_InputLayout.GetAddressOf())))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFunc)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 픽셀 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "ps_5_0", g_iFlag, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 PixelShader 객체 만들기
	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_InputLayout.Get());
	CONTEXT->IASetPrimitiveTopology(m_eTopology);

	//모든 셰이더에 대해서 설정,각 셰이더는 따로 설정해줘야한다.
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	CONTEXT->RSSetState(CDevice::GetInst()->GetRS(m_eRSType).Get());//래스터라이저 스테이트 설정
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDS(m_eDSType).Get(), 0); //깊이 스텐실 설정
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBS(m_eBSType).Get(), Vec4(), 0xffffffff);	//블렌드 스테이트 설정
}

void CGraphicsShader::AddScalarParamInfo(const wstring& _strDesc, SCALAR_PARAM _eParamType)
{
	m_vecScalarParamInfo.push_back(tScalarParamInfo{ _strDesc,_eParamType });
}

void CGraphicsShader::AddTexParamInfo(const wstring& _strDesc, TEX_PARAM _eParamType)
{
	m_vecTexParamInfo.push_back(tTexParamInfo{ _strDesc ,_eParamType });
}


void CGraphicsShader::AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc)
{
	g_vecLayout.push_back(_desc);
}