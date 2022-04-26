#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
    : m_pProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _pProtoObj)
    : m_pProtoObj(_pProtoObj)
{
}

CPrefab::~CPrefab()
{
    SAFE_DELETE(m_pProtoObj);
}

CGameObject* CPrefab::Instantiate()
{
    // ��üȭ�� ���� ���� 
    // ó�� ���ҽ��� �ʿ�� �� �� �ε��ϴ� ������ ������ Ŭ�������� �̸� ������ü�� �ε�
    // �̸� �ε��ð��� Ȯ���ؼ� ���� ���ҽ��� �ε��ϰ� �ΰ��ӿ����� ���ҽ��� ã�Ƽ� ����ؼ� �ΰ��ӵ��߿� �ε����� �����Ƿ� ���� �ɸ��� �ʴ´�
    return m_pProtoObj->Clone();
}