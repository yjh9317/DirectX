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
	
	// 호출 그룹 개수 계산

	m_iGroupPerThreadCountX; // 그룹 스레드의 X
	m_iGroupPerThreadCountY; // 그룹 스레드의 Y

	
	m_iGroupX = (UINT)m_pOutputTex->Width() / m_iGroupPerThreadCountX + !!((UINT)m_pOutputTex->Width() % m_iGroupPerThreadCountX); // 나머지가 있었다면 !!로 1이 나오고 없다면 0이 나온다
	m_iGroupY = (UINT)m_pOutputTex->Height() / m_iGroupPerThreadCountY + !!((UINT)m_pOutputTex->Height() % m_iGroupPerThreadCountY);
	m_iGroupZ = 1;

	//나머지가 있다면 스레드의 개수가 모자르다
	
}

void CTestShader::Clear()
{
	CTexture::ClearCS(0);
}

