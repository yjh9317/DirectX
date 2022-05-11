#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CTransform.h"

#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000)
	, m_bPosInherit(0)
	, m_iAliveCount(1)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(2.f)
	, m_fStartSpeed(100.f)
	, m_fEndSpeed(10.f)
	, m_vStartColor(Vec4(1.f, 0.2f, 0.7f, 1.f))
	, m_vEndColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vStartScale(Vec3(10.f, 10.f, 1.f))
	, m_vEndScale(Vec3(1.f, 1.f, 1.f))
	, m_fParticleCreateDistance(50.f)
	, m_fParticleCreateTerm(0.005f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\AlphaCircle.png");
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);


	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
	// AliveCount�� ������۷� �Ѱ��ָ� �ȵǴ� ������ Count�� �޾Ƶ� �����ؼ� Count�� �ٿ��ټ� ���� ����
	// ������۷� ����ִ� ��ƼŬ ������ �����͵� ���� �ϳ��� Ȱ��ȭ �ص� �б� ��常 �����ϱ� ������ ����ȭ ���۷� �ۼ�.
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}



void CParticleSystem::finalupdate()
{
	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_iAliveCount;

		tParticleData data = {};
		data.iAliveCount = 1;
		m_DataBuffer->SetData(&data, 1);
	}

	// StructuredBuffer ReadWrite Test
	/*
	tParticleData data = {};
	data.iAliveCount = 1;
	m_DataBuffer->SetData(&data, 1);

	tParticleData data1 = {};
	m_DataBuffer->GetData(&data1);
	*/



	// ���� ó���� �ݺ������� �Ѵٸ� cpu�� ����ó���� �ϳ��ϳ� ó���ϹǷ�
	// gpu�� �Űܼ� ����ó���� �ٲ��ش�.

	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);		// ��ƼŬ�� ��������
	m_CS->SetParticleBuffer(m_ParticleBuffer);						// ��ƼŬ ����
	m_CS->SetParticleDataBuffer(m_DataBuffer);						  //Particle�� ����� �����͹���
	

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);	// ��ƼŬ ����ð�
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);				// ��ƼŬ �ӵ�
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);				// ��ƼŬ ����
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);				// ��ƼŬ ũ��

	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());			//��ƼŬ�� ������ġ(�߽�)�� �� WorldPos

	m_CS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);	// Particle�� ��ӿ��θ� ����
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount); // Instancing���� Draw Call ����� ����

	m_ParticleBuffer->Clear();

	// �������� ����� ���̱� ���� ��ü�� �׸��°����� ������ ������ �������Ӹ��� �߻��ϴ°��� �ٿ��ش�.
	// ���� �׸��������� ���������� X
}