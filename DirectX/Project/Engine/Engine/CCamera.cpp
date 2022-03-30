#include "pch.h"
#include "CCamera.h"


#include "CTransform.h"
#include "CDevice.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
{
}

CCamera::~CCamera()
{
}

void CCamera::finalupdate()
{
	//world�� �ִ� ��ü���� view space�� ������ ��ȯ����� ���ϴ� �۾�

	// View ��� ���

	Vec3 vCamPos = Transform()->GetPos(); //ī�޶� ������Ʈ�� ��ǥ
	// ī�޶� ������ �ǵ��� ī�޶� �̵��� ��ŭ �ٸ� ������Ʈ���� �ݴ����(����)�� �̵��ؾ� �ȴ�.
	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);


	// ���� ��� ���

	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	//XMMatrixOrthographicLH�� ���� ���, �������� ����,����,z��ǥ ó�� ,z��ǥ ��
	m_matProj = XMMatrixOrthographicLH(vRenderResolution.x, vRenderResolution.y, 0.f, 5000.f);

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	
	
}