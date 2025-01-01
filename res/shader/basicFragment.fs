#version 410 core

in vec2 texCoord0;

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D sampler;

void main()
{
  fragColor = texture(sampler, texCoord0.xy) * vec4(color, 1);
}
