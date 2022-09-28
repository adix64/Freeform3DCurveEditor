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


vec3 linearBezier(float t, vec3 control_p1, vec3 control_p2)
{
	return control_p1 + (control_p2 - control_p1) * t;
}

vec3 quadraticBezier(float t, vec3 control_p1, vec3 control_p2, vec3 control_p3)
{

	return (1 - t) * (1 - t) * control_p1 + 2 * (1 - t) * t * control_p2 + t * t * control_p3;
}

vec3 cubicBezier(float t, vec3 control_p1, vec3 control_p2, vec3 control_p3, vec3 control_p4)
{
	return control_p1 * (1 - t)*(1 - t)*(1 - t) + control_p2 * 3 * t*(1 - t)*(1 - t) + control_p3 * 3 * t*t*(1 - t) + control_p4 *t*t*t;
}

void main()
{
#define PVM (projection_matrix * view_matrix * model_matrix)
	mat4 model_matrix = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	
	int base = 0;
	for(int c = 0; c < num_curves; c++)
	{
		int order = curve_orders[c];

		switch(order)
		{
		case 3: // cubic Bezier
			for(float t = minT; t < maxT; t += step)
			{
				gl_Position = PVM * vec4(cubicBezier(t, control_points[base], control_points[base + 1], control_points[base + 2], control_points[base + 3]), 1); EmitVertex();
				gl_Position = PVM * vec4(cubicBezier(t + step, control_points[base], control_points[base + 1], control_points[base + 2], control_points[base + 3]), 1); EmitVertex();
				EndPrimitive();
			}
			base += 3;
			break;
		case 2: // quadratic Bezier
			for(float t = minT; t < maxT; t += step)
			{
				gl_Position = PVM * vec4(quadraticBezier(t, control_points[base], control_points[base + 1], control_points[base + 2]), 1); EmitVertex();
				gl_Position = PVM * vec4(quadraticBezier(t + step, control_points[base], control_points[base + 1], control_points[base + 2]), 1); EmitVertex();
				EndPrimitive();
			}
			base += 2;
			break;
		case 1: // linear Bezier
			for(float t = minT; t < maxT; t += step)
			{
				gl_Position = PVM * vec4(linearBezier(t, control_points[base], control_points[base + 1]), 1); EmitVertex();
				gl_Position = PVM * vec4(linearBezier(t + step, control_points[base], control_points[base + 1]), 1); EmitVertex();
				EndPrimitive();
			}
			base += 1;
			break;
		default:
			break;
		}
	}
}
