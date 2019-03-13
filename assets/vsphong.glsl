#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;
layout(location=3) in vec3 VertexTangent;	// Über Vertexbuffer?
layout(location=4) in vec3 VertexBitangent;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

void main()
{
    Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
	Tangent = (ModelMat * vec4(VertexTangent.xyz,0)).xyz;
	Bitangent = (ModelMat * vec4(VertexBitangent.xyz,0)).xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * VertexPos;
}

