varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
  N = gl_NormalMatrix*gl_Normal;
  V = vec3(gl_ModelViewMatrix*gl_Vertex);
  L = normalize(gl_LightSource[0].position.xyz-V);
  //gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
