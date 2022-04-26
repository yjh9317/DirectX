StructuredBuffer(구조화버퍼)
=============================
ConstantBuffer(상수 버퍼)는 변수를 만들어서 그 변수를 gpu 레지스터에 복사하여 사용하는 방식이라 메모리 제한이 있다.  
하지만 StructuredBuffer(구조화버퍼)는 ConstantBuffer와 달리 b레지스터가 아닌 t레지스터 (텍스쳐 레지스터)를 이용하므로  
레지스터에 메모리쪽을 간접적인 참조방식(포인터처럼)을 이용하여 메모리에 제한이 덜 받는다.  


StructuredBuffer 헤더
======================
#pragma once
#include "CEntity.h"

// ID3D11Buffer-> gpu에 버퍼를 잡는다

//구조화 버퍼의 타입값
      enum class SB_TYPE
      {
          READ_ONLY,          // 쉐이더 상에서 읽기
          READ_WRITE,         // 쉐이더 상에서 읽기,쓰기 (UAV를 이용하여 PS에서 데이터를 수정가능)
      };
      
      
      class CStructuredBuffer :
          public CEntity
      {
      private:
          ComPtr<ID3D11Buffer>              m_SB;      // Dynamic ,Write 옵션  System->gpu로 이동할 때 사용할 버퍼
          //ComPtr<ID3D11Buffer>            m_SB_Read; // Stage,Read 옵션      gpu->System로 이동할 때 사용할 버퍼
      
          ComPtr<ID3D11ShaderResourceView>  m_SRV;    //READ_ONLY타입일 때 , 이 View를 사용
          ComPtr<ID3D11UnorderedAccessView> m_UAV;    //Read_Write타입일 때 , 이 뷰를 만들어서 전달해야됨
      
          D3D11_BUFFER_DESC                m_desc;
      
          
          UINT                             m_iElementSize;     // 버퍼의 사이즈
          UINT                             m_iElementCount;    // 버퍼 원소(tTileData) 개수
      
          SB_TYPE                          m_eType;
          bool                             m_bCpuRead; // System에서 gpu로 보낼때와 gpu에서 System로 읽어드릴때의 버퍼 총 2개가 필요함.
      
          
      public:
          UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }
      
      public:
          int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);//사이즈,개수,타입,Cpu접근,초기데이터
          void SetData(void* _pSrc, UINT _iElementCount); //데이터의 시작주소 , 원소의 개수 , gpu로 보냄
      
          // PIPELINE_STAGE
          void UpdateData(PIPELINE_STAGE _iStage, UINT _iRegisterNum);//레지스터로 바인딩
      public:
          CLONE_DISABLE(CStructuredBuffer);
      
      public:
          CStructuredBuffer();
          ~CStructuredBuffer();
      };
      



StructuredBuffer Cpp
====================

        #include "pch.h"
        #include "CStructuredBuffer.h"
        
        #include "CDevice.h"
        
        CStructuredBuffer::CStructuredBuffer()
            : m_desc{}
            , m_iElementCount(0)
            , m_iElementSize(0)
            , m_eType(SB_TYPE::READ_ONLY)
        {
        }
        
        CStructuredBuffer::~CStructuredBuffer()
        {
        }
        
        
        int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData)
        {
            m_SB = nullptr;  //ComPtr의 변수가 nullptr가 되면 기존에 있던 값을 지우고 재생성함. (생성될 때마다 초기화시켜서 재사용)
            m_SRV = nullptr;
        
        
            m_iElementSize = _iElementSize;
            m_iElementCount = _iElementCount;
            m_eType = _eType;
        
            
            // Read_Only : PipeLine 기준으로 바인딩된 리소스를 읽지만 수정하지는 못함(쉐이더)
            // Read_Write : PipeLine 기준으로 바인딩된 리소스를 읽고 수정도 가능함.(쉐이더),Compute Shader에서만 사용가능한 UnorderAccessView로 수정해야됨.
        
            // Cpu Access : 구조화버퍼는 gpu메모리에 있는데 System메모리쪽으로 가지고 올 수 있냐 없냐를 판단하는 변수
            
            // 총4가지 경우의수가 있음
            // Read_Only -> Cpu Access :true, false
            // Read_Write -> Cpu Access : true ,false
            
        
            m_desc.ByteWidth = m_iElementSize * m_iElementCount;
            m_desc.BindFlags=D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE; // 쉐이더 리소스,텍스쳐 레지스터에 바인딩할 때 중간매개체로 전달
            m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 추가 플래그,Texture2D버퍼를 이용해서 Texutre이라고 착각할 수 있으므로 구조화버퍼라고 용도를 알려줌
            m_desc.StructureByteStride = m_iElementSize;//구조화 버퍼에서만 사용, 한칸단위의 크기를 알려줌,시작으로부터 메모리 접근을 위해 한칸당 사이즈가 필요
        
            // Usage가 Stage고 Flags가 Read가 gpu에서 메모리로 읽어드리는 옵션
        
            m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
            m_desc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE; // System메모리에서 gpu접근
        
            
            
            
            // 초기 데이터(Initial Data)가 있는 경우
            if (nullptr != _pInitialData) {
                D3D11_SUBRESOURCE_DATA tSub = {};
                tSub.pSysMem = _pInitialData;
                DEVICE->CreateBuffer(&m_desc,&tSub ,m_SB.GetAddressOf());
            }
            else //초기 데이터가 없는 경우
            {
                DEVICE->CreateBuffer(&m_desc,nullptr,m_SB.GetAddressOf());
            }
        
            // Shader Resource View 생성
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        
            srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER; // 정체, 이 값으로 구조체를 설정(안은 유니온으로 구성됨)
            srvDesc.BufferEx.NumElements = m_iElementCount;// BufferDesc의 개수
        
            if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &srvDesc, m_SRV.GetAddressOf())))
            {
                return E_FAIL;
            }
        
            return S_OK;
        }
        
        void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
        {
            D3D11_MAPPED_SUBRESOURCE tSub = {};
        
            //버퍼를 gpu로 이전
            CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
            memcpy(tSub.pData, _pSrc, (size_t)m_iElementSize * (size_t)_iElementCount);
            CONTEXT->Unmap(m_SB.Get(), 0);
        }
        
        void CStructuredBuffer::UpdateData(PIPELINE_STAGE _iStage, UINT _iRegisterNum)
        {
            //레지스터, 개수,버퍼를 설명해주는 쉐이더리소스뷰 전달
            if (_iStage & PIPELINE_STAGE::VS)
            {
                CONTEXT->VSGetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf()); 
            }
        
            if (_iStage & PIPELINE_STAGE::HS)
            {
                CONTEXT->HSGetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf()); 
            }
        
            if (_iStage & PIPELINE_STAGE::DS)
            {
                CONTEXT->DSGetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf()); 
            }
        
            if (_iStage & PIPELINE_STAGE::GS)
            {
                CONTEXT->GSGetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf()); 
            }
        
            if (_iStage & PIPELINE_STAGE::PS)
            {
                CONTEXT->PSGetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf()); 
            }
        }
