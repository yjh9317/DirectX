#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

#ifdef _DEBUG
static UINT g_iFlag = D3DCOMPILE_DEBUG;
#else
UINT g_iFlag = 0;
#endif

CComputeShader::CComputeShader()
{
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// ��ǻƮ ���̴�(HLSL) ������
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "cs_5_0", g_iFlag, 0
		, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Compute Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// ������ �� �ڵ�� ComputeShader ��ü �����	
	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize()
		, nullptr, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
