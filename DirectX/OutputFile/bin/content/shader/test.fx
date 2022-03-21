#ifndef _TEST
#define _TEST

// Vertex Shader

	/*���̴��� ������ �ּҸ� �ް� ������ ���� ����(��üũ��)�� �𸣱� ������ �޾ƾߵ�
	���⼭ ������ ���������� �𸣱⶧���� semantic�� ���
	���� ������ �ø�ƽ ������ ���εǰ�����
	�׷��⿡ ������ �ٲ� ����� ����.

	Vertex Shader���� ������ ����ü�� �������� �ʴ� ������
	���߰��� ������ ũ�Ⱑ Ŀ���� Ư�� ����� �޾ƿ��� �ϱ� ���� semantic�� �����.*/

struct VTX_IN
{
	float3 vPos : POSITION;	// semantic : ���� �ȿ��� � ������ ������ ���������� ���ҵǴ����� �˷��ִ� �߰�����
	float4 vColor : COLOR;
};


struct VTX_OUT
{
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};


VTX_OUT VS_Test(VTX_IN _in)		//���� �ϳ��� ȣ��� �Լ� _in�� ����
{
	VTX_OUT output = (VTX_OUT)0.f;


	output.vPosition = float4(_in.vPos,1.f);
	output.vColor =_in.vColor;


	return output;
}


// Rasterizer
// ������ ������ ���� �ȿ� ������ �ȼ��� ���� (�ȼ� ���̴� �ĺ�)
// �ش� �ȼ��� ���� �ȼ� ���̴� ȣ��


float4 PS_Test(VTX_OUT _in) : SV_Target  // �ȼ����� ȣ��Ǵ� �Լ�
{
	float4 vOutColor = (float4) 0.f;

	vOutColor = _in.vColor;				 // �ȼ����� ��� ������ ���� �Ÿ����� ����ؼ� ����� �����ϼ��� �� ������ ���� ���� ������ �����ش�. (��������)

	return vOutColor;
}


// Pixel Shader










#endif

//���̴� ����	ȿ��f/x
//���̴� ��	5.0
//��ü ���� �����
