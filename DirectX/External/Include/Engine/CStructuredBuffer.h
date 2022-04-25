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

    SB_TYPE                          m_eType;
    bool                             m_bCpuRead; // ���ۿ��� gpu�� �������� gpu���� ���۷� �о�帱���� ���� �� 2���� �ʿ���.

    
public:
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }

public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccessRead, void* _pInitialData);//������,����,Ÿ��,Cpu����,�ʱⵥ����

public:
    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

