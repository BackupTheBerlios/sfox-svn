uniform sampler2D resTex;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 dest = texture2D(resTex, tcoord);
  if(dest.w >= 1.)
    gl_FragDepth = 0.;
  else
    gl_FragDepth = 10000.;  
}
