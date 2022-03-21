Semantic
===================
Semantic은 데이터의 출처와 역할에 대해 의미를 부여하기 위한 키워드이다.  
사용할 정점(구조체)의 멤버변수를 HLSL 파일에서 semantic을 이용하여 서로를 대응(mapping)시킬 수 있다.  
 
  
Code
==============

     #ifndef _TEST
     #define _TEST
     
     // HLSL(High-Level Shader Language) 코드
     // HLSL은 DirectX에서 프로그래밍 가능한 셰이더와 함께 사용하는 C와 비슷한 고급 셰이더 언어다.
     
     // 예를 들어 HLSL을 사용하여 꼭짓점 셰이더또는 픽셀 셰이더를 작성하고 
     // Direct3D 애플리케이션의 렌더러 구현에서 해당 셰이더를 사용할 수 있다.
     // 인텔리센스가 작동되지 않는다면 위에서 확장-> 확장 모드를 켜서 HLSL를 검색하고 다운로드 받으면 된다.
     
     struct VTX_IN
     {
     	float3 vPos : POSITION;	// semantic : 정점 안에서 어떤 구조로 정점이 세부적으로 분할되는지를 알려주는 추가정보
     	float4 vColor : COLOR;
     };
     
     
     struct VTX_OUT
     {
     	float4 vPosition : SV_Position;
     	float4 vColor : COLOR;
     };
     
     
     VTX_OUT VS_Test(VTX_IN _in)		//정점 하나당 호출될 함수
     {
     	VTX_OUT output = (VTX_OUT)0.f;
     
     
     	output.vPosition = float4(_in.vPos,1.f);
     	output.vColor =_in.vColor;
     
     
     	return output;
     }
     
     
     // Rasterizer
     // 정점이 만도는 도형 안에 들어오는 픽셀을 검출
     // 해당 픽셀들 마다 픽셀 쉐이더 호출
     
     
     float4 PS_Test(VTX_OUT _in) : SV_Target  // 픽셀마다 호출되는 함수
     {
     	float4 vOutColor = (float4) 0.f;
     
     	vOutColor = _in.vColor;				 // 픽셀마다 모든 정점에 대한 거리값을 계산해서 가까운 정점일수록 그 정점에 대한 색의 비율을 높여준다. (선형보간)
     
     	return vOutColor;
     }
     
     
     // Pixel Shader
     
     #endif
     
     //프로젝트 속성
     //세이더 형식	효과f/x
     //세이더 모델	5.0
     //개체 파일 지우기
     
