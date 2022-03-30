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
	//world에 있는 물체들이 view space로 오도록 변환행렬을 구하는 작업

	// View 행렬 계산

	Vec3 vCamPos = Transform()->GetPos(); //카메라 오브젝트의 좌표
	// 카메라가 원점이 되도록 카메라가 이동한 만큼 다른 오브젝트들은 반대방향(음수)로 이동해야 된다.
	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);


	// 투영 행렬 계산

	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	//XMMatrixOrthographicLH는 직영 행렬, 윈도우의 가로,세로,z좌표 처음 ,z좌표 끝
	m_matProj = XMMatrixOrthographicLH(vRenderResolution.x, vRenderResolution.y, 0.f, 5000.f);

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	
	
}