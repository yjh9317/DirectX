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
        

