#include "pch.h"
#include "CGameObject.h"



#include "CComponent.h"
#include "CTransform.h"
#include "CMeshRender.h"



CGameObject::CGameObject()
	:m_arrCom{}
	,m_bActive(true)
	,m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	:CEntity(_origin)
	, m_arrCom{}
	, m_bActive(true)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i]) {
			AddComponent(_origin.m_arrCom[i]->Clone());
		}
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
}

void CGameObject::start()
{
	// 게임이 시작되는 순간에 최초로 호출되는 함수
	// 게임 중간에 추가되는 오브젝트들은 생성되서 합류하는 시점에서 start를 호출된다
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

}

void CGameObject::update()
{


	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->update();
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}
}

void CGameObject::render()
{
	if (nullptr != MeshRender())
		MeshRender()->render();
}

void CGameObject::AddComponent(CComponent* _component)
{
	COMPONENT_TYPE eType= _component->GetType();		//컴포넌트의 타입확인후

	assert(nullptr == m_arrCom[(UINT)eType]);	
	

	m_arrCom[(UINT)eType] = _component;				// 배열에 추가
	_component->m_pOwner =this;						// 컴포넌트에서 소유주 알림
}