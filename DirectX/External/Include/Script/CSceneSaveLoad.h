#pragma once

class CScene;
class CGameObject;
class CPrefab;
// Engine의 모든 class의 부모인 CEntity에 가상함수 SaveToScene,LoadFromScene를 만들어놓고 
// 그 밑에 클래스에서는 부모의 가상함수를 오버라이딩해서 그 클래스에서 저장해야할 변수들을 적어주고 반복하는 형태.

// SaveScene에서는 저장해야할 Scene,Layer등 각 클래스에 호출하고 그 내용은 호출받은 클래스 안에서 작성
// LoadScene도 SaveScene과 형식이 동일
class CSceneSaveLoad							
{
public:
	// Save
	static void SaveScene(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);
	static void SavePrefab(CPrefab* _Prefab, const wstring& _strFilePath);

	// Load
	static CScene* LoadScene(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObject(FILE* _pFile);
	static int LoadPrefab(CPrefab* _Prefab, const wstring& _strFilePath);
};

