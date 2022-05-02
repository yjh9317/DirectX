#ifndef _TESTCS
#define _TESTCS

#include "value.fx"

// cpu�� ���ξ����尡 ���������� ���������� gpu�� ����ó���� �⺻
// Compute Shader������ �׷�� �����带 �������� ��� �� �� �ִ�.
// ����ڴ� �� �����带 ��� �� �������� �����ؼ� �׷� ������ ��� �� �� �ִ�.

// �׷� ������ ����
// �ִ� ������ ���� 1024

// �ø�ƽ ����
// Dispatch : Compute Shader�� �������ִ� �Լ�

// SV_GroupThreadID     : �׷� �������� ������ �ε��� 
// SV_GroupIndex        : �׷� �������� ������ �ε����� 1���� ������ ��ȯ

// SV_GroupID           : �����尡 ���� �׷��� �ε��� 
// SV_DispatchThreadID  : ��� �����带 �������� ���� ���� �ε���

// ���� : �б����� ����� �ǹ��ϴ°� SV_GroupID, �� �ݾȿ��� �ڱ� ��ȣ�� �ǹ��ϴ°� SV_GroupThreadID
// GroupIndex�� GroupThreadID�� 1���� �迭�� ���� �ε����� ���� ��

// SV_DispatchThreadID�� (GroupID)*(�׷� ������ x,y,z) + GroupThreadID, �׷� ������(10,8,3)�̸� �� 10*8*3 = 240���� �����带 ����
// ex) [(2,1,0) * (10,8,3)] + (7,5,0) = (27,13,0) [] ���� �� �ೢ�� ���Ѵ�.
// �׷� ������ �߿��� (2,1,0)���̰� �� �׷쾲������ ������ (10*8*3)�̰� �� �ȿ��� �������� ��ȣ�� (7,5,0)


// SV_DispatchThreadID�� �����帶�� ���� �ٸ��Ƿ� ���� Ȱ��ȴ�.

RWTexture2D<float4> g_RWTex_0 : register(u0);

#define WIDTH g_int_0
#define HEIGHT g_int_1

#define Color g_vec4_0


[numthreads(32, 32, 1)] // 3���� ����ȭ , x :32,y :32 ,z :1 
void CS_Test(int3 _id : SV_DispatchThreadID) //������� �ϳ��� �ȼ��� ���߸� �������� Dispatch���� �� �ȼ� �ε���
{
    // value.fx�ȿ� �ִ� ������ۿ��� ����� �����ͼ� ���
    
    if (_id.x < 0 || WIDTH <= _id.x ||  _id.y < 0 || HEIGHT <= _id.y )
        return; // �� �ȼ��� ���� �ۿ� �ִ� �ȼ��̹Ƿ� return
    
    
    g_RWTex_0[_id.xy] = Color;
}

#endif

// Bilboard Effect : �ϼ��Ǿ� �ִ� �̹����� �ִϸ��̼����� ����
// ���� : ����ȭ�� ���� , ���� : ����Ƽ�� ������

// Particle Effect : ���ڸ��� ������ �ϳ��� ����ؼ� ����ó���� ǥ��
// ���� : �������� ����Ƽ�� ��������. ����: ����� ���� ��� ����