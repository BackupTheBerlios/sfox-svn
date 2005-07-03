varying vec3 N;
varying vec3 V;
varying vec3 L;

void main (void)
{
  vec3 n = normalize(N);
  vec3 l = normalize(L);
  float s = pow(max(dot(normalize(V), reflect(l, n)), 0.0), gl_FrontMaterial.shininess);
  gl_FragColor = max(dot(n, l), 0.0)*gl_FrontMaterial.diffuse+s*gl_FrontMaterial.specular;
}
