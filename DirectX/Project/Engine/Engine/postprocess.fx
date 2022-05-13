#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// =====================================
// PostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite
#define PostProcessTarget   g_tex_0
#define IsBind              g_btex_0
// =====================================
VTX_OUT VS_PostProcess(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2, 1.f);
    // RectMesh�� ���� �����̽��� ��ǥ�� -0.5 ~ 0.5�� �������µ�
    // �� ���� 2���ؼ� -1 ~ 1�� ������༭ NDC�� �����ϰ� ���������ν�
    // ��ó�� ����� ȭ�����ο� ����ϰ� �����.
    
    // output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    // ���� output.vPosition�� ������ ����ϰ� ī�޶� �¿�� �����δٸ�
    // ���� ������ �����. �� ������ ������ ���� ����� �ٽ� ��ó�� ȿ���� �־��ְ�
    // ������ ���� ����� �ٽ� ����Ÿ�ٿ� ���鼭 1. ���� ����� 2. ������ ���� ��ó�� ����� ������ ���ÿ� �����.
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (IsBind)
    {
        vOutColor = PostProcessTarget.Sample(g_sam_0, _in.vUV);
        vOutColor.r *= 2.f;
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return vOutColor;
}






    /*
    ��ó��
    	���������δܰ��߿��� ����Ÿ���� ���� ��� Ÿ���̸鼭 ���ÿ� ���ҽ��� ����� �� ����
    	�׷��� ����Ÿ�ٰ� ������ ���纻�� ���� ����Ѵ�.
    	�ȼ����̴����� ���� �׸��������� ���纻�� ����Ÿ���� ������ ������.�׷��� �� ���纻�� �����Ͽ� ����Ÿ�ٿ� �߰����� ó��
    	ex) ĳ���Ͱ� ������ ȭ���� �������� ȿ�������� ��ó���� �Ѵ�.
        ��� �������� ������ ������ ��ó���� ����

		PostProcess ��ü�� ���������� �纹�縦 �ؾ���, �ֳ��ϸ� ��ó���ϰ� PostProcess�� �ٽ� RenderTarget���� ������ؾ��ϱ� ������
		RenderTarget�� PostProcess �� ����->����� PostProcess�� ��ó�� ȿ�� ���� -> ����� PostProcess�� RenderTarget�� �ٽ� �����
    */


#endif
