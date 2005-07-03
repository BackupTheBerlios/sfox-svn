attribute vec3 tangent;
varying vec3 V;
varying vec3 L;
varying vec2 tcoord;

void main()
{
  vec3 N = normalize(gl_Normal);
  vec3 T = normalize(tangent);
  vec3 B = normalize(cross(N, T));

  mat3 TBN = mat3(T, B, N);

  V.x = -gl_ModelViewMatrix[0][3];
  V.y = -gl_ModelViewMatrix[1][3];
  V.z = -gl_ModelViewMatrix[2][3];
  V = V-gl_Vertex.xyz;
  L = TBN*(gl_LightSource[0].position.xyz-gl_Vertex.xyz);
  V = TBN*V;
  tcoord = vec2(gl_MultiTexCoord0);
  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
