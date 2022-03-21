#ifndef _TEST
#define _TEST

// Vertex Shader

	/*쉐이더는 버퍼의 주소를 받고 정점에 대한 단위(전체크기)를 모르기 때문에 받아야됨
	여기서 정점의 세부정보를 모르기때문에 semantic을 사용
	내가 지정한 시멘틱 정보로 맵핑되고있음
	그렇기에 순서가 바뀌어도 상관이 없다.

	Vertex Shader에서 정점의 구조체를 맵핑하지 않는 이유는
	나중가면 정점이 크기가 커져서 특정 멤버만 받아오게 하기 위해 semantic을 사용함.*/

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


VTX_OUT VS_Test(VTX_IN _in)		//정점 하나당 호출될 함수 _in이 정점
{
	VTX_OUT output = (VTX_OUT)0.f;


	output.vPosition = float4(_in.vPos,1.f);
	output.vColor =_in.vColor;


	return output;
}


// Rasterizer
// 정점이 만도는 도형 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
// 해당 픽셀들 마다 픽셀 쉐이더 호출


float4 PS_Test(VTX_OUT _in) : SV_Target  // 픽셀마다 호출되는 함수
{
	float4 vOutColor = (float4) 0.f;

	vOutColor = _in.vColor;				 // 픽셀마다 모든 정점에 대한 거리값을 계산해서 가까운 정점일수록 그 정점에 대한 색의 비율을 높여준다. (선형보간)

	return vOutColor;
}


// Pixel Shader










#endif

//세이더 형식	효과f/x
//세이더 모델	5.0
//개체 파일 지우기
