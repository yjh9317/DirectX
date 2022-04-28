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
	
	// ȣ�� �׷� ���� ���

	m_iGroupPerThreadCountX; // �׷� �������� X
	m_iGroupPerThreadCountY; // �׷� �������� Y

	
	m_iGroupX = (UINT)m_pOutputTex->Width() / m_iGroupPerThreadCountX + !!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX); // �������� �־��ٸ� !!�� 1�� ������ ���ٸ� 0�� ���´�
	m_iGroupY = (UINT)m_pOutputTex->Height() / m_iGroupPerThreadCountY + !!((UINT)m_pOutputTex->Height() % m_iGroupPerThreadCountY);
	m_iGroupZ = 1;

	//�������� �ִٸ� �������� ������ ���ڸ���
	
}

void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}

