#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"

CGameObject::CGameObject()
	:m_arrCom{}
{
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
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