#ifndef _TEST
#define _TEST

//������� �������� , b0:���Գѹ�(�������� ��ȣ)

//�������� ��ȣ�� ������۸� 1:1�� ��Ī

cbuffer TRANSFORM : register(b0)
{
    float4 g_Pos; //������ ��(�̵���)
	
}



struct VTX_IN
{
	float3 vPos : POSITION;	// semantic
	float4 vColor : COLOR;
};


struct VTX_OUT
{
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};


VTX_OUT VS_Test(VTX_IN _in)		//���� �ϳ��� ȣ��� �Լ�
{
	VTX_OUT output = (VTX_OUT)0.f;
	
    float3 vFinalPos = _in.vPos + g_Pos.xyz; //���ؽ� ���� + ��������� �̵���

    output.vPosition = float4(vFinalPos, 1.f);
	output.vColor =_in.vColor;


	return output;
}


// Rasterizer
// ������ ������ ���� �ȿ� ������ �ȼ��� ����
// �ش� �ȼ��� ���� �ȼ� ���̴� ȣ��


float4 PS_Test(VTX_OUT _in) : SV_Target  // �ȼ����� ȣ��Ǵ� �Լ�
{
	float4 vOutColor = (float4) 0.f;

	vOutColor = _in.vColor;				 // �ȼ����� ��� ������ ���� �Ÿ����� ����ؼ� ����� �����ϼ��� �� ������ ���� ���� ������ �����ش�. (��������)

	return vOutColor;
}





#endif

//������Ʈ �Ӽ�
//���̴� ����	ȿ��f/x
//���̴� ��	5.0
//��ü ���� �����
