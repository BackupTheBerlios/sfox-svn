varying vec3 V;
varying vec3 L;
varying vec3 N;

void main (void)
{
  vec3 n = normalize(N);
  vec3 h = normalize(V+L);
  float s = pow(max(dot(n, h), 0.0), gl_FrontMaterial.shininess);
  float d = max(dot(n, L), 0.0);
  if(d <= 0.) s = 0.;
  gl_FragColor = d*gl_FrontMaterial.diffuse+s*gl_FrontMaterial.specular;
}
