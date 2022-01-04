#version 430 core
//#extension GL_EXT_gpu_shader4 : enable
#define F3 0.333333333
#define G3 0.166666667

layout(triangles, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

in TC_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} te_in[];

out vec3 tcNormal[];
out vec3 tcBitang[];
out vec3 tcTang[];
out vec3 tcPosition[];

out vec3 tePosition;
out vec3 teNormal;
out vec3 teBitang;
out vec3 teTang;

out TE_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} te_out;

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec3 x) {
	const vec3 step = vec3(110, 241, 171);

	vec3 i = floor(x);
	vec3 f = fract(x);

	// For performance, compute the base input to a 1D hash from the integer part of the argument and the
	// incremental change to the 1D based on the 3D -> 1D wrapping
    float n = dot(i, step);

	vec3 u = f * f * (3.0 - 2.0 * f);
	return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x),
                   mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y),
               mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x),
                   mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z);
}

const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );

float fbm( vec3 p ){
    float f = 0.0;
    f += 0.5000*noise( p ); p = m3*p*2.02;
    f += 0.2500*noise( p ); p = m3*p*2.03;
    f += 0.1250*noise( p ); p = m3*p*2.01;
    f += 0.0625*noise( p );

    return f/0.9375;
}

void main(){
    vec3 tcPos0 = (tcPosition[0]);
    vec3 tcPos1 = (tcPosition[1]);
    vec3 tcPos2 = (tcPosition[2]);

    vec3 p0 = gl_TessCoord.x * tcPos0;
    vec3 p1 = gl_TessCoord.y * tcPos1;
    vec3 p2 = gl_TessCoord.z * tcPos2;
    tePosition = (p0 + p1 + p2);

    vec3 t0 = gl_TessCoord.x * tcTang[0];
    vec3 t1 = gl_TessCoord.y * tcTang[1];
    vec3 t2 = gl_TessCoord.z * tcTang[2];
    teTang = (t0 + t1 + t2);

    vec3 b0 = gl_TessCoord.x * tcBitang[0];
    vec3 b1 = gl_TessCoord.y * tcBitang[1];
    vec3 b2 = gl_TessCoord.z * tcBitang[2];
    teBitang = (b0 + b1 + b2);

    vec3 n0 = gl_TessCoord.x * tcNormal[0];
    vec3 n1 = gl_TessCoord.y * tcNormal[1];
    vec3 n2 = gl_TessCoord.z * tcNormal[2];
    teNormal = (n0 + n1 + n2);

    te_out.FragPos =  gl_TessCoord[0] * te_in[0].FragPos
                    + gl_TessCoord[1] * te_in[1].FragPos
	           	      + gl_TessCoord[2] * te_in[2].FragPos;

	te_out.TexCoords = gl_TessCoord[0] * te_in[0].TexCoords
	           	    + gl_TessCoord[1] * te_in[1].TexCoords
	                + gl_TessCoord[2] * te_in[2].TexCoords;

    mat3 TBN = transpose(mat3(teTang, teBitang, teNormal));

    te_out.TangentLightPos = TBN * lightPos;
    te_out.TangentViewPos  = TBN * viewPos;
    te_out.TangentFragPos  = TBN * te_out.FragPos;

    gl_Position = projection * view * model * vec4(tePosition, 1.0);
}
