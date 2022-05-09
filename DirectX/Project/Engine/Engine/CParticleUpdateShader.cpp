#include "pch.h"
#include "CParticleUpdateShader.h"


CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(32, 1, 1)
	, m_ParticleBuffer(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0, false);

	m_iGroupX = m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadCountX + !!(m_ParticleBuffer->GetElementCount() % m_iGroupPerThreadCountX);
	m_iGroupY = 1;
	m_iGroupZ = 1;

	//Max Thread Count , 이 값으로 Particle의 UV값을 조정하는데 사용
	m_Param.iArr[0] = m_iGroupX * m_iGroupPerThreadCountX;
	m_Param.iArr[1] = m_ParticleBuffer->GetElementCount();
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
}