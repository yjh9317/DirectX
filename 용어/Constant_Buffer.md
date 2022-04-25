Vertex Buffer를 이용한 이동
============================
아래는 키를 입력한다면 버텍스 버퍼의 정점 위치를 옮겨서 도형을 옮기는 코드이다.  
(인덱스 버퍼를 이용한 사각형이므로 정점의 개수는 4)  
  
    void Update(){
    
  	if (KEY_PRESSED(KEY::LEFT))
  	{
  		for (int i = 0; i < 4; ++i)
  		{
  			arrVtx[i].vPos.x -= DT * 0.5f;
  		}
  	}
  
  	if (KEY_PRESSED(KEY::RIGHT))
  	{
  		for (int i = 0; i < 4; ++i)
  		{
  			arrVtx[i].vPos.x += DT * 0.5f;
  		}
  	}

  	D3D11_MAPPED_SUBRESOURCE tSub = {}; //임시 메모리
  
  	CONTEXT->Map(g_pVB.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&tSub); //임시 메모리와 버텍스 버퍼를 맵핑
  	memcpy(tSub.pData, arrVtx, sizeof(VTX) * 4);        		//arrVtx값을 임시 메모리에 전이
  	CONTEXT->Unmap(g_pVB.Get(), 0);
    
    }

  
Constant Buffer
=================
위에서 처럼 버텍스 버퍼를 이용하여 물체를 이동시킬 수 있지만 그렇게 한다면 문제점이 있다.  
버텍스 버퍼자체를 수정하기 때문에 정점의 개수가 많아진다면 모든 정점의 위치값(vPos)를 개별적으로   
값을 바꾸기 때문에 엄청난 비용이 들게 된다.  
  
그렇기에 상수 버퍼를 따로 만들어서 그 버퍼에 상수(이동량)를 저장하고  
모든 정점에 대해 상수값을 적용시켜 물체를 이동시킬 수 있게 된다.  


Constant Buffer 헤더
=================================
	// 상수 버퍼는 gpu메모리를 사용하지만 실제 파이프라인일 때
	// 특정 레지스터에 상수값을 보내야 하는데 몇번 레지스터를 보낼지 모르기에
	// 미리 만들어 놓는다.
	
	
	
	#pragma once
	#include "CEntity.h"
	
	class CConstBuffer :
	    public CEntity
	{
	private:
	    ComPtr<ID3D11Buffer>	m_CB;	// 상수버퍼
	    D3D11_BUFFER_DESC           m_Desc;	// 버퍼 Desc
	
	    CB_TYPE                     m_eCBType; // 상수버퍼를 바인딩 할 레지스터 번호
	
	public:
	    virtual void UpdateData() override;	// 버퍼를 레지스터에 바인딩
	    int Create(UINT _iBufferSize);	// 상수버퍼 생성
	
	    void SetData(void* _pData, UINT _iSize);
	
	    virtual CConstBuffer* Clone() { return nullptr; }
	
	public:
	    CConstBuffer(CB_TYPE _eType);
	    ~CConstBuffer();
	};
	
	
		


Constant Buffer cpp
===============
      

	#include "pch.h"
	#include "CConstBuffer.h"
	
	#include "CDevice.h"
	
	CConstBuffer::CConstBuffer(CB_TYPE _eType)
		: m_eCBType(_eType)
		, m_Desc{}
	{
	}
	
	CConstBuffer::~CConstBuffer()
	{
	}
	
	void CConstBuffer::UpdateData() // 상수버퍼를 레지스터에 바인딩
	{
		CONTEXT->VSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
		CONTEXT->HSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
		CONTEXT->DSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
		CONTEXT->GSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
		CONTEXT->PSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
	}
	
	int CConstBuffer::Create(UINT _iBufferSize)
	{
		m_Desc.ByteWidth = _iBufferSize;
	
		// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	
		// 정점을 저장하는 목적의 상수 버퍼 임을 알림
		m_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		m_Desc.MiscFlags = 0;
		m_Desc.StructureByteStride = 0;
	
		if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
		{
			return E_FAIL;
		}
	
		return S_OK;
	}
	
	void CConstBuffer::SetData(void* _pData, UINT _iSize)
	{
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		
		// CPU->GPU로 데이터 복사
		CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, _pData, _iSize);
		CONTEXT->Unmap(m_CB.Get(), 0);
	}
	
	
	    
	
