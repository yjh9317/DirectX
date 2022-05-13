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
    // RectMesh는 로컬 스페이스의 좌표가 -0.5 ~ 0.5인데
    // 이 값을 2배해서 -1 ~ 1로 만들어줘서 NDC와 동일하게 맞춰줌으로써
    // 후처리 장면을 화면전부에 출력하게 만든다.
    
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
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
    후처리
    	파이프라인단계중에서 렌더타겟을 최종 출력 타겟이면서 동시에 리소스로 사용할 순 없다
    	그래서 렌더타겟과 동일한 복사본을 만든다
    	픽셀세이더에서 새로 그리기전까진 복사본과 렌더타겟은 완전히 동일함
    	그래서 그 복사본을 가공하여 렌더타겟에 추가적인 처리
    	ex) 캐릭터가 맞으면 화면이 빨개지는 효과같은걸 후처리로 한다.
                    모든 렌더링이 끝나고 나서야 후처리를 진행
    */


#endif
