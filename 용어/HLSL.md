Semantic
===================
Semantic은 데이터의 출처와 역할에 대해 의미를 부여하기 위한 키워드이다.  
사용할 정점(구조체)의 멤버변수를 HLSL 파일에서 semantic을 이용하여 서로를 대응(mapping)시킬 수 있다.  
 
  
Code
==============
   #ifndef _TEST
   #define _TEST
   
   cbuffer TRANSFORM : register(b0)
   {
       // 행 우선으로 읽기
       row_major matrix g_matWorld;     //matrix or float4x4 (64바이트) 4x4형태 행렬
   }
   
   // gpu는 기본적으로 행렬을 가져올때 열 우선으로 메모리를 열거하기 때문에
   // row_major로 행 우선으로 바꿔준다.
   
   
   // Texture2D g_tex_0 : register(t0);
   // StructuredBuffer<float4> g_buffer : register(t1);
   // sampler g_sam : register(s0);
   // RWStructuredBuffer<float4> g_rwbuffer : register(u0);
   
   
   
   
   // Vertex Shader
   struct VTX_IN
   {
       float3 vPos : POSITION; // semantic    
       float4 vColor : COLOR;
   };
   
   struct VTX_OUT
   {
       float4 vPosition : SV_Position;
       float4 vColor : COLOR;
   };
   
   VTX_OUT VS_Test(VTX_IN _in)
   {
       VTX_OUT output = (VTX_OUT) 0.f;
       
       float4 vFinalPos = mul(float4(_in.vPos, 1.f), g_matWorld);   // 4*4 행렬과 위치를 곱해서 최종위치
       
       // 정점(float3)를 4*4행렬 로 맞춰주기위해 float4로 바꿔주면서 마지막 값은 행렬곱해서 위치값을 저장해주기 위해 1.f로 세팅
      
       
       output.vPosition = vFinalPos;
       output.vColor = _in.vColor;
       
       return output;
   }
   
   
   // Rasterizer
   // 정점이 만드는 도형(Topology) 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
   // 해당 픽셀들 마다 픽셀 쉐이더 호출
   
   float4 PS_Test(VTX_OUT _in) : SV_Target
   {
       float4 vOutColor = (float4) 0.f;
       
       vOutColor = _in.vColor;
       
       return vOutColor;
   }
   
   
   
   
   #endif
     
     //프로젝트 속성
     //세이더 형식	효과f/x
     //세이더 모델	5.0
     //개체 파일 지우기
     
