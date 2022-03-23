#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

#ifdef _DEBUG		// ����� ���
UINT g_iFlag = D3DCOMPILE_DEBUG;	
#else				// ����� �ƴ� ��
UINT g_iFlag = 0;
#endif

vector<D3D11_INPUT_ELEMENT_DESC> CGraphicsShader::g_vecLayout;		// ��ǲ ���̾ƿ� �ʱ�ȭ


CGraphicsShader::CGraphicsShader()
	: m_eTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	// �⺻ �����ڰ� ���⿡ ó���� Topology�� ������� ��.
{
}

CGraphicsShader::~CGraphicsShader()
{
}





int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFunc)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// ���ؽ� ���̴�(HLSL) ������
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "vs_5_0", g_iFlag, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// ������ �� �ڵ�� VertexShader ��ü �����
	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateInputLayout(g_vecLayout.data(), g_vecLayout.size()
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

	// ���ؽ� ���̴�(HLSL) ������
	HRESULT hr = D3DCompileFromFile(wstring(strContentPath + _strRelativePath).c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), "ps_5_0", g_iFlag, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile Failed!!", MB_OK);
		return E_FAIL;
	}

	// ������ �� �ڵ�� VertexShader ��ü �����
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

	//��� ���̴��� ���ؼ� ����,�� ���̴��� ���� ����������Ѵ�.
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);
}


void CGraphicsShader::AddInputLayout(D3D11_INPUT_ELEMENT_DESC _desc)
{
	g_vecLayout.push_back(_desc);
}