uniform sampler3D volData;
uniform sampler2D raysDir;
uniform sampler2D resTex;
uniform float t;
uniform float dt;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 rayDir = texture2D(raysDir, tcoord);
  vec4 ray;
  vec4 res = vec4(0., 0., 0., 0.);

  for(float i = 0.; i < 10.; i++) {
    ray.xyz = gl_Color.xyz+dt*(t+i)*rayDir.xyz;
    if(rayDir.w <= dt*(t+i))
      res.w = 1.;
    float s = texture3D(volData, ray.xyz).x;
    res.xyz = res.xyz+(1.-res.w)*s*.6*vec3(s, s, s);
    res.w = res.w+(1.-res.w)*s*.6;
  }

  vec4 dest = texture2D(resTex, tcoord);
  gl_FragColor.xyz = dest.xyz+(1.-dest.w)*res.w*.6*res.xyz;
  gl_FragColor.w = dest.w+(1.-dest.w)*res.w*.6;
}
