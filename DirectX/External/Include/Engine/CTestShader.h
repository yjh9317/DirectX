#pragma once
#include "CComputeShader.h"



class CTestShader :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_pOutputTex;
    Vec4            m_vColor;


public:
    void SetOutputTexture(Ptr<CTexture> _pOutTex) { m_pOutputTex = _pOutTex; }
    void SetColor(Vec4 _vColor) { m_vColor = _vColor; }


    virtual void UpdateData();
    virtual void Clear();


public:
    CTestShader();
    ~CTestShader();
};

