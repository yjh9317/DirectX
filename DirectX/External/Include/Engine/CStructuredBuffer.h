#pragma once
#include "CEntity.h"

// ID3D11Buffer-> gpu에 버퍼를 잡는다

//구조화 버퍼의 타입값
enum class SB_TYPE
{
    READ_ONLY,          // 쉐이더 상에서 읽기
    READ_WRITE,         // 쉐이더 상에서 읽기,쓰기
};


class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>              m_SB;      // Dynamic ,Write 옵션  System->gpu
    //ComPtr<ID3D11Buffer>            m_SB_Read; // Stage,Read 옵션      gpu->System

    ComPtr<ID3D11ShaderResourceView>  m_SRV;
    ComPtr<ID3D11UnorderedAccessView> m_UAV;    //Read_Write타입일 때 , 이 뷰를 만들어서 전달해야됨

    D3D11_BUFFER_DESC                m_desc;

    
    UINT                             m_iElementSize;     // 버퍼의 사이즈
    UINT                             m_iElementCount;    // 버퍼 원소(tTileData) 개수
    UINT                             m_iRecentBindNumSRV;  // 최근 바인딩 레지스터 번호(t)
    UINT                             m_iRecentBindNumUAV;  // 최근 바인딩 레지스터 번호(u)

    SB_TYPE                          m_eType;
    bool                             m_bCpuRead; // System에서 gpu로 보낼때와 gpu에서 System로 읽어드릴때의 버퍼 총 2개가 필요함.

    
public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetElementSize() { return m_iElementSize; }
public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);//사이즈,개수,타입,Cpu접근,초기데이터
    void SetData(void* _pSrc, UINT _iElementCount); //데이터의 시작주소 , 원소의 개수 , gpu로 보냄

    // PIPELINE_STAGE
    void UpdateData(UINT _iStage, UINT _iRegisterNum);//레지스터로 바인딩
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource);  //_bShaderResource가 true면 t레지스터, false면 u레지스터
    void Clear();

public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

