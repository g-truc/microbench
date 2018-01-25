#version 430 core

layout(binding = 0) uniform bufferFetch
{
  vec4 Color;
} Buffer;

layout(location = 0, index = 0) out vec4 Color;

void main()
{
    Color = Buffer.Color;
}
