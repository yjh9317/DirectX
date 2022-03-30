Camera
================
Winapi에서는 Camera를 Singletone으로 사용했지만 DirectX에서는 Component중 하나로,  
카메라는 하나의 오브젝트를 만들어서 Camera Component를 갖고 카메라역할을 수행한다.  
  
여기서 다른 오브젝트들은 World Space을 기준으로 좌표를 갖고 있었지만,  
카메라를 기준으로 하기 위해 좌표들을 카메라 오브젝트의 좌표에 맞게 설정해야 한다.  
  
여기서 기존 원점을 기준으로 하는 World Space에서 카메라를 기준으로 한 공간을 Camera Space 혹은 View Space라고도 한다.  

View Space(Camera Space): 카메라의 위치,방향을 기준으로 물체들의 Transform값을 적용한다.  
View Space에서는 카메라의 위치가 원점, 카메라가 바라보는 정면이 z축이다.  




헤더
============
      #pragma once
      #include "CComponent.h"
      class CCamera :         
          public CComponent
      {
      private:
          Matrix m_matView;   // View 행렬, 이 행렬을 곱하면 View Space로 넘어온다.
          Matrix m_matProj;   // 투영 행렬(View Space로 넘어온 물체를 -1 ~ 1범위(셰이더는 ndc이므로)로 압축할 때 필요한 행렬)
      
      public:
          virtual void finalupdate() override;
      
      public:
          CCamera();
          ~CCamera();
      };
      
      
      
Code
=============
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
      	//world에 있는 물체들이 view space에서 사용하기 위해 View행렬,투영행렬을 구함
      
      	// View 행렬 계산
      
      	Vec3 vCamPos = Transform()->GetPos(); //카메라 오브젝트의 좌표
      	// 카메라가 이동한 만큼 다른 오브젝트들은 반대방향(음수)로 이동해야 된다.
      	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);
      
      
      	// 투영 행렬 계산
      
      	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
      	//XMMatrixOrthographicLH는 직교 투영 행렬, 윈도우의 가로,세로,z좌표 처음 ,z좌표 끝
      	m_matProj = XMMatrixOrthographicLH(vRenderResolution.x, vRenderResolution.y, 0.f, 5000.f);
      
      	g_transform.matView = m_matView;  //전역변수에 저장
      	g_transform.matProj = m_matProj;
       
      
      }


직교투영 행렬
===================
도움될만한 사이트 http://egloos.zum.com/EireneHue/v/985792  

직교투영 행렬은 X,Y축에 수직인 평행선을 그어서 z값은 변하지 않고 2D형식이 된다  
윈도우의 해상도에 맞게 가로,세로의 크기를 맞춰서 윈도우 안에서의 좌표와 1픽셀을 맞춰준다.

r: right   
l : left   
  
t: top  
b : bottom  
  
f: far  
n: near  
![asdasd](https://user-images.githubusercontent.com/66587512/160822001-151a87a8-e755-411a-bfb3-5462b62465f3.jpg)


