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
  vec4 res = 0.;

  for(float i = 0.; i < 4.; i++) {
    if(ray.w <= length(gl_Color.xyz-ray.xyz))
      res.w = 1.;
    float s = texture3D(volData, ray.xyz).w;
    res = res+(1.-res.w)*s*vec4(s, s, s, s);
    ray.xyz = dt*i*ray.xyz;
  }
  vec4 dest = texture2D(resTex, tcoord);
  gl_FragColor = dest+(1.-dest.w)*res.w*res;
  gl_FragColor = res;
}
