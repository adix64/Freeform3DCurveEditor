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

vec4 rotateY(vec3 punct, float u)
{
	float x = punct.x * cos(u) - punct.z *sin(u);
	float z = punct.x * sin(u) + punct.z *cos(u);
	return vec4(x, punct.y, z, 1);
}

vec4 translateZ(vec3 punct, float s)
{
	return vec4(punct.x, punct.y, punct.z + s, 1);
}

vec3 BSpline(float t, vec3 p0, vec3 p1, vec3 p2)
{
	float t2 = t * t, b, b1, b2;

	b = 0.5 * (1 - 2 * t + t2);
	b1 = 0.5 * (-2 * t2 + 2 * t + 1);
	b2 = 0.5 * t2;
	return (b * p0 + b1 * p1 + b2 * p2) / (b + b1 + b2);
}

void main()
{
	vec3 bspline1, bspline2;
#define PVM (projection_matrix * view_matrix * model_matrix)
	mat4 model_matrix = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	if(surface_type == TRANSLATION){
		if(num_control_points > 1)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[0], control_points[0], control_points[1]);
				bspline2 = BSpline(t + step, control_points[0], control_points[0], control_points[ 1]);
				
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}

			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[num_control_points-2], control_points[num_control_points-1], control_points[num_control_points-1]);
				bspline2 = BSpline(t + step, control_points[num_control_points-2], control_points[num_control_points-1], control_points[num_control_points-1]);
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}
		}

		for(int base = 1; base < num_control_points-1; base++)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[base-1], control_points[base], control_points[base + 1]);
				bspline2 = BSpline(t + step, control_points[base-1], control_points[base], control_points[base + 1]);
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}
		}
	}else
	{
		if(num_control_points > 1)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[0], control_points[0], control_points[1]);
				bspline2 = BSpline(t + step, control_points[0], control_points[0], control_points[ 1]);
				
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}

			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[num_control_points-2], control_points[num_control_points-1], control_points[num_control_points-1]);
				bspline2 = BSpline(t + step, control_points[num_control_points-2], control_points[num_control_points-1], control_points[num_control_points-1]);
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}
		}

		for(int base = 1; base < num_control_points-1; base++)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				bspline1 = BSpline(t, control_points[base-1], control_points[base], control_points[base + 1]);
				bspline2 = BSpline(t + step, control_points[base-1], control_points[base], control_points[base + 1]);
				gl_Position = PVM * vec4(bspline1,1); EmitVertex();
				gl_Position = PVM * vec4(bspline2,1); EmitVertex();
				EndPrimitive();
			}
		}
	}
}
