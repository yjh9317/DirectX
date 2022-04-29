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
    m_SB = nullptr;  //ComPtr�� ������ nullptr�� �Ǹ� ������ �ִ� ���� ����� �������.
    m_SRV = nullptr;


    m_iElementSize = _iElementSize;
    m_iElementCount = _iElementCount;
    m_eType = _eType;

    
    // Read_Only : PipeLine �������� ���ε��� ���ҽ��� ������ ���������� ����(���̴�)
    // Read_Write : PipeLine �������� ���ε��� ���ҽ��� �а� ������ ������.(���̴�),Compute Shader������ ��밡���� UnorderAccessView�� �����ؾߵ�.

    // Cpu Access : ����ȭ���۴� gpu�޸𸮿� �ִµ� System�޸������� ������ �� �� �ֳ� ���ĸ� �Ǵ��ϴ� ����
    
    // Read_Only -> Cpu Access :true, false
    // Read_Write -> Cpu Access : true ,false
    

    m_desc.ByteWidth = m_iElementSize * m_iElementCount;
    m_desc.BindFlags=D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE; // ���̴� ���ҽ�,�ؽ��� �������Ϳ� ���ε��� �� �߰��Ű�ü�� ����
    m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �߰� �÷���,Texture2D���۸� �̿��ؼ� Texutre�̶�� ������ �� �־� ����ȭ���۶�� ����.
    m_desc.StructureByteStride = m_iElementSize;//����ȭ ���ۿ����� ���, ��ĭ������ ũ�⸦ �˷���,�������κ��� �޸� ������ ���� ��ĭ�� ����� �ʿ�

    // Usage�� Stage�� Flags�� Read�� gpu���� �޸𸮷� �о�帮�� �ɼ�

    m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    m_desc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE; // System�޸𸮿��� gpu����

    
    
    
    // �ʱ� Initial Data �� �ִ� ���
    if (nullptr != _pInitialData)
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _pInitialData;
        DEVICE->CreateBuffer(&m_desc, &tSub, m_SB.GetAddressOf());
    }
    else
    {
        DEVICE->CreateBuffer(&m_desc, nullptr, m_SB.GetAddressOf());
    }

    // Shader Resource View ����
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER; // ��ü, �� ������ ����ü�� ����(���� ���Ͽ����� ������)
    srvDesc.BufferEx.NumElements = m_iElementCount;// BufferDesc�� ����

    if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &srvDesc, m_SRV.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
{
    D3D11_MAPPED_SUBRESOURCE tSub = {};

    CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
    memcpy(tSub.pData, _pSrc, (size_t)m_iElementSize * (size_t)_iElementCount);
    CONTEXT->Unmap(m_SB.Get(), 0);
}

void CStructuredBuffer::UpdateData(PIPELINE_STAGE _iStage, UINT _iRegisterNum)
{
    if (_iStage & PIPELINE_STAGE::VS)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if (_iStage & PIPELINE_STAGE::HS)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if (_iStage & PIPELINE_STAGE::DS)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if (_iStage & PIPELINE_STAGE::GS)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if (_iStage & PIPELINE_STAGE::PS)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}
