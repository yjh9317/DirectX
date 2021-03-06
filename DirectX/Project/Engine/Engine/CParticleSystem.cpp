#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CTransform.h"

#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000)
	, m_bPosInherit(0)
	, m_iAliveCount(3)
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
	, m_fParticleCreateTerm(1.f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"));


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
	, m_iMaxCount(_origin.m_iMaxCount)
	, m_bPosInherit(_origin.m_bPosInherit)
	, m_iAliveCount(_origin.m_iAliveCount)
	, m_fAccTime(_origin.m_fAccTime)
	, m_fMinLifeTime(_origin.m_fMinLifeTime)
	, m_fMaxLifeTime(_origin.m_fMaxLifeTime)
	, m_fStartSpeed(_origin.m_fStartSpeed)
	, m_fEndSpeed(_origin.m_fEndSpeed)
	, m_vStartColor(_origin.m_vStartColor)
	, m_vEndColor(_origin.m_vEndColor)
	, m_vStartScale(_origin.m_vStartScale)
	, m_vEndScale(_origin.m_vEndScale)
	, m_fParticleCreateDistance(_origin.m_fParticleCreateDistance)
	, m_fParticleCreateTerm(_origin.m_fParticleCreateTerm)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"));

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
}


CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}


void CParticleSystem::SetMaxParticleCount(UINT _iMax)
{
	if (m_iMaxCount < _iMax)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), _iMax, SB_TYPE::READ_WRITE, false, nullptr);
	}
	m_iMaxCount = _iMax;
}

void CParticleSystem::finalupdate()
{
	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;


		tParticleData data = {};
		data.iAliveCount = m_iAliveCount;	//파티클 개수 설정
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

	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);		// 파티클의 생성범위
	m_CS->SetParticleBuffer(m_ParticleBuffer);						// 파티클 버퍼
	m_CS->SetParticleDataBuffer(m_DataBuffer);						  //Particle이 사용할 데이터버퍼
	

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);	// 파티클 생명시간
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);				// 파티클 속도
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);				// 파티클 색상
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);				// 파티클 크기

	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());			//파티클의 스폰위치(중심)이 될 WorldPos

	m_CS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);	// Particle의 상속여부를 전달
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount); // Instancing으로 Draw Call 비용을 줄임

	m_ParticleBuffer->Clear();

	// 렌더링의 비용을 줄이기 위해 물체를 그리는과정과 마무리 과정이 매프레임마다 발생하는것을 줄여준다.
	// 전부 그리기전까지 마무리과정 X
}

void CParticleSystem::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	wstring strKey;
	if (nullptr != m_CS)
		strKey = m_CS->GetKey();
	SaveWStringToFile(strKey, _pFile);

	fwrite(&m_iMaxCount, sizeof(UINT), 1, _pFile);		// 최대 개수
	fwrite(&m_bPosInherit, sizeof(int), 1, _pFile);		// 부모 영향 체크
	fwrite(&m_iAliveCount, sizeof(int), 1, _pFile);		// 생성 개수
	fwrite(&m_fMinLifeTime, sizeof(float), 1, _pFile);	// 최소 생명 시간
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);	// 최대 생명 시간
	fwrite(&m_fStartSpeed, sizeof(float), 1, _pFile);	// 시작 속도
	fwrite(&m_fEndSpeed, sizeof(float), 1, _pFile);		// 끝 속도
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _pFile);	// 시작 색깔
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _pFile);		// 끝 색깔
	fwrite(&m_vStartScale, sizeof(Vec3), 1, _pFile);	// 시작 크기
	fwrite(&m_vEndScale, sizeof(Vec3), 1, _pFile);		// 끝 크기
	fwrite(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);	// 파티클 범위
	fwrite(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);		// 파티클 생성 시간
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	wstring strKey;
	LoadWStringFromFile(strKey, _pFile);
	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();


	UINT iMaxCount = 0;
	fread(&iMaxCount, sizeof(UINT), 1, _pFile);
	SetMaxParticleCount(iMaxCount);

	fread(&m_bPosInherit, sizeof(int), 1, _pFile);
	fread(&m_iAliveCount, sizeof(int), 1, _pFile);
	fread(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fread(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fread(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fread(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);


}
