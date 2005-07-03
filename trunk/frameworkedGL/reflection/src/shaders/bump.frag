uniform sampler2D texNormal;
varying vec3 V;
varying vec3 L;
varying vec2 tcoord;

void main (void)
{
  vec3 v = normalize(V);
  vec3 l = normalize(L);
  vec3 n = normalize(texture2D(texNormal, tcoord).xyz*2.-1.);
  vec3 h = normalize(l+v);
  float s = pow(max(dot(n, h), 0.0), gl_FrontMaterial.shininess);
  float d = max(dot(n, l), 0.0);
  if(d <= 0.) s = 0.;
  gl_FragColor = d*gl_FrontMaterial.diffuse+s*gl_FrontMaterial.specular;
  //gl_FragColor = texture2D(texNormal, tcoord);
}
