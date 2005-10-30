varying vec3 V;
varying vec3 L;
varying vec3 N;

void main()
{
//Lighting in eyespace
/*   V = (gl_ModelViewMatrix*gl_Vertex).xyz; */
/*   L = normalize(gl_LightSource[0].position.xyz-V.xyz); */
/*   V = -normalize(V); */
/*   N = normalize(gl_NormalMatrix*gl_Normal); */

/*   gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; */

//Lighting in objectspace
  vec4 eye = vec4(-gl_ModelViewMatrix[0][3], -gl_ModelViewMatrix[1][3],
		  -gl_ModelViewMatrix[2][3], 1.);
  V = vec3(gl_ModelViewMatrixInverse*eye)-gl_Vertex;
  V = normalize(V);
  L = vec3(gl_ModelViewMatrixInverse*gl_LightSource[0].position);
  L = normalize(L-gl_Vertex.xyz);
  N = normalize(gl_Normal);

  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
