#version 330
layout(lines) in;
layout(line_strip, max_vertices = 256) out;

uniform mat4 view_matrix, projection_matrix;
uniform int num_instances;
uniform int curve_type;
uniform int num_curves;
uniform int curve_orders[100];

uniform int num_control_points;
uniform vec3 control_points[256];

in int InstanceID[2];

#define TRANSLATION 1
#define ROTATION 2
uniform int surface_type;
uniform float offset, minT, maxT, step;

vec4 rotateY(vec4 punct, float u)
{
	float x = punct.x * cos(u) - punct.z *sin(u);
	float z = punct.x * sin(u) + punct.z *cos(u);
	return vec4(x, punct.y, z, 1);
}

vec4 translateZ(vec3 punct, float s)
{
	return vec4(punct.x, punct.y, punct.z + s, 1);
}

vec3 Hermite(float t, vec3 p0, vec3 m0, vec3 m1, vec3 p1)
{
	return (2.0*t*t*t - 3.0*t*t + 1.0) * p0  
         + (t*t*t - 2.0*t*t + t) * (m0 - p0) * 4
         + (-2.0*t*t*t + 3.0*t*t) * p1
         + (t*t*t - t*t) * ( p1 - m1) * 4;
}

void main()
{
#define PVM (projection_matrix * view_matrix * model_matrix)
	mat4 model_matrix = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	for(int base = 0; base < num_control_points-1; base+=3)
	{
		for(float t = minT; t < maxT; t+= step)
		{
			vec3 hermite1 = Hermite(t,        control_points[base], control_points[base + 1], control_points[base + 2], control_points[base + 3]);
			vec3 hermite2 = Hermite(t + step, control_points[base], control_points[base + 1], control_points[base + 2], control_points[base + 3]);
			gl_Position = PVM * vec4(hermite1,1); EmitVertex();
			gl_Position = PVM * vec4(hermite2,1); EmitVertex();
			EndPrimitive();
		}
	}
}
