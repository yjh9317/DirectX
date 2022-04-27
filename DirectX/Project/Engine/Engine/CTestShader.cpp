#include "pch.h"
#include "CTestShader.h"

CTestShader::CTestShader()
	: CComputeShader(32, 32, 1)
{
}

CTestShader::~CTestShader()
{
}


void CTestShader::UpdateData()
{
	m_pOutputTex->UpdateData_CS(0);// UAV를 이용해 u레지스터로 바인딩

}

void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}

