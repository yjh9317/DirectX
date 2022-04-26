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
    // 객체화로 인한 장점 
    // 처음 리소스를 필요로 할 때 로딩하는 과정을 프리팹 클래스에서 미리 원본객체를 로딩
    // 미리 로딩시간을 확보해서 원형 리소스를 로딩하고 인게임에서는 리소스를 찾아서 사용해서 인게임도중에 로딩하지 않으므로 렉이 걸리지 않는다
    return m_pProtoObj->Clone();
}