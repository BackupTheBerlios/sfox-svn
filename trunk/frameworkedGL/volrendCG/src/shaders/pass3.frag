float4 main(in float4 color : COLOR,
	    in float4 texCoord : TEXCOORD0,
	    in float4 fragcoord : WPOS,
	    uniform sampler3D volData,
	    uniform sampler2D raysDir,
	    uniform sampler2D colorMap,
	    uniform float t,
	    uniform float dt,
	    uniform float2 winScale) : COLOR
{ 
  float2 tcoord = fragcoord.xy*winScale;
  float4 rayDir = tex2D(raysDir, tcoord);
  float4 ray;
  float prevS = 0;
  float4 res = float4(0, 0, 0, 0);

  ray.xyz = color.xyz+dt*t*rayDir.xyz;
  float rayLen = 0.;
  for(float i = 0.; i < 173.; i++) {
    if(res.w>=0.95) 
       break;     
     if(rayDir.w <= rayLen) { 
       res.w = 1.; 
     } else { 
       float s = tex3D(volData, ray.xyz).x;       
       //float4 col = tex2D(colorMap, float2(s, 0));
       float4 col = tex2D(colorMap, float2(s, prevS));
       res.xyz = res.xyz+(1.-res.w)*col.w*0.4*col.xyz;
       res.w = res.w+(1.-res.w)*col.w*0.4;
       prevS = s;
    } 
    ray.xyz += dt*rayDir.xyz;
    rayLen += dt;    
  }
  return res;  
}
