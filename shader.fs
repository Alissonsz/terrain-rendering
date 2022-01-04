#version 330 core
out vec4 FragColor;
uniform sampler2D heightsTexture;
uniform sampler2D texture2;
uniform sampler2D normalTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int parFlag = 0;
uniform int binaryIter = 5;



/*in vec2 texCord;
in vec3 Normal;
in vec3 FragPos;*/

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

vec2 parallaxBinarySearch(vec2 texCoords, vec3 viewDir){
    vec2 P = viewDir.xy * 0.3;

    vec2 prevCoords = texCoords;
    texCoords = texCoords + P;
    P = prevCoords;

    vec2 uvin = texCoords;
    vec2 uvout = P;

    vec2 currentCoords;
    float Hmax = 1.0f;
    float Hmin = 0.0f;

    for(int i=0; i<binaryIter; i++){
        float H = (Hmin + Hmax)/2; // middle
        currentCoords = uvin * H + uvout * (1 - H);
        float h = texture(heightsTexture, currentCoords).r;
        
        if(H <= h) Hmin = H;
        else Hmax = H;
    }

    return currentCoords;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir){ 
    // number of depth layers
    const float numLayers = 60;
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * 0.3; 
      

    vec2 deltaTexCoords = P / numLayers;
    vec2 prevCoords = texCoords;
    texCoords = texCoords + P;

    P = prevCoords;

    // get initial values
    vec2  currentTexCoords     = texCoords ;
    float currentDepthMapValue = 1 - texture(heightsTexture, currentTexCoords).r;
    vec2 finalCoords;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        
        // get depthmap value at current texture coordinates
       
        currentDepthMapValue = 1 - texture(heightsTexture, currentTexCoords).r;  
        
        // get depth of next layer
        currentLayerDepth += layerDepth;
    
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (1 - texture(heightsTexture, prevTexCoords).r) - currentLayerDepth + layerDepth;
    
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;

    
 
} 


void main() {
    vec3 newPos;
    vec3 newNormal;
    
    vec2 texCoords = fs_in.TexCoords;
    
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    if(texCoords.x < 0.0 || texCoords.y < 0.0) discard;
    else if(parFlag == 1)
        texCoords = parallaxMapping(fs_in.TexCoords, viewDir);
    else if(parFlag == 2)
        texCoords = parallaxBinarySearch(fs_in.TexCoords, viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
       discard;

    
    newNormal = texture(normalTexture, vec2(texCoords.x, 0 + (1 - texCoords.y))).rgb;
    newNormal = normalize(newNormal * 2.0 - 1.0);
    
    vec3 norm = newNormal;
    vec3 lightColor = texture(texture2, vec2(texCoords)).rgb;
    
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
 

    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;

    vec4 result = vec4(ambient + diffuse + specular, 1.0);
    FragColor = result;
}
