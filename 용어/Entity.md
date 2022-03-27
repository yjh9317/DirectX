Entity
==========
모든 오브젝트의 상위 클래스.  


헤더파일
=====================
      #pragma once
      
      
      class CEntity	//모든 오브젝트의 상위클래스
      {
      private:
      	static UINT g_iNextID;    // 오브젝트 객체의 아이디
      
      private:
      	const UINT	m_iID;        
      	wstring		m_strName;      // 오브젝트의 이름
      
      public:
      	void SetName(const wstring& _name) { m_strName = _name; }
      	const wstring& GetName() { return m_strName; }
      	UINT GetID() { return m_iID; }
      
      public:
      	virtual void UpdateData() {}	//파이프라인에 전달
      
      public:
      	CEntity();
      	CEntity(const CEntity& _origin);
      	virtual ~CEntity();
      };
      
      


Cpp
==================


      UINT CEntity::g_iNextID = 0;  //초기화
      
      
      CEntity::CEntity()
      	: m_iID(g_iNextID++)  //생성 될 때마다 클래스의 정적 변수를 이용해 아이디를 부여
      {
      }
      
      CEntity::CEntity(const CEntity& _origin)  //복사 생성자
      	: m_strName(_origin.m_strName)
      	, m_iID(g_iNextID++)
      {
      }
      
      CEntity::~CEntity()
      {
      }
