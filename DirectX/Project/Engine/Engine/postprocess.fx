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
    
    //output.vPosition = float4(_in.vPos * 2, 1.f); //필터처럼 사용하기 위해서는 화면전체
    
    // RectMesh는 로컬 스페이스의 좌표가 -0.5 ~ 0.5로 잡아줬었는데
    // 이 값을 2배해서 -1 ~ 1로 만들어줘서 NDC와 동일하게 맞춰줌으로써
    // 후처리 장면을 화면전부에 출력하게 만든다.
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);

    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // VS에서 PS로 들어올 때는 보통 보간해서 가져오지만 
    // _in.vPosition; // SV_Position 시맨틱을 사용하면 PS로 들어갈 때 픽셀 좌표를 넣어준다.
    
    // 화면의 일정부분만을 후처리 효과를 넣어주기 위해서는 일정화면의 UV값을 알아야 한다.
    // 그 값을 구하기 위해 그 픽셀위치값을 분자로 하고 화면 해상도를 분모로 하면 비율 값을 알 수 있다.
    float2 vScreenUV = _in.vPosition.xy / vResolution;
    
   
    
    if (IsBind)
    {
        //1번
        vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (fAccTime * 4.f)) * 0.01;
        
        //2번 Wrap모드를 사용하면 넘어가는부분도 샘플링을 해서 문제가 생김.
        //_in.vUV.x += fAccTime * 0.1f;
        //vScreenUV += g_noise_cloud.Sample(g_sam_1,_in.vUV).rg * 0.03f;
        //vScreenUV = saturate(vScreenUV); // saturate : 0~1 범위 밖으로가면 고정시켜주는거일듯?
        
        //postprocess distortion shader검색하면 여러개있음
        
        vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
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
    	그래서 렌더타겟과 동일한 복사본을 만들어서 사용한다.
    	픽셀세이더에서 새로 그리기전까진 복사본과 렌더타겟은 완전히 동일함.그래서 그 복사본을 가공하여 렌더타겟에 추가적인 처리
    	ex) 캐릭터가 맞으면 화면이 빨개지는 효과같은걸 후처리로 한다.
        모든 렌더링이 끝나고 나서야 후처리를 진행

		PostProcess 물체가 있을때마다 재복사를 해야함, 왜냐하면 후처리하고난 PostProcess를 다시 RenderTarget으로 재출력해야하기 때문에
		RenderTarget를 PostProcess 에 복사->복사된 PostProcess에 후처리 효과 넣음 -> 복사된 PostProcess를 RenderTarget에 다시 재출력
    */


#endif
