uniform sampler3D volData;
uniform sampler2D resTex;
uniform sampler2D raysDir;
uniform float t;
uniform float dt;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 ray = texture2D(raysDir, tcoord);
  ray.xyz = gl_Color.xyz+t*ray.xyz;
  vec4 res = 0.;

  for(float i = 0.; i < 4.; i++) {
    float s = texture3d(volData, ray.xyz);
    res = res+(1.-res.w)*s*vec4(s, s, s, s);
    ray.xyz = dt*i*ray.xyz;
  }
  vec4 dest = texture2D(resTex, tcoord);
  gl_FragColor = dest+(1.-dest.w)*res.w*res;
}
