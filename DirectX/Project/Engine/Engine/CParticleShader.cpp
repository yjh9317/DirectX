#include "pch.h"
#include "CParticleShader.h"

CParticleShader::CParticleShader()
	:CComputeShader(32,1,1)
	,m_ParticleBuffer(nullptr)
{
}

CParticleShader::~CParticleShader()
{
}


void CParticleShader::UpdateData()
{
	//m_ParticleBuffer->UpdateData_CS(0); //0�� �������Ϳ� ���ε�

	// ������ ������ ����ؼ� ������ ����
	m_iGroupX = m_ParticleBuffer->GetBufferCount()/ m_iGroupPerThreadCountX + !!(m_ParticleBuffer->GetBufferCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;

	
}

void CParticleShader::Clear()
{
	//m_ParticleBuffer->Clear();
}

