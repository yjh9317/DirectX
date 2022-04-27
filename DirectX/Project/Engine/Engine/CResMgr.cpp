#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Del_Map(m_Res[i]);
	}
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag)
{
	assert(nullptr ==  FindRes<CTexture>(_strKey));
	
	

	CTexture* pTexture = new CTexture;

	pTexture->Create(_iWidth, _iHeight, _format, _flag);
	pTexture->SetKey(_strKey);

	AddRes<CTexture>(_strKey, pTexture);

	return pTexture;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D)
{
	assert(nullptr == FindRes<CTexture>(_strKey));



	CTexture* pTexture = new CTexture;

	pTexture->Create(_pTex2D);
	pTexture->SetKey(_strKey);

	AddRes<CTexture>(_strKey, pTexture);

	return pTexture;
}

