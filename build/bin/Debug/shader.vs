#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

//out vec3 ourColor;
uniform sampler2D heightsTexture;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

/*out vec3 Normal;
out vec3 FragPos;
out vec2 texCord;*/


void main(){ 

    /*vec4 curV;
    curV = model * vec4(aPos, 1.0);
    float texX, texY;
    texX = (curV.x + 15) / 30;
    texY = (curV.z + 15) / 30;

  
    gl_Position = projection * view * curV;
    
    Normal = aNormal;
    FragPos = vec3(curV);
    

    texCord = vec2(texX, texY);*/

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;   
    
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 
