#version 460 core

layout (location = 0) out vec4 v_FragColor;

layout (location = 0) in vec2 v_TexUV;
layout (location = 1) in vec4 v_Color;
layout (location = 2) flat in uint v_TexArrayID;
layout (location = 3) flat in uint v_TexID;

layout (binding = 0) uniform sampler2DArray u_textures[9];

void main() {
    vec2 tiled_UV = fract(v_TexUV);
    vec4 texColor = texture(u_textures[v_TexArrayID], vec3(tiled_UV, float(v_TexID)));
    v_FragColor = texColor * v_Color;
}