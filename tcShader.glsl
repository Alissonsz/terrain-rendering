#version 430 core

layout(vertices = 3) out;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} tc_in[];

in vec3 vPosition[];
in vec3 vTang[];
in vec3 vBitang[];
in vec3 vNormal[];

out vec3 tcNormal[];
out vec3 tcBitang[];
out vec3 tcTang[];
out vec3 tcPosition[];

out TC_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} tc_out[];

uniform vec3 viewPos;

uniform int tess;
//
//uniform float dx;
//uniform float dy;
//uniform mat4 V;
//uniform float dz;
//uniform float radius;
//uniform float mesh;

#define ID gl_InvocationID
//bool newMet = true;

//float LOD(vec3 posV, vec3 cam){
//  float dist = distance(posV, cam);
//  if(newMet){
//    float rsc  = mesh/0.5;
//    float a = dist/rsc;
//    a = floor(4/a);
//    return 1 + a*a;
//  }
//  else{
//    if(dist<=50) return 32.0;
//    else if(dist>50 && dist<=100) return 16.0;
//    else if(dist>100 && dist<=200) return 8.0;
//    else if(dist>200 && dist<=300) return 4.0;
//    else if(dist>300 && dist<=400) return 2.0;
//    else if(dist>400) return 1.0;
//  }
//}

void main(){
//  float TessLevelInner = 1;
//  float e0, e1, e2;
//  vec3 d1, d2, d3;
//
//  e0 = e1 = e2 = 1;
  tc_out[gl_InvocationID].FragPos = tc_in[gl_InvocationID].FragPos;
	tc_out[gl_InvocationID].TexCoords = tc_in[gl_InvocationID].TexCoords;
	tc_out[gl_InvocationID].TangentLightPos = tc_in[gl_InvocationID].TangentLightPos;
	tc_out[gl_InvocationID].TangentViewPos = tc_in[gl_InvocationID].TangentViewPos;
  tc_out[gl_InvocationID].TangentFragPos = tc_in[gl_InvocationID].TangentFragPos;

  tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
  tcTang[gl_InvocationID] = vTang[gl_InvocationID];
  tcBitang[gl_InvocationID] = vBitang[gl_InvocationID];
  tcNormal[gl_InvocationID] = vNormal[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  if (ID == 0) {
//    vec3 v0 = gl_in[0].gl_Position.xyz;
//    vec3 v1 = gl_in[1].gl_Position.xyz;
//    vec3 v2 = gl_in[2].gl_Position.xyz;
//
//    vec3 bTriangulo = (v0 + v1 + v2)/3;
//
//    if(tess==1){
//      TessLevelInner = LOD(bTriangulo, viewPos);
//
//      d1=v1+(v2-v1)/2;
//      d2=v0+(v2-v0)/2;
//      d3=v0+(v1-v0)/2;
//
//      e0=LOD(d1,viewPos);
//      e1=LOD(d2,viewPos);
//      e2=LOD(d3,viewPos);
//    }
//    else if(tess == 0){
//      TessLevelInner = 1;
//    }
//    else if(tess == 2){
//      TessLevelInner = 2;
//    }

    gl_TessLevelInner[0] = 4;
    gl_TessLevelOuter[0] = 2;
    gl_TessLevelOuter[1] = 2;
    gl_TessLevelOuter[2] = 2;
  }
  //if(TessLevelInner == 8.0 /*|| TessLevelOuter == 8.0*/){
    //tcColor[ID] == vec4(1.0, 1.0, 1.0, 1.0);
  //}

}


