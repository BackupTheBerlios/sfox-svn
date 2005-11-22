float4 main(in float4 color : COLOR,
	    in float4 texCoord : TEXCOORD0,
	    in float4 fragcoord : WPOS,
	    uniform sampler3D volData,
	    uniform sampler2D raysDir,
	    uniform float t,
	    uniform float dt,
	    uniform float2 winScale) : COLOR
{ 
  float2 tcoord = fragcoord.xy*winScale;
  float4 rayDir = tex2D(raysDir, tcoord);
  float4 ray;
  float4 res = float4(0, 0, 0, 0);

  ray.xyz = color.xyz+dt*t*rayDir.xyz;
  float rayLen = 0.;
  for(float i = 0.; i < 173.; i++) {
/*     if(res.w>=0.95) */
/*       break;     */
/*     if(rayDir.w <= rayLen) { */
/*       res.w = 1.; */
/*     } else { */
/*       float s = tex3D(volData, ray.xyz).x; */
    float s = 2.;
      //if(s<0.1||s>0.7)
      //  s=0.;
/*       res.xyz = res.xyz+(1.-res.w)*s*0.4*float3(s, s, s); */
      res.xyz = res.xyz+(1.-res.w)*s*0.4f;
/*       res.w = res.w+(1.-res.w)*s*0.4; */
/*     } */
    ray.xyz += dt*rayDir.xyz;
    rayLen += dt;
    res.x += i*t+rayLen;
  }
  return res;  
}
