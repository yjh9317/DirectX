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

    SB_TYPE                          m_eType;
    bool                             m_bCpuRead; // 버퍼에서 gpu로 보낼때와 gpu에서 버퍼로 읽어드릴때의 버퍼 총 2개가 필요함.

    
public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }

public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);//사이즈,개수,타입,Cpu접근,초기데이터

public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

