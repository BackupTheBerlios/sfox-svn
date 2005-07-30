void main()
{
  vec4 pos = gl_ModelViewMatrix*gl_Vertex;
  gl_FrontColor = gl_MultiTexCoord0;
  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
