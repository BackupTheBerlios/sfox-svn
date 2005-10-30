varying vec3 R;

void main()
{
  vec3 N = gl_NormalMatrix*gl_Normal;
  vec3 V = vec3(gl_ModelViewMatrix*gl_Vertex);
  R = reflect(V, N);

  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
