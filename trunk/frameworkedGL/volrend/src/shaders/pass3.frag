uniform sampler3D volData;
uniform sampler2D raysDir;
uniform sampler2D resTex;
uniform float t;
uniform float dt;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 ray = texture2D(raysDir, tcoord);
  ray.xyz = gl_Color.xyz+dt*t*ray.xyz;
  vec4 res = vec4(0., 0., 0., 0.);

  for(float i = 0.; i < 1.; i++) {
    if(ray.w <= length(gl_Color.xyz-ray.xyz))
      res.w = 1.;
    float s = texture3D(volData, ray.xyz).x;
    if(s>.99||s<0.2)
      s=0.;
    res.xyz = res.xyz+(1.-res.w)*s*.6*vec3(s, s, s);
    res.w = res.w+(1.-res.w)*s*.6;
    ray.xyz += dt*i*ray.xyz;
  }
  vec4 dest = texture2D(resTex, tcoord);
  gl_FragColor.xyz = dest.xyz+(1.-dest.w)*res.w*.6*res.xyz;
  gl_FragColor.w = dest.w+(1.-dest.w)*res.w*.6;
}
