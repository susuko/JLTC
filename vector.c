#include <jltc.h>

int vec2Eq(t_vec2 a, t_vec2 b)
{
	return a.x == b.x && a.y == b.y;
}

int vec3Eq(t_vec3 a, t_vec3 b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}
