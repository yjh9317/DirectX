#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"

#include "CTransform.h"

#include "CResMgr.h"

const int iWidth = 10;
const int iHeight = 10;

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(iWidth* iHeight)
	, m_bPosInherit(0)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();



	tParticle arrParticle[iWidth * iHeight] = {};

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	



	for (int i = 0; i < iHeight; ++i)
	{
		for (int j = 0; j < iWidth; ++j)
		{
			arrParticle[iWidth * i + j].vPos =
				Vec3((vResolution.x / iWidth) * (float)j - vResolution.x / 2.f
					, (vResolution.y / iHeight) * (float)i - vResolution.y / 2.f
					, 500.f);

			arrParticle[iWidth * i + j].vScale = Vec3(10.f, 10.f, 1.f);
		}
	}




	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, arrParticle);
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
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::VS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i); // Particle�� �ε����� ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);	// Particle�� ��ӿ��θ� ����
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount); // Instancing���� Draw Call ����� ����

	/*for (int i = 0; i < m_iMaxCount; ++i)
	{
		GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
		GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
		GetMaterial()->UpdateData();
		GetMesh()->render_particle(m_iMaxCount);
	}*/

	// �������� ����� ���̱� ���� ��ü�� �׸��°����� ������ ������ �������Ӹ��� �߻��ϴ°��� �ٿ��ش�.
	// ���� �׸��������� ���������� X
}