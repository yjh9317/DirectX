#pragma once
#include "CShader.h"

#include "CDevice.h"

#include "CTexture.h"
#include "CStructuredBuffer.h"

class CComputeShader :
    public CShader
{
private:

    ComPtr<ID3DBlob>                 m_CSBlob;
    ComPtr<ID3D11ComputeShader>      m_CS;


protected:
    // �������� ������
    UINT                             m_iGroupX;
    UINT                             m_iGroupY;
    UINT                             m_iGroupZ;

    // �׷�� ������ ����
    UINT                            m_iGroupPerThreadCountX;
    UINT                            m_iGroupPerThreadCountY;
    UINT                            m_iGroupPerThreadCountZ;

    tScalarParam                    m_Param; // ��ǻ�� ���̴����� ����� �����

public:
    void Excute();      // ����
    void Excute(UINT _GroundX, UINT _GrounY, UINT GroupZ);

protected:
    virtual void UpdateData()=0; // �ʿ��� �ڿ����� �������Ϳ� ���ε�
    virtual void Clear()=0;      // ������ ���� ���ε��� ���ҽ��� ���� ����
    
    

public:
    int CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc);

public:
    CComputeShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CComputeShader();
};

