Mesh
==============
Mesh는 버퍼안에 생성한 정점을 이용해서 도형이나 물체를 만들고 리소스처럼 사용한다.  


헤더 파일
===================
      #pragma once
      #include "CRes.h"
      class CMesh :       // 정점을 이용한 도형,물체(리소스로 취급해서 사용)
          public CRes
      {
      private:
          ComPtr<ID3D11Buffer>    m_VB;       // 버텍스 버퍼
          ComPtr<ID3D11Buffer>    m_IB;       // 인덱스 버퍼
      
          D3D11_BUFFER_DESC      m_tVBDesc;   
          D3D11_BUFFER_DESC      m_tIBDesc;   
      
          UINT                    m_iVtxCount;    // 정점 개수
          UINT                    m_iIdxCount;    // 인덱스 개수
      
      
      
          //기본적으로 버퍼와 맵핑되는 시스템메모리 쪽도 관리해준다.
          //시스템 메모리쪽에 있는 데이터가 소실되면 gpu메모리도 손상될 수 있기 때문
      
          void*                   m_pVtxSys; 
          void*                   m_pIdxSys; 
      
      public:
          virtual int Load(const wstring& _strFilePath) { return S_OK; }
          virtual void UpdateData() override;
          
          int Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount); //버퍼 생성
      
          void render();
      
      public:
          CMesh();
          ~CMesh();
      };



Cpp
============
      #include "pch.h"
      #include "CMesh.h"
      
      #include "CDevice.h"
      
      
      CMesh::CMesh()
      	: m_tVBDesc{}
      	, m_tIBDesc{}
      	, m_iVtxCount(0)
      	, m_iIdxCount(0)
      	, m_pVtxSys(nullptr)
      	, m_pIdxSys(nullptr)
      {
      }
      
      CMesh::~CMesh()
      {
      	SAFE_DELETE(m_pVtxSys); // 시스템 메모리에 있는 버텍스 버퍼 삭제
      	SAFE_DELETE(m_pIdxSys); // 시스템 메모리에 있는 인덱스 버퍼 삭제
      }
      
      
      
      int CMesh::Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount)
      {
      	m_iVtxCount = _iVtxCount;
      	m_iIdxCount = _iIdxCount;
      
      	// 버텍스 버퍼의 개수만큼 생성
      	m_tVBDesc.ByteWidth = sizeof(Vertex) * _iVtxCount;
      
      	// 버텍스 버퍼 수정 X
      	m_tVBDesc.CPUAccessFlags = 0;
      	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;
      
      	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
      	m_tVBDesc.MiscFlags = 0;
      	m_tVBDesc.StructureByteStride = 0;
      
      	D3D11_SUBRESOURCE_DATA tSubDesc = {};
      	tSubDesc.pSysMem = _pVtxSys;
      
      	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_VB.GetAddressOf())))
      	{
      		return E_FAIL;
      	}
      
      	m_tIBDesc.ByteWidth = sizeof(UINT) * _iIdxCount;
      
      	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션 (밑에는 수정x)
      	m_tIBDesc.CPUAccessFlags = 0;
      	m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
      
      	// 정점을 저장하는 목적의 버퍼 임을 알림
      	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
      	m_tIBDesc.MiscFlags = 0;
      	m_tIBDesc.StructureByteStride = 0;
      
      	// 초기 데이터를 넘겨주기 위한 정보 구조체
      	tSubDesc = {};
      	tSubDesc.pSysMem = _pIdxSys;
      
      	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSubDesc, m_IB.GetAddressOf())))
      	{
      		return E_FAIL;
      	}
      
      
      	//시스템메모리의 지역변수를 받아오면 그 함수가 끝날 때 주소가 날라가므로 내부적으로 복사해서 관리
      	m_pVtxSys = new Vtx[m_iVtxCount];
      	memcpy(m_pVtxSys, _pVtxSys, sizeof(Vtx) * m_iVtxCount);
      
      	m_pIdxSys = new UINT[m_iIdxCount];
      	memcpy(m_pIdxSys, _pIdxSys, sizeof(UINT) * m_iIdxCount);
      
      	return S_OK;
      }
      
      void CMesh::render()
      {
      	UpdateData();
      
      	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
      }
      
      void CMesh::UpdateData()
      {
      	
      
      	UINT iStride = sizeof(Vtx); //정점의 크기(바이트)
      	UINT iOffset = 0;
      
      	//버퍼를 gpu 메모리에 전달 
      	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
      	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
      }
