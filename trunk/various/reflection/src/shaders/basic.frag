uniform sampler2D myTexture;
//varying vec3 N;
varying vec3 V;
varying vec3 L;

void main (void)
{
  //vec4 C = gl_FrontMaterial.diffuse;
  vec3 l = normalize(L);
  vec3 N = normalize(vec3(texture2D(myTexture, gl_TexCoord[0].xy))*2.0-1.0);
  vec3 H = normalize(l-V);
  float s = pow(max(dot(N, H), 0.0), 128.0);
  //float s=0.0;
  float d = max(dot(N, l), 0.0);
  //gl_FragColor = max(dot(N, L), 0.0)*gl_FrontMaterial.diffuse;
  //gl_FragColor = C;
  gl_FragColor = d*vec4(1,1,0,0)+s*vec4(0.5,0.5,0.5,0.5);
  //gl_FragColor = N;
}
