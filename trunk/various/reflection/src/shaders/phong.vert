//attribute vec4 position;
attribute vec3 tangent;
varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
  vec3 t = tangent;
  N = gl_NormalMatrix*gl_Normal;
  V = vec3(gl_ModelViewMatrix*gl_Vertex);
  L = gl_LightSource[0].position.xyz-V;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
