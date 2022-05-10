#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CTransform.h"

#include "CResMgr.h"

const int iWidth = 10;
const int iHeight = 10;

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(iWidth* iHeight)
	, m_bPosInherit(0)
	, m_iAliveCount(1)
	, m_fAccTime(0.f)
	, m_fParticleCreateTerm(1.f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\AlphaCircle.png"));


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
	if (1.f < m_fAccTime)
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
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);			   //Particle�� ����� �����͹���
	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());   //��ƼŬ�� ������ġ(�߽�)�� �� WorldPos

	m_CS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i); // Particle�� �ε����� ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);	// Particle�� ��ӿ��θ� ����
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount); // Instancing���� Draw Call ����� ����

	m_ParticleBuffer->Clear();
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