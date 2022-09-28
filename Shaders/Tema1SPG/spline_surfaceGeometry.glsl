#version 330
layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

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
vec3 Spline(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
    float t2 = t * t;
    float t3 = t2 * t;
    vec3 ret;
    ret = 0.5 * ((2.0 * p1) +
    (-p0 + p2) * t +
    (2.0f * p0 - 5.0f * p1 + 4 * p2 - p3) * t2 +
    (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

    return ret;
}

void main()
{
	vec3 spline1, spline2;
#define PVM (projection_matrix * view_matrix * model_matrix)
	mat4 model_matrix = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	float step = 0.2;
	float extstep = 5;

#define instanceID (InstanceID[0] - 1)
	
	if(surface_type == TRANSLATION){
		for(int base = 1; base < num_control_points - 2; base++)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				for(int i = 0; i < 1; i++)
				{
					spline1 = Spline(t       , control_points[base-1], control_points[base],
															  control_points[base + 1], control_points[base + 2]);
					spline2 = Spline(t  + step, control_points[base-1], control_points[base],
															  control_points[base + 1], control_points[base + 2]);
					
					
					for(int i = 0; i < 1; i++){
						gl_Position = PVM * translateZ(spline1, (offset + i) * 4); EmitVertex();
						gl_Position = PVM * translateZ(spline2, (offset + i) * 4); EmitVertex();
						gl_Position = PVM * translateZ(spline2, (offset + i + 1) * 4); EmitVertex();
						EndPrimitive();

						gl_Position = PVM * translateZ(spline1, (offset + i) * 4); EmitVertex();
						gl_Position = PVM * translateZ(spline1, (offset + i + 1) * 4); EmitVertex();
						gl_Position = PVM * translateZ(spline2, (offset + i + 1) * 4); EmitVertex();
						EndPrimitive();
					}
					
				}
			}
		}
	}else{
		for(int base = 1; base < num_control_points - 2; base++)
		{
			for(float t = minT; t < maxT; t+= step)
			{
				for(int i = 0; i < 1; i++)
				{
					spline1 = Spline(t       , control_points[base-1], control_points[base],
															  control_points[base + 1], control_points[base + 2]);
					spline2 = Spline(t  + step, control_points[base-1], control_points[base],
															  control_points[base + 1], control_points[base + 2]);
					
					
					for(int i = 0; i < 1; i++){
						gl_Position = PVM * rotateY(spline1, (offset + i) * 0.251325); EmitVertex();
						gl_Position = PVM * rotateY(spline2, (offset + i) * 0.251325); EmitVertex();
						gl_Position = PVM * rotateY(spline2, (offset + i + 1) * 0.251325); EmitVertex();
						EndPrimitive();

						gl_Position = PVM * rotateY(spline1, (offset + i) * 0.251325); EmitVertex();
						gl_Position = PVM * rotateY(spline1, (offset + i + 1) * 0.251325); EmitVertex();
						gl_Position = PVM * rotateY(spline2, (offset + i + 1) * 0.251325); EmitVertex();
						EndPrimitive();
					}
					
				}
			}
		}
	}

}
