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
    
    //output.vPosition = float4(_in.vPos * 2, 1.f); //����ó�� ����ϱ� ���ؼ��� ȭ����ü
    
    // RectMesh�� ���� �����̽��� ��ǥ�� -0.5 ~ 0.5�� �������µ�
    // �� ���� 2���ؼ� -1 ~ 1�� ������༭ NDC�� �����ϰ� ���������ν�
    // ��ó�� ����� ȭ�����ο� ����ϰ� �����.
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);

    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // VS���� PS�� ���� ���� ���� �����ؼ� ���������� 
    // _in.vPosition; // SV_Position �ø�ƽ�� ����ϸ� PS�� �� �� �ȼ� ��ǥ�� �־��ش�.
    
    // ȭ���� �����κи��� ��ó�� ȿ���� �־��ֱ� ���ؼ��� ����ȭ���� UV���� �˾ƾ� �Ѵ�.
    // �� ���� ���ϱ� ���� �� �ȼ���ġ���� ���ڷ� �ϰ� ȭ�� �ػ󵵸� �и�� �ϸ� ���� ���� �� �� �ִ�.
    float2 vScreenUV = _in.vPosition.xy / vResolution;
    
   
    
    if (IsBind)
    {
        //1��
        vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (fAccTime * 4.f)) * 0.01;
        
        //2�� Wrap��带 ����ϸ� �Ѿ�ºκе� ���ø��� �ؼ� ������ ����.
        //_in.vUV.x += fAccTime * 0.1f;
        //vScreenUV += g_noise_cloud.Sample(g_sam_1,_in.vUV).rg * 0.03f;
        //vScreenUV = saturate(vScreenUV); // saturate : 0~1 ���� �����ΰ��� ���������ִ°��ϵ�?
        
        //postprocess distortion shader�˻��ϸ� ����������
        
        vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
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
