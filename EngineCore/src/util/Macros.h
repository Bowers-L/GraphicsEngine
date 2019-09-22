#pragma once
#include <iostream>

#define PRINTLN(x) std::cout << x << std::endl

#define ASSERT(x) if (!(x)) __debugbreak();

#define VEC2PRINTFORMAT(V) V.x << ", " << V.y
#define VEC3PRINTFORMAT(V) V.x << ", " << V.y << ", " << V.z