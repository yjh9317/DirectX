#pragma once

class CScene;
class CGameObject;

// Engine�� ��� class�� �θ��� CEntity�� �����Լ� SaveToScene,LoadFromScene�� �������� 
// �� �ؿ� Ŭ���������� �θ��� �����Լ��� �������̵��ؼ� �� Ŭ�������� �����ؾ��� �������� �����ְ� �ݺ��ϴ� ����.

// SaveScene������ �����ؾ��� Scene,Layer�� �� Ŭ������ ȣ���ϰ� �� ������ ȣ����� Ŭ���� �ȿ��� �ۼ�
// LoadScene�� SaveScene�� ������ ����
class CSceneSaveLoad							
{
public:
	// Save
	static void SaveScene(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);

	// Load
	static CScene* LoadScene(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObject(FILE* _pFile);
};

