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
	// AliveCount를 상수버퍼로 넘겨주면 안되는 이유는 Count를 받아도 수정해서 Count를 줄여줄수 없기 때문
	// 상수버퍼로 살아있는 파티클 개수를 가져와도 만약 하나를 활성화 해도 읽기 모드만 지원하기 때문에 구조화 버퍼로 작성.
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



	// 입자 처리를 반복문으로 한다면 cpu는 직렬처리로 하나하나 처리하므로
	// gpu로 옮겨서 병렬처리로 바꿔준다.
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);			   //Particle이 사용할 데이터버퍼
	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());   //파티클의 스폰위치(중심)이 될 WorldPos

	m_CS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i); // Particle의 인덱스를 전달
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);	// Particle의 상속여부를 전달
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount); // Instancing으로 Draw Call 비용을 줄임

	m_ParticleBuffer->Clear();
	/*for (int i = 0; i < m_iMaxCount; ++i)
	{
		GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
		GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
		GetMaterial()->UpdateData();
		GetMesh()->render_particle(m_iMaxCount);
	}*/

	// 렌더링의 비용을 줄이기 위해 물체를 그리는과정과 마무리 과정이 매프레임마다 발생하는것을 줄여준다.
	// 전부 그리기전까지 마무리과정 X
}