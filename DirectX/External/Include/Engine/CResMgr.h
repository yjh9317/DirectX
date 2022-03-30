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
	map<wstring, CRes*>			m_Res[(UINT)RES_TYPE::END]; //리소스마다 각자의 맵으로 관리

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

	template<typename type>	// 리소스마다 각자 맵이 다르므로 템플릿
	void AddRes(const wstring& _strKey,type* _pRes);

};


template<typename type>
inline RES_TYPE CResMgr::GetResType()
{
	const type_info& info = typeid(type);	//타입의 정보를 알아 낼 수 있다

	//	typeid는  자료형이나 변수 또는 식을 입력받아 const type_info& 형식의 객체를 반환해 주는 연산자


	if (info.hash_code() == typeid(CMesh).hash_code())	//타입의 해쉬코드 비교
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

	assert(nullptr == pRes); //null이 아니면 이미 있다는거 이므로 어설트

		
	m_Res[(UINT)eType].insert(make_pair(_strKey,_pRes));
}
