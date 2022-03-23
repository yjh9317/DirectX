#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"


#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CConstBuffer.h"


// ���� ����
Vertex arrVtx[4] = {};
Vec4   g_vPos;


CMesh* g_pMesh = nullptr;
CGraphicsShader* g_pShader = nullptr;
CConstBuffer* g_pCB = nullptr;

void TestInit()
{
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.2f, 1.f, 0.2f, 1.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.2f, 0.2f, 1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);

	UINT arrIdx[6] = { 0,2,3,0,1,2 };

	g_pMesh = new CMesh;
	g_pMesh->Create(arrVtx, 4, arrIdx, 6);


	// ������� �����
	g_pCB = new CConstBuffer(CB_TYPE::TRANSFORM);
	g_pCB->Create(sizeof(Vec4));


	// ���� �Է� ���� (InputLayout ����)	
	UINT iOffset = 0;
	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};

	tInputDesc.SemanticName = "POSITION";	// Semantic �̸�
	tInputDesc.SemanticIndex = 0;			// �ߺ� �̸��� ��� �ε����� ����
	tInputDesc.InputSlot = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName = "COLOR";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 16;

	CGraphicsShader::AddInputLayout(tInputDesc);


	g_pShader = new CGraphicsShader;
	g_pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	g_pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

}

void TestUpdate()
{
	if (KEY_PRESSED(KEY::LEFT))
	{
		g_vPos.x -= DT * 0.5f;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		g_vPos.x += DT * 0.5f;
	}

	g_pCB->SetData(&g_vPos, sizeof(Vec4));
}

void TestRender()
{
	CDevice::GetInst()->ClearTarget();

	// render	 		
	g_pShader->UpdateData();	// ����� ���̴�
	g_pCB->UpdateData();		// b0 �� ������ ���
	g_pMesh->render();			// ����� ����(�޽�)

	CDevice::GetInst()->Present();
}

void TestRelease()
{
	SAFE_DELETE(g_pMesh);
	SAFE_DELETE(g_pShader);
	SAFE_DELETE(g_pCB);
}
