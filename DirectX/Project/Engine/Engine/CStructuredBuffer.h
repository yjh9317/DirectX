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



    // 평소에는 nullptr이였다가 CpuAccess가 true라면 이 두개의 버퍼로 메인버퍼와 링크해서 값을 읽어오거나 복사해서 사용한다.
    // 이렇게 하는 이유는 Usage가 Dynamic이면서 CpuAccess가 true라면(Write) UAV로 바인딩하지 못하기 때문 
    // System메모리->Write버퍼 데이터 이동 -> 메인버퍼로 복사 -> UAV레지스터 바인딩
    // CS에서 수정 -> 메인버퍼 -> Read 버퍼로 데이터 이동 -> System 메모리로 복사
    ComPtr<ID3D11Buffer>                m_SB_Write;
    ComPtr<ID3D11Buffer>                m_SB_Read;

    D3D11_BUFFER_DESC                   m_desc_write;
    D3D11_BUFFER_DESC                   m_desc_read;

    
    UINT                             m_iElementSize;     // 버퍼의 사이즈
    UINT                             m_iElementCount;    // 버퍼 원소(tTileData) 개수

    int                             m_iRecentBindNumSRV;  // 최근 바인딩 레지스터 번호(t)
    int                             m_iRecentBindNumUAV;  // 최근 바인딩 레지스터 번호(u)

    SB_TYPE                          m_eType;
    bool                             m_bCpuAccess; // System에서 gpu로 보낼때와 gpu에서 System로 읽어드릴때의 버퍼 총 2개가 필요함.

    
public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetElementSize() { return m_iElementSize; }
public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccess, void* _pInitialData);//사이즈,개수,타입,Cpu접근,초기데이터
    void SetData(void* _pSrc, UINT _iElementCount); //데이터의 시작주소 , 원소의 개수 , gpu로 보냄
    void GetData(void* _pDst);

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

