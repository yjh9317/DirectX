#include "pch.h"
#include "extern.h"


tTransform g_transform = {};			// extern 전역변수 선언

// 기저벡터
const Vec3 Vec3::Right = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::Up = Vec3(0.f, 1.f, 0.f);
const Vec3 Vec3::Front = Vec3(0.f, 0.f, 1.f);