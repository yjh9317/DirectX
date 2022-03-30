#pragma once

#include "CMaterial.h"
#include "CMesh.h"
//#include "CTexture.h"
#include "CGraphicsShader.h"
//#include "CComputeShader.h"
//#include "CSound.h"

//#include "CMeshData.h"


class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*>			m_Res[(UINT)RES_TYPE::END]; //���ҽ����� ������ ������ ����

public:
	void init();

private:
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineShader();
	void CreateEngineMaterial();
	void MakeInputLayoutInfo();
	
public:
	template<typename type>
	RES_TYPE GetResType();

	template<typename type>
	type* FindRes(const wstring& _strKey);

	template<typename type>	// ���ҽ����� ���� ���� �ٸ��Ƿ� ���ø�
	void AddRes(const wstring& _strKey,type* _pRes);

};


template<typename type>
inline RES_TYPE CResMgr::GetResType()
{
	const type_info& info = typeid(type);	//Ÿ���� ������ �˾� �� �� �ִ�

	//	typeid��  �ڷ����̳� ���� �Ǵ� ���� �Է¹޾� const type_info& ������ ��ü�� ��ȯ�� �ִ� ������


	if (info.hash_code() == typeid(CMesh).hash_code())	//Ÿ���� �ؽ��ڵ� ��
		return RES_TYPE::MESH;
	else if (info.hash_code() == typeid(CGraphicsShader).hash_code())
		return RES_TYPE::GRAPHICS_SHADER;
	else if (info.hash_code() == typeid(CMaterial).hash_code())
		return RES_TYPE::MATERIAL;
	//else if (info.hash_code() == typeid(CMesh).hash_code())
	//	return RES_TYPE::MESH;
	//else if (info.hash_code() == typeid(CMesh).hash_code())
	//	return RES_TYPE::MESH;


	return RES_TYPE::END;
}

template<typename type>
type* CResMgr::FindRes(const wstring& _strKey)
{
	RES_TYPE eType = GetResType<type>();

	map<wstring,CRes*>::iterator iter=	m_Res[(UINT)eType].find(_strKey);

	if (iter == m_Res[(UINT)eType].end())
		return nullptr;


	return (type*)iter->second;
}

template<typename type>
void CResMgr::AddRes(const wstring& _strKey, type* _pRes)
{
	RES_TYPE eType = GetResType<type>();

	CRes* pRes = FindRes<type>(_strKey);

	assert(nullptr == pRes); //null�� �ƴϸ� �̹� �ִٴ°� �̹Ƿ� �Ʈ

		
	m_Res[(UINT)eType].insert(make_pair(_strKey,_pRes));
}
