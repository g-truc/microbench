#version 430 core

layout(binding = 0) uniform bufferFetch
{
  uint Color;
} Buffer;

layout(location = 0, index = 0) out vec4 Color;

void main()
{
    Color = unpackUnorm4x8(Buffer.Color);
}
