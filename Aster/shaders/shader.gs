#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 30.0;
    vec3 direction = normal * time * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {    
    vec3 normal = GetNormal();

	float randValue = rand(normal.xy);

	normal *= randValue;

    //gl_Position = gl_in[0].gl_Position;
    gl_Position = explode(gl_in[0].gl_Position, normal);
    EmitVertex();
    
    //gl_Position = gl_in[1].gl_Position;
    gl_Position = explode(gl_in[1].gl_Position, normal);
    EmitVertex();

    //gl_Position = gl_in[2].gl_Position;
    gl_Position = explode(gl_in[2].gl_Position, normal);
    EmitVertex();

    EndPrimitive();
}