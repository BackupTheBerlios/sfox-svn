uniform sampler2D heightfield;

void main()
{
  vec3 N;
  float c = texture2D(heightfield, gl_TexCoord[0].xy).w;
  N.x = c-texture2D(heightfield, gl_TexCoord[0].xy+vec2(1./10., 0.)).w;
  N.y = c-texture2D(heightfield, gl_TexCoord[0].xy+vec2(0., 1./10.)).w;
  N.z = 1.0;
  N = normalize(N);
  
  gl_FragColor = vec4((N.xyz), 1.);
  //gl_FragColor = vec4(c , c, c, 1.0);
}
