float4 main(in float4 color : COLOR,
	    in float4 texCoord : TEXCOORD0,
	    in float4 hpos : WPOS,
	    uniform sampler2D frontFace,
            uniform float2 winScale) : COLOR
{ 
  float4 OUT;
  float2 tcoord = hpos.xy*winScale;
  float4 ray = color-tex2D(frontFace, tcoord);
  float norm = length(ray.xyz);
  OUT.w = norm;
  OUT.xyz = ray.xyz*1.0/norm;

  return OUT;
}
