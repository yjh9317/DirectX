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
    // 배정받은 쓰레드
    UINT                             m_iGroupX;
    UINT                             m_iGroupY;
    UINT                             m_iGroupZ;

    // 그룹당 쓰레드 개수
    UINT                            m_iGroupPerThreadCountX;
    UINT                            m_iGroupPerThreadCountY;
    UINT                            m_iGroupPerThreadCountZ;

    tScalarParam                    m_Param; // 컴퓨터 쉐이더에서 사용할 상수값

public:
    void Excute();      // 실행
    void Excute(UINT _GroundX, UINT _GrounY, UINT GroupZ);

protected:
    virtual void UpdateData()=0; // 필요한 자원들을 레지스터에 바인딩
    virtual void Clear()=0;      // 끝나고 나서 바인딩된 리소스들 연결 끊기
    
    

public:
    int CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc);

public:
    CComputeShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CComputeShader();
};

