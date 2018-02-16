#version 430 core

#define FETCH_COUNT 16

#define FRAG_COLOR	0

#define NORMALIZE_COORD float(1.0 / 2048.0)

layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];

in vec4 gl_FragCoord;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

vec2 alus(vec2 Coord)
{
	vec4 Tmp = vec4(Coord, Coord);
	for(int i = 0; i < 100; ++i)
	{
		Tmp *= vec4(1.000001, 1.000002, 1.000003, 1.000004);
		Tmp -= vec4(0.000001, 0.000002, 0.000003, 0.000004);
		Tmp /= vec4(1.000001, 1.000002, 1.000003, 1.000004);
		Tmp += vec4(0.000001, 0.000002, 0.000003, 0.000004);
	}
	return (Tmp.xy + Tmp.zw) * 0.5;
}

void main()
{
	vec2 Coord = gl_FragCoord.xy * NORMALIZE_COORD;

	vec2 Temp = vec2(0);
	for(int i = 0; i < FETCH_COUNT; ++i)
	{
		Temp = texture(Texture[i], Coord).xy + Temp;
		Temp = alus(Temp);
	}
	Color = vec4(Temp * (1.0 / float(FETCH_COUNT)), 0.0, 1.0);
}
