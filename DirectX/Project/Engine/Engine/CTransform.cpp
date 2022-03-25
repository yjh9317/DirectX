#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
{
}

CTransform::~CTransform()
{
}

void CTransform::UpdateData()
{
	// ��ǥ������ �������Ǳ� ������ b0�������Ϳ� ������
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	
	pBuffer->SetData(&m_vRelativePos, sizeof(Vec4));
	pBuffer->UpdateData();

}

void CTransform::finalupdate()
{
}