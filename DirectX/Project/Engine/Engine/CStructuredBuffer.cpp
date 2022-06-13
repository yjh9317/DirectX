#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_desc{}
    , m_iElementCount(0)
    , m_iElementSize(0)
    , m_iRecentBindNumSRV(-1)
    , m_iRecentBindNumUAV(-1)
    , m_eType(SB_TYPE::READ_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}


int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bCpuAccess, void* _pInitialData)
{
    m_SB = nullptr;  //ComPtr의 변수가 nullptr가 되면 기존에 있던 값을 지우고 재생성함.
    m_SRV = nullptr;


    m_iElementSize = _iElementSize;
    m_iElementCount = _iElementCount;
    m_eType = _eType;

    m_bCpuAccess = _bCpuAccess;
    // Read_Only : PipeLine 기준으로 바인딩된 리소스를 읽지만 수정하지는 못함(쉐이더)
    // Read_Write : PipeLine 기준으로 바인딩된 리소스를 읽고 수정도 가능함.(쉐이더),Compute Shader에서만 사용가능한 UnorderAccessView로 수정해야됨.

    // Cpu Access : 구조화버퍼는 gpu메모리에 있는데 System메모리쪽으로 가지고 올 수 있냐 없냐를 판단하는 변수
    
    // Read_Only -> Cpu Access :true, false
    // Read_Write -> Cpu Access : true ,false
    

    m_desc.ByteWidth = m_iElementSize * m_iElementCount;
 
    m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 추가 플래그,Texture2D버퍼를 이용해서 Texutre이라고 생각할 수 있어 구조화버퍼라고 선언.
    m_desc.StructureByteStride = m_iElementSize;//구조화 버퍼에서만 사용, 한칸단위의 크기를 알려줌,시작으로부터 메모리 접근을 위해 한칸당 사이즈가 필요

    // Usage가 Stage고 Flags가 Read가 gpu에서 메모리로 읽어드리는 옵션

    // Usage와 CPU를 이렇게 설정하면 system 메모리를 이용해서 gpu의 데이터를 덮어써서 메모리를 수정가능(단방향만)
    //m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    //m_desc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE; // System메모리에서 gpu접근

    m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    m_desc.CPUAccessFlags = 0;

        
    if (SB_TYPE::READ_ONLY == m_eType)
    {
        m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 쉐이더 리소스,텍스쳐 레지스터에 바인딩할 때 중간매개체로 전달
    }
    else
    {
        m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;    // Read,Write로 가능한 UAV도 추가
    }
        
    // 초기 Initial Data 가 있는 경우
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

    // Shader Resource View 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

    srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER; // 정체, 이 값으로 구조체를 설정(안은 유니온으로 구성됨)
    srvDesc.BufferEx.NumElements = m_iElementCount;// BufferDesc의 개수

    if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &srvDesc, m_SRV.GetAddressOf())))
    {
        return E_FAIL;
    }

    // SB_TYPE 이 Read_Write
    if (SB_TYPE::READ_WRITE == m_eType)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.NumElements = m_iElementCount;

        if (FAILED(DEVICE->CreateUnorderedAccessView(m_SB.Get(), &uavDesc, m_UAV.GetAddressOf())))
        {
            return E_FAIL;
        }
    }


    if (m_bCpuAccess)
    {
        // SysMem -> GPU Buffer
        m_desc_write.ByteWidth = m_iElementSize * m_iElementCount;
        m_desc_write.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        m_desc_write.StructureByteStride = m_iElementSize;

        m_desc_write.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
        m_desc_write.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

        m_desc_write.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        // 초기 Initial Data 가 있는 경우
        if (nullptr != _pInitialData)
        {
            D3D11_SUBRESOURCE_DATA tSub = {};
            tSub.pSysMem = _pInitialData;
            DEVICE->CreateBuffer(&m_desc_write, &tSub, m_SB_Write.GetAddressOf());
        }
        else
        {
            DEVICE->CreateBuffer(&m_desc_write, nullptr, m_SB_Write.GetAddressOf());
        }

        // GPU -> SysMem Buffer
        m_desc_read.ByteWidth = m_iElementSize * m_iElementCount;
        m_desc_read.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        m_desc_read.StructureByteStride = m_iElementSize;

         m_desc_read.Usage = D3D11_USAGE::D3D11_USAGE_STAGING;
        // STAGING은 Only 읽기 모드이기에 BindFlags는 주면 안된다.
        
        m_desc_read.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;

        // m_desc_read.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        //m_desc_read.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        m_desc_read.BindFlags = 0;

        // 초기 Initial Data 가 있는 경우
        if (nullptr != _pInitialData)
        {
            D3D11_SUBRESOURCE_DATA tSub = {};
            tSub.pSysMem = _pInitialData;
            DEVICE->CreateBuffer(&m_desc_read, &tSub, m_SB_Read.GetAddressOf());
        }
        else
        {
            DEVICE->CreateBuffer(&m_desc_read, nullptr, m_SB_Read.GetAddressOf());
        }
    }

    return S_OK;
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
{
    assert(m_bCpuAccess);

    D3D11_MAPPED_SUBRESOURCE tSub = {};

    // SysMem -> Wirte Buffer
    CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
    memcpy(tSub.pData, _pSrc, (size_t)m_iElementSize * (size_t)_iElementCount);
    CONTEXT->Unmap(m_SB_Write.Get(), 0);

    // Write Buffer -> Main Buffer
    CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _pDst)
{
    assert(m_bCpuAccess);
    // Main Buffer -> Read Buffer
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

    //D3D11_MAP_READ : tSub의 pData에 주소를 넣어줌

    // Read Buffer -> SysMem
    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tSub);
    memcpy(_pDst, tSub.pData, (size_t)m_iElementSize * (size_t)m_iElementCount);
    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}

void CStructuredBuffer::UpdateData(UINT _iStage, UINT _iRegisterNum)
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
     
    m_iRecentBindNumSRV = _iRegisterNum;
    // u레지스터이지만 텍스쳐레지스터로 사용하여 리소스처럼 사용
}

void CStructuredBuffer::UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource)
{
    // _bShaderResource의 값에 따라 SRV를 만들어 t 레지스터에 바인딩 or UAV를 만들어 u 레지스터에 바인딩

    if (_bShaderResource)
    {
        m_iRecentBindNumSRV = _iRegisterNum;
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
    }
    else
    {
        assert(m_UAV.Get());

        m_iRecentBindNumUAV = _iRegisterNum;
        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
    }
}

void CStructuredBuffer::Clear()
{
    if (-1 != m_iRecentBindNumSRV)
    {
        ID3D11ShaderResourceView* pSRV = nullptr;
        CONTEXT->VSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        CONTEXT->HSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        CONTEXT->DSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        CONTEXT->GSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        CONTEXT->PSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        CONTEXT->CSSetShaderResources(m_iRecentBindNumSRV, 1, &pSRV);
        m_iRecentBindNumSRV = -1;
    }

    if (-1 != m_iRecentBindNumUAV)
    {
        ID3D11UnorderedAccessView* pUAV = nullptr;
        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(m_iRecentBindNumUAV, 1, &pUAV, &i);
        m_iRecentBindNumUAV = -1;
    }
}