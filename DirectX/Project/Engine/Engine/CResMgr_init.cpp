#include "pch.h"
#include "CResMgr.h"

void CResMgr::init()
{
	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineShader();
	CreateEngineMaterial();
}

void CResMgr::CreateEngineMesh()
{
	CMesh* pMesh = nullptr;

	vector<Vtx>		vecVtx;
	vector<UINT>	vecIdx;

	Vtx v;

	// RectMesh
	// 0 ---- 1
	// |  \   | 
	// 3 ---- 2 
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.2f, 1.f, 0.2f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.2f, 0.2f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(),(UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);
	vecVtx.clear(); vecIdx.clear();

	// CircleMesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);	//�ؽ��� ��ǥ��(0,0 ~ 1,1)
	vecVtx.push_back(v);

	UINT iSliceCount = 50;	// ���� ������ ���� ������ ��� ���� ����(���ڸ������)
	float fRadius = 0.5f;	// ���� ������
	float fAngleStep = XM_2PI / float(iSliceCount); //���� ���� ( 360�� / iSliceCount )


	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		// 2�������� z�� 0 ����, x,y�� ���������� ���� ��ǥ�� (r*cos��,r*sin��)�� �̿��ؼ� ������ ���� ���� ��ǥ�� ���Ѵ�.
		v.vPos = Vec3(fRadius*cosf(fAngleStep*(float)i), fRadius * sinf(fAngleStep * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		// ���� uv�� ���� ���ϰ� �� ���� �ȿ� �ִ� �ȼ��� �ֺ� ������ ������ �޴´�.
		// ���� �߰��� 0.5�̹Ƿ� Local Space�� UV��ǥ(0~1)�� ���ؼ� ���.
		v.vUV = Vec2(v.vPos.x+0.5f,-v.vPos.y +0.5f);


		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		// ������ ������ ���� ���� ������ �ð�������� �ε������ۿ� ��´�.
		vecIdx.push_back(0);	
		vecIdx.push_back(i+2);
		vecIdx.push_back(i+1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);
	vecVtx.clear(); vecIdx.clear();


	// Cube

	// Sphere

	// Cylinder

	// Frustum


}

void CResMgr::CreateEngineTexture()
{
}

void CResMgr::CreateEngineShader()
{
	MakeInputLayoutInfo();

	CGraphicsShader* pShader = nullptr;

	// TestShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->AddScalarParamInfo(L"IsColorRed", SCALAR_PARAM::INT_0);

	AddRes<CGraphicsShader>(L"TestShader", pShader);
}

void CResMgr::CreateEngineMaterial()
{
	CMaterial* pMtrl = nullptr;

	// TestMtrl ����
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));

	AddRes<CMaterial>(L"TestMtrl", pMtrl);
}


void CResMgr::MakeInputLayoutInfo()
{
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

	tInputDesc.SemanticName = "TEXCOORD";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 8;

	CGraphicsShader::AddInputLayout(tInputDesc);

}
