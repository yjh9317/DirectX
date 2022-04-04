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
	// ������ ���۵Ǵ� ������ ���ʷ� ȣ��Ǵ� �Լ�
	// ���� �߰��� �߰��Ǵ� ������Ʈ���� �����Ǽ� �շ��ϴ� �������� start�� ȣ��ȴ�
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
	COMPONENT_TYPE eType= _component->GetType();		//������Ʈ�� Ÿ��Ȯ����

	assert(nullptr == m_arrCom[(UINT)eType]);	
	

	m_arrCom[(UINT)eType] = _component;				// �迭�� �߰�
	_component->m_pOwner =this;						// ������Ʈ���� ������ �˸�
}