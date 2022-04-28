#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	:CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(5)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleMtrl"));
		


	m_CS = (CParticleShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleShader").Get();

	tParticle arrParticle[5] =
	{
		tParticle{Vec3(-100.f,0.f,500.f),	Vec3(10.f,10.f,1.f),},
		tParticle{Vec3(-50.f,0.f,500.f),	Vec3(10.f,10.f,1.f),},
		tParticle{Vec3(0.f,0.f,500.f),		Vec3(10.f,10.f,1.f),},
		tParticle{Vec3(50.f,0.f,500.f),		Vec3(10.f,10.f,1.f),},
		tParticle{Vec3(100.f,0.f,500.f),	Vec3(10.f,10.f,1.f),},
	};

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_ONLY, false, arrParticle);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
}


void CParticleSystem::finalupdate()
{
	// ���� ó���� �ݺ������� �Ѵٸ� cpu�� ����ó���� �ϳ��ϳ� ó���ϹǷ�
	// gpu�� �Űܼ� ����ó���� �ٲ��ش�.
	// m_CS->SetParticleBuffer(m_ParticleBuffer);
	// m_CS->Excute();
}

void CParticleSystem::render()
{
	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::VS,16); // Particle�� ��ġ�� �������� VS���� ������Ʈ

	for (int i = 0; i < m_iMaxCount; ++i)
	{
		GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
		GetSharedMaterial()->UpdateData();
		GetMesh()->render();
	}
}