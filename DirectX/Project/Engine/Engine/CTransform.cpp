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
	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	
	pBuffer->SetData(&m_vRelativePos, sizeof(Vec4));
	pBuffer->UpdateData();

}

void CTransform::finalupdate()
{
}