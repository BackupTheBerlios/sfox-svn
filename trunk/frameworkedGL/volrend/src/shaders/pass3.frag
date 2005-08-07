uniform sampler3D volData;
uniform sampler2D raysDir;
uniform float t;
uniform float dt;
uniform vec2 winScale;

void main() {
  vec2 tcoord = gl_FragCoord.xy*winScale;
  vec4 rayDir = texture2D(raysDir, tcoord);
  vec4 ray;
  vec4 res = vec4(0., 0., 0., 0.);

  ray.xyz = gl_Color.xyz+dt*t*rayDir.xyz;
  float rayLen = 0.;
  for(float i = 0.; i < 173.; i++) {
    if(res.w>=0.95)
      break;    
    if(rayDir.w <= rayLen) {
      res.w = 1.;           
    } else {
      float s = texture3D(volData, ray.xyz).x;
      //if(s<0.1||s>0.7)
      //  s=0.;
      res.xyz = res.xyz+(1.-res.w)*s*0.4*vec3(s, s, s);
      res.w = res.w+(1.-res.w)*s*0.4;
    }
    ray.xyz += dt*rayDir.xyz;
    rayLen += dt;
  }  
  gl_FragColor = res;  
}
