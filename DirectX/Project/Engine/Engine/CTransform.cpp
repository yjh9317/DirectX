#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
	,m_vRelativeScale(Vec3(0.5f,0.5f,0.5f))	//행렬곱할 때 0이면 크기가 0이 고정이 되므로 전부 1로 설정
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	//렌더링하기 직전에 전달할 행렬을 finalupdate에서 생성 ,렌더링하기 직전에 전달

	//DirectX 함수로 행렬값 전달
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z); 
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);
	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);

	Matrix matRotation = matRotX * matRotY * matRotZ; //라디안 단위

	// World 행렬 만들기
	// Local Mesh에 World 행렬을 곱해서 Wolrd에 배치
	
	// 크기 x 회전(자전) x 이동 순서로 행렬곱 (순서중요)
	m_matWorld = matScale * matRotation * matTranslation;

	

	// 회전 행렬 (축을 기준으로 회전하므로 축의 좌표값은 바뀌지않고 나머지 좌표값이 바뀐다)
	// 원리는 삼각함수의 합차공식을 이용해서 나타냄.

	// x축 회전 행렬
	// (  1     0	    0  )
	// (  0    cosα   sinα )
	// (  0    -sinα  cosα )
	// 
	// y축 회전 행렬
	// ( cosα   0	  sinα )
	// (  0     1      0   )
	// ( -sinα  0     sinα )
	// 
	// z축 회전 행렬
	// ( cosα   sinα   0  )
	// ( -sinα  cosα   0  )
	// (  0      0     1  )

	// 행렬의 곱셈에서는 교환법칙이 성립하지 않으므로 회전,이동,크기 행렬을 곱할 때 
	// 크기-> 회전 -> 이동 순으로 곱해야한다
}



void CTransform::UpdateData()
{
	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform 컴포넌트의 상수버퍼를 가져오고
	pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// 특정 레지스터에 보낸다
	pBuffer->UpdateData();

}

