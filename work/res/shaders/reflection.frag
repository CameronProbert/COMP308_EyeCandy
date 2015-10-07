varying vec3 vNormal;
varying vec3 vPosition;


uniform samplerCube env;


void main(){

	vec3 I = normalize(vPosition); // Camera position
    vec3 R = reflect(I, vNormal);
    gl_FragColor = textureCube(env, R);

	//gl_FragColor = mix(matColor, textureCube(env, R), 0.5);
}