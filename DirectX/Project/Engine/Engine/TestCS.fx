#ifndef _TESTCS
#define _TESTCS

#include "value.fx"

// cpu는 메인쓰레드가 순차적으로 실행하지만 gpu는 병렬처리가 기본
// Compute Shader에서는 그룹당 쓰레드를 여러개로 사용 할 수 있다.
// 사용자는 그 쓰레드를 몇개를 쓸 것인지를 지정해서 그룹 개수를 잡아 줄 수 있다.

// 그룹 쓰레드 개수
// 최대 쓰레드 제한 1024

// 시멘틱 종류
// Dispatch : Compute Shader를 실행해주는 함수

// SV_GroupThreadID     : 그룹 내에서의 스레드 인덱스 
// SV_GroupIndex        : 그룹 내에서의 스레드 인덱스를 1차원 값으로 변환

// SV_GroupID           : 스레드가 속한 그룹의 인덱스 
// SV_DispatchThreadID  : 모든 스레드를 기준으로 했을 때의 인덱스

// 예시 : 학교에서 몇반을 의미하는게 SV_GroupID, 그 반안에서 자기 번호를 의미하는게 SV_GroupThreadID
// GroupIndex는 GroupThreadID를 1차원 배열로 나눠 인덱스로 받은 값

// SV_DispatchThreadID는 (GroupID)*(그룹 쓰레드 x,y,z) + GroupThreadID, 그룹 쓰레드(10,8,3)이면 총 10*8*3 = 240개의 쓰레드를 보유
// ex) [(2,1,0) * (10,8,3)] + (7,5,0) = (27,13,0) [] 안은 각 축끼리 곱한다.
// 그룹 쓰레드 중에서 (2,1,0)번이고 이 그룹쓰레드의 개수는 (10*8*3)이고 그 안에서 쓰레드의 번호가 (7,5,0)


// SV_DispatchThreadID는 스레드마다 값이 다르므로 많이 활용된다.

RWTexture2D<float4> g_RWTex_0 : register(u0);

[numthreads(32, 32, 1)] // 3차원 도식화 , x :32,y :32 ,z :1 
void CS_Test(int3 _id : SV_DispatchThreadID) //스레드당 하나의 픽셀로 맞추면 스레드의 Dispatch값이 곧 픽셀 인덱스
{
    g_RWTex_0[_id.xy] = float4(1.f, 0.f, 0.f, 1.f);
}

#endif


