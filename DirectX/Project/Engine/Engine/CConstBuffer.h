// 상수 버퍼자체는 gpu메모리를 사용하지만 실제 파이프라인일 때
// 특정 레지스터에 상수값을 보내야 하는데 몇번 레지스터를 보낼지 모르기에
// 미리 만들어 놓는다.

#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>		m_CB;
    D3D11_BUFFER_DESC           m_Desc;

    CB_TYPE                     m_eCBType; // 상수버퍼를 바인딩 할 레지스터 번호

public:
    virtual void UpdateData() override;
    int Create(UINT _iBufferSize);

    void SetData(void* _pData, UINT _iSize);
        
    virtual CConstBuffer* Clone() { return nullptr; }

public:
    CConstBuffer(CB_TYPE _eType);
    ~CConstBuffer();
};

