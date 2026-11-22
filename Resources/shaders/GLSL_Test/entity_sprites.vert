#version 460 core

layout (location = 0) in vec2 a_Pos;

struct Data {
	mat4 model;
	vec4 UV;
	uint texArrayID;
	uint texID;
};

layout(std430, binding = 0) buffer EntitySSBO {
    Data objects[];
};

layout(std140, binding = 2) uniform UB {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	ivec2 u_WorldSize;
	float u_DayRatio;
};

layout (location = 0) out vec2 v_GlobalPos;
layout (location = 1) out vec2 v_TexUV;
layout (location = 2) flat out uint v_TexArrayID;
layout (location = 3) flat out uint v_TexID;

vec2 get_quad_UV(int vertex_id, vec4 uv){
	float u = uv.x + (vertex_id == 2 || vertex_id == 3 ? uv.z : 0.0);
    float v = uv.y + (vertex_id == 1 || vertex_id == 2 ? uv.w : 0.0);
    return vec2(u, v);
}

void main() {
	Data obj = objects[gl_InstanceID];
	vec4 global_pos = obj.model * vec4(a_Pos, 0.0, 1.0);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * global_pos;

	v_GlobalPos = global_pos.xy;
	v_TexUV = get_quad_UV(int(gl_VertexID % 4), obj.UV);
	v_TexArrayID = obj.texArrayID;
	v_TexID = obj.texID;
}