Transform
===============
Transform은 GameObject가 가지고있는 Component중 하나로 오브젝트의 Location(위치,크기,회전) 를 담당한다.  
3차원(x, y, z)의 값을 가진 변수들을 4차원(x, y, z, 1) 변수로 바꾸고 행렬곱을 이용해 나타낸다  
(마지막 값은 0 or 1 이 들어가고 여기서는 1을 넣어 행렬곱으로 위치값을 받는다.)  

<br/>
<br/>

    
예시) x,y,z의 크기가 2배가 되고 x는 10, y는 2, z는 6 이동
![asdasd](https://user-images.githubusercontent.com/66587512/160394906-fe6d6b7f-e80b-4343-a699-9c00475de8a2.PNG)


1행1열, 2행2열, 3행3열은 x , y, z 의 크기값이고  
4행1열은 x좌표의 이동값  
4행2열은 y좌표의 이동값   
4행3열은 z좌표의 이동값이다.  


코드
==========================================

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

	//DirectX 행렬 함수
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z); //1행1열,2행2열,3행3열에 크기
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z); 4행1,2,3열에 위치
	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);

	Matrix matRotation = matRotX * matRotY * matRotZ; //라디안 단위

	// World 행렬 만들기
	// Local Mesh에 World 행렬을 곱해서 Wolrd에 배치


	// 행렬의 곱셈에서는 교환법칙이 성립하지 않으므로 회전,이동,크기 행렬을 곱할 때 
	// 크기-> 회전 -> 이동 순으로 곱해야한다
	
	// 크기 x 회전(자전) x 이동 순서로 행렬곱 (순서중요)
	m_matWorld = matScale * matRotation * matTranslation;

	

	// 회전 행렬 (축을 기준으로 회전하므로 축의 좌표값은 바뀌지않고 나머지 좌표값이 바뀐다)

	/*
	    삼각함수의 합차공식으로 단위원에서 각도가 θ,α+θ인 두 좌표를 이용해 
		각도가 θ인 좌표는 (cosθ,sinθ)이 나오고 
		각도가 α+θ인 좌표는 cosθ(cosα -sinα)+sinθ(sinα + cosα)가 된다. -> cosθ,sinθ끼리 묶음
		
		각도가 θ인 점의 좌표(cosθ,sinθ)에서 각도가 α+θ인 점의 좌표(cos(α+θ),sin(α+θ))이 나오도록
		행렬을 곱한다면 다음과 같은 행렬이 나오고 이 행렬로 기준이 아닌 축에 곱해서 회전행렬을 구한다. 

		( cosα   sinα )
		( -sinα  cosα ) 
		
	*/

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
	}
	
	
	
	void CTransform::UpdateData()
	{
		// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
		CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform 컴포넌트의 상수버퍼를 가져오고
		pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// 특정 레지스터에 보낸다
		pBuffer->UpdateData();
	
	}

    	 
    void CTransform::UpdateData()
    {
    	// 좌표정보가 렌더링되기 직전에 b0레지스터에 보내짐
    	CConstBuffer* pBuffer= CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);	// Transform 컴포넌트의 상수버퍼를 가져오고
    	pBuffer->SetData(&m_matWorld, sizeof(Matrix));							// 특정 레지스터에 보낸다
    	pBuffer->UpdateData();
    
    }
	
헤더
===========
        #pragma once
        #include "CComponent.h"
        class CTransform :      //오브젝트의 Location(위치,크기,회전) 담당
            public CComponent
        {
        private:
            Vec3        m_vRelativePos;
            Vec3        m_vRelativeScale;
            Vec3        m_vRelativeRot;
        
            Matrix      m_matWorld;         //위치변환 정보 행렬, 행렬로 위치,크기,회전값을 정한다.
        
        public:
            void SetPos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
            void SetScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
            void SetRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }
        
            const Vec3& GetPos() { return m_vRelativePos; }
            const Vec3& GetScale() { return m_vRelativeScale; }
            const Vec3& GetRotation() { return m_vRelativeRot; }
        
            virtual void UpdateData() override; //필요한 데이터를 gpu메모리로 이동
        
            virtual void finalupdate() override;
        
        public:
            CTransform();
            ~CTransform();
        };
        
HLSL
=======================
        
        cbuffer TRANSFORM : register(b0)
        {
             // row_major : 행 우선으로 읽기 , matrix or float4x4 : 4x4 행렬
             
            row_major matrix g_matWorld;     // 4x4형태 행렬
        }
        
        버텍스 셰이더
        VTX_OUT VS_Test(VTX_IN _in)   _in은 정점
        {
            VTX_OUT output = (VTX_OUT) 0.f;
            
            float4 vFinalPos = mul(float4(_in.vPos, 1.f), g_matWorld);   // 4*4 행렬과 위치를 곱해서 최종위치
            
            // 정점(float3)을 4*4행렬 로 맞춰주기위해 float4로 바꿔주면서 마지막 값은 행렬곱해서 위치값을 저장해주기 위해 1.f로 세팅
           
            
            output.vPosition = vFinalPos;
            output.vColor = _in.vColor;
            
            return output;
        }
        

