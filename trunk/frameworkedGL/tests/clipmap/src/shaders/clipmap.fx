uniform float4x4 mvp;
uniform float4 scaleTranslate;
uniform float4 scaleTranslateTex;
uniform sampler2D heightmap;
uniform sampler2D grass;
uniform sampler2D cliff;
uniform sampler2D heightTex;

sampler2D terrainTexSamp = sampler_state
{
  minFilter = Linear;
  magFilter = Linear;
};

sampler2D heightSamp = sampler_state
{
  minFilter = Linear;
  magFilter = Linear;
};

float f1(float z)
{
  if(z<=0.75) {
    return 1-4./3.*z;
  } else
    return 0;
}

float4 dumbFrag(in float4 color : COLOR) : COLOR
{
  return float4(scaleTranslate.x, 0, 0, 1);
}

float4 clipmapFrag(in float4 color : COLOR,
                   in float4 texCoord : TEXCOORD0,
                   in float z2 : TEXCOORD1) : COLOR
{
  float z = tex2D(heightmap, texCoord.xy).x;
  float alpha = 1.-f1(z2);
  return lerp(tex2D(grass, texCoord.xy*10), tex2D(cliff, texCoord.xy*5), alpha);
  //return z2.xxxx;
//   return tex2D(heightTex, texCoord.xy).xxxx;
  // return tex2D(heightmap, texCoord.xy).zzzz;
  //return tex2D(heightmap, texCoord.xy).wwww;
//  return color;
}


struct vertout {
  float4 hpos : POSITION;
  float4 color : COLOR;
  float4 texCoord : TEXCOORD0;
  float z: TEXCOORD1;
};

vertout clipmapVert(float3 position : POSITION,
                    float4 color : COLOR)
{
  vertout OUT;

  float2 uv = position.xz*scaleTranslateTex.xy+scaleTranslateTex.zw;
  OUT.z.x = tex2Dlod(heightmap, float4(uv, 0, 1)).x;
  float2 worldPos = position.xz*scaleTranslate.xy+scaleTranslate.zw;
  //OUT.z = 0;
  OUT.hpos = mul(mvp, float4(worldPos.x, OUT.z.x*0, worldPos.y, 1));
  OUT.color = color;
  OUT.color = float4(uv, 0, 1);
  OUT.texCoord = float4(uv, 0, 1);

  return OUT;
}

technique clipmap {
  pass {
    VertexProgram = compile vp40 clipmapVert();
    FragmentProgram = compile fp40 clipmapFrag();
  }
}

technique clipmapNoFrag {
  pass {
    VertexProgram = compile vp40 clipmapVert();
    FragmentProgram = NULL;
  }
}
