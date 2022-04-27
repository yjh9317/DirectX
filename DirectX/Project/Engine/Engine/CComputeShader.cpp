#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CPathMgr.h"



#ifdef _DEBUG
static UINT g_iFlag = D3DCOMPILE_DEBUG;
#else
UINT g_iFlag = 0;
#endif




CComputeShader::CComputeShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_iGroupX(0)
	, m_iGroupY(0)
	, m_iGroupZ(0)
	, m_iGroupPerThreadCountX(_iGroupPerThreadX)
	, m_iGroupPerThreadCountY(_iGroupPerThreadY)
	, m_iGroupPerThreadCountZ(_iGroupPerThreadZ)
{
}

CComputeShader::~CComputeShader()
{
}

void CComputeShader::Excute()
{
	UpdateData();

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0); //Dispatch가 실행될 때 사용할 Compute Shader 세팅

	CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);// Compute Shader 실행

	Clear();
}

void CComputeShader::Excute(UINT _GroundX, UINT _GrounY, UINT GroupZ)
{
	UpdateData();

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0); //Dispatch가 실행될 때 사용할 Compute Shader 세팅

	CONTEXT->Dispatch(_GroundX, _GrounY, GroupZ);// Compute Shader 실행

	Clear();
}



int CComputeShader::CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// 컴퓨트 쉐이더(HLSL) 컴파일
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "cs_5_0", g_iFlag, 0
		, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Compute Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// 컴파일 된 코드로 ComputeShader 객체 만들기	
	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize()
		, nullptr, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
