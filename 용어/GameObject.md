GameObject
================
오브젝트임을 알리는 클래스.  


헤더 파일
==============
      #pragma once
      #include "CEntity.h"
      
      #define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }
      // C##type -> Ctype  , 위에서 class를 붙이면 전방선언도 같이된다.
      
      
      class CComponent;   //서로의 헤더에서 #include로 서로를 참조하면 오류가 생긴다, 그래서 한쪽은 전방선언으로해야된다.
      
      
      class CGameObject : //오브젝트는 컴포넌트 기반으로 실행
          public CEntity
      {
      private:
          CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];  //오브젝트의 컴포넌트 배열
      
      public:
          void start();
          void update();
          void lateupdate();
          void finalupdate();
          void render();
      
      
      
      public:
          void    AddComponent(CComponent* _component);
          CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
      
        
          // 컴포넌트를 매크로형식으로 작성
          GET_COMPONENT(Transform, TRANSFORM)
          GET_COMPONENT(MeshRender, MESHRENDER)
      
      
      public:
          CGameObject();
          ~CGameObject();
      };
     
     



Cpp 
===========

      CGameObject::CGameObject()
      	:m_arrCom{}
      {
      }
      
      CGameObject::~CGameObject()
      {
      	Safe_Del_Arr(m_arrCom);
      }
      
      void CGameObject::start()
      {
      	// 게임이 시작되는 순간에 최초로 호출되는 함수
      	// 게임 중간에 추가되는 오브젝트들은 생성되서 합류하는 시점에서 start를 호출된다
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
      	if (nullptr != MeshRender())  //MeshRender가 nullptr이 아닐때만 render
      		MeshRender()->render();
      }
      
      void CGameObject::AddComponent(CComponent* _component)
      {
      	COMPONENT_TYPE eType= _component->GetType();		//컴포넌트의 타입확인후
      
      	assert(nullptr == m_arrCom[(UINT)eType]);	
      	
      
      	m_arrCom[(UINT)eType] = _component;				// 배열에 추가
      	_component->m_pOwner =this;						    // 컴포넌 자기를 참조하는 오브젝트를 알림
      }
