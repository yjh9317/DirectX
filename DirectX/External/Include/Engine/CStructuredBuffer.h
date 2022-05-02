#pragma once
#include "CEntity.h"

// ID3D11Buffer-> gpu�� ���۸� ��´�

//����ȭ ������ Ÿ�԰�
enum class SB_TYPE
{
    READ_ONLY,          // ���̴� �󿡼� �б�
    READ_WRITE,         // ���̴� �󿡼� �б�,����
};


class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>              m_SB;      // Dynamic ,Write �ɼ�  System->gpu
    //ComPtr<ID3D11Buffer>            m_SB_Read; // Stage,Read �ɼ�      gpu->System

    ComPtr<ID3D11ShaderResourceView>  m_SRV;
    ComPtr<ID3D11UnorderedAccessView> m_UAV;    //Read_WriteŸ���� �� , �� �並 ���� �����ؾߵ�

    D3D11_BUFFER_DESC                m_desc;

    
    UINT                             m_iElementSize;     // ������ ������
    UINT                             m_iElementCount;    // ���� ����(tTileData) ����
    UINT                             m_iRecentBindNumSRV;  // �ֱ� ���ε� �������� ��ȣ(t)
    UINT                             m_iRecentBindNumUAV;  // �ֱ� ���ε� �������� ��ȣ(u)

    SB_TYPE                          m_eType;
    bool                             m_bCpuRead; // System���� gpu�� �������� gpu���� System�� �о�帱���� ���� �� 2���� �ʿ���.

    
public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetElementSize() { return m_iElementSize; }
public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);//������,����,Ÿ��,Cpu����,�ʱⵥ����
    void SetData(void* _pSrc, UINT _iElementCount); //�������� �����ּ� , ������ ���� , gpu�� ����

    // PIPELINE_STAGE
    void UpdateData(UINT _iStage, UINT _iRegisterNum);//�������ͷ� ���ε�
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource);  //_bShaderResource�� true�� t��������, false�� u��������
    void Clear();

public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

