#pragma once
#include "CShader.h"


class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                 m_CSBlob;
    ComPtr<ID3D11ComputeShader>      m_CS;

public:
    int CreateComputeShader(const wstring& _strRelativePath, const string& _strFunc);

public:
    CComputeShader();
    ~CComputeShader();
};

