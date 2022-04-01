#pragma once
#include "CRes.h"

#include "CGraphicsShader.h"
#include "CTexture.h"

// 재질을 설정한다 = 어떤식으로 렌더링할 것인지 (셰이더포함)
class CMaterial :
    public CRes
{
private:
    tScalarParam                m_Param;
    Ptr<CTexture>               m_arrTex[(UINT)TEX_PARAM::END];
    Ptr<CGraphicsShader>        m_pShader;

    vector<tScalarParamInfo>    m_vecScalarParamInfo;
    vector<tTexParamInfo>       m_vecTexParamInfo;

public:
    void SetShader(Ptr<CGraphicsShader> _pShader);
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }

    void SetScalarParam(SCALAR_PARAM _eType,void* _pData);  // 상수버퍼에 추가적인 인자 전달
    void SetScalarParam(const wstring& _strParamName, void* _pData);

    void SetTexParam(TEX_PARAM _eType, Ptr<CTexture> _pTex);
    void SetTexParam(const wstring& _strParamName, Ptr<CTexture> _pTex);

    void UpdateData();

public:
    virtual int Load(const wstring& _strFilePath) override { return S_OK; }
    

public:
    CMaterial();
    ~CMaterial();
};

