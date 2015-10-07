#version 120



const float Eta = 0.67;          // Ratio of indices of refraction (air -> glass)
const float FresnelPower = 10.0; // Controls degree of reflectivity at grazing angles
 
const float F  = ((1.0 - Eta) * (1.0 - Eta)) / ((1.0 + Eta) * (1.0 + Eta));

varying vec3  Reflect;
varying vec3  Refract;
varying float Ratio;

attribute vec3 normal;

uniform vec3 viewer;     

void main( void )
{

    vec4 worldPosition = calcWorldPos( gl_Vertex );
    vec3 ecPosition3  = worldPosition.xyz - viewer;
 
    vec3 i = normalize(ecPosition3);
    vec3 n = normalize( calcWorldVec(normal) );
 
    Ratio = F + (1.0 - F) * pow((1.0 - dot(-i, n)), FresnelPower);
    //Ratio = 1.0 - Ratio; // Add this line if your cube map is not based on the right side

    Refract = refract(i, n, Eta);
    Refract = vec3(gl_TextureMatrix[0] * vec4(Refract, 1.0));


    Reflect = reflect(i, n);
    Reflect = vec3(gl_TextureMatrix[0] * vec4(Reflect, 1.0));
 
    gl_Position   = gl_ModelViewProjectionMatrix * worldPosition;

}

