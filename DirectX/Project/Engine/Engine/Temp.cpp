#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CConstBuffer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

CGameObject* g_pObj = nullptr;	//������Ʈ

void TestInit()
{
	g_pObj = new CGameObject;
	g_pObj->SetName(L"Player");

	g_pObj->AddComponent(new CTransform);
	g_pObj->AddComponent(new CMeshRender);

	g_pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));		
	g_pObj->MeshRender()->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"TestShader"));
}

void TestUpdate()
{
	g_pObj->update();
	g_pObj->lateupdate();
	g_pObj->finalupdate();

}

void TestRender()
{
	CDevice::GetInst()->ClearTarget();

	// render	 		

	//����� ���̴�
	CGraphicsShader* pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(L"TestShader");
	pShader->UpdateData();

	CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM)->UpdateData(); //b0�� ������ �������

	CMesh* pRectMesh=CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


	pRectMesh->render();	//����� ���� �޽�

	

	CDevice::GetInst()->Present();
}

void TestRelease()
{
	SAFE_DELETE(g_pObj);
}
