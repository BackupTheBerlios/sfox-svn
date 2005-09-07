void main()
{
  gl_FrontColor = gl_MultiTexCoord0;
  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
