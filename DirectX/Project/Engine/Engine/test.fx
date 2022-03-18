#ifndef _TEST
#define _TEST

// Vertex Shader

struct VTX_IN
{
	float3 vPos : POSITION;	//semantic
	float4 vColor : COLOR;
};


struct VTX_OUT
{
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};


VTX_OUT VS_TEST(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT)0.f;


	output.vPosition = float4(_in.vPos,1.f);
	output.vColor =_in.vColor;

	return output;
}


// Rasterizer
// 정점이 만도는 도형 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
// 해당 픽셀들 마다 픽셀 쉐이더 호출

float4 PS_Test(VTX_OUT _in) : SV_Target
{
	float4 vOutColor = (float4) 0.f;

	vOutColor = _in.vColor;

	return vOutColor;
}


// Pixel Shader










#endif

//세이더 형식	효과f/x
//세이더 모델	5.0
//개체 파일 지우기
