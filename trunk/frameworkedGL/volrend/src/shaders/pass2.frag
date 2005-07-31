uniform sampler2D frontFace;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 ray = gl_Color-texture2D(frontFace, tcoord);
  vec4 norm = length(ray.xyz);  
  gl_FragColor.w = norm;
  gl_FragColor.xyz = ray.xyz*1.0/norm;  
}
