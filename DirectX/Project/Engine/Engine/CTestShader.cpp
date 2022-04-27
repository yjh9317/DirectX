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
	m_pOutputTex->UpdateData_CS(0);// UAV�� �̿��� u�������ͷ� ���ε�

}

void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}

