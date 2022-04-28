#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
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
	//리소스 바인딩
	UpdateData();

	// 상수 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::SCALAR_PARAM);
	pCB->SetData(&m_Param, sizeof(tScalarParam));
	pCB->UpdateData_CS();

	// 사용할 Compute Shader 세팅
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);

	// 컴퓨트 쉐이더 실행 (그룹 개수 지정)
	CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);

	Clear();
}

void CComputeShader::Excute(UINT _GroundX, UINT _GrounY, UINT GroupZ)
{
	// 1. 자식 클래스가 가지고 있는 Texture의 Update_CS에서 레지스터 바인딩
	// 2. CONTEXT->CSSetShader로 사용할 Compute Shader 세팅
	// 3. 그 후 Dispatch로 ComputeShader 실행
	// 4. 마지막에는 정적 멤버 함수인 CTexture의 ClearCS로 레지스터를 nullptr로 변경

	//리소스 바인딩
	UpdateData();	

	// 상수 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::SCALAR_PARAM);
	pCB->SetData(&m_Param, sizeof(tScalarParam));
	pCB->UpdateData_CS();

	// 사용할 Compute Shader 세팅
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0); 

	// 컴퓨트 쉐이더 실행 (그룹 개수 지정)
	CONTEXT->Dispatch(_GroundX, _GrounY, GroupZ);

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
		assert(hr);
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
