#version 120

varying vec3  Reflect;
varying vec3  Refract;
varying float Ratio;

uniform samplerCube tex0;

void main(void)
{

    vec3 refractColor = vec3 (textureCube(tex0, Refract));
    vec3 reflectColor = vec3 (textureCube(tex0, Reflect));

    vec3 color   = mix(refractColor, reflectColor, Ratio);

    gl_FragColor = vec4(color, 1.0);

}

