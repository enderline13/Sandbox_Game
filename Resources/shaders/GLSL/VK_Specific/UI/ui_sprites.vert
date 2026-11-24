#version 460 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TexUV;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in uint a_TexArrayID;
layout (location = 4) in uint a_TexID;

layout(std140, binding = 1) uniform UI_UB {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

layout(push_constant) uniform UIPushConstants {
    layout (offset = 0) vec2 u_ContentOffset;
} u_PushConstant;

layout (location = 0) out vec2 v_TexUV;
layout (location = 1) out vec4 v_Color;
layout (location = 2) flat out uint v_TexArrayID;
layout (location = 3) flat out uint v_TexID;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Pos + u_PushConstant.u_ContentOffset, 0.0, 1.0);
	v_TexUV = a_TexUV;
	v_Color = a_Color;
	v_TexArrayID = a_TexArrayID;
	v_TexID = a_TexID;
}