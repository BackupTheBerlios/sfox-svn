uniform float4x4 mvp;
uniform float4 scaleTranslate;
uniform float4 scaleTranslateTex;
uniform sampler2D heightmap;
uniform sampler2D grass;
uniform sampler2D heightTex;
uniform sampler2D cliff;

sampler2D terrainTexSamp = sampler_state
{
  generateMipMap = true;
  minFilter = LinearMipMapLinear;
  magFilter = Linear;
  maxAnisotropy = 8;
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

float4 clipmapFrag(in float4 color : COLOR,
                   in float4 texCoord : TEXCOORD0) : COLOR
{
  float workAround = tex2D(heightmap, texCoord.xy).x;
  float z = tex2D(heightmap, texCoord.xy).x;
  float alpha = 1.-f1(z);
  return lerp(tex2D(grass, texCoord.xy*10), tex2D(cliff, texCoord.xy*5), alpha);}


struct vertout {
  float4 hpos : POSITION;
  float4 color : COLOR;
  float4 texCoord : TEXCOORD0;
};

vertout clipmapVert(float3 position : POSITION,
                    float4 color : COLOR)
{
  vertout OUT;

  float2 uv = position.xz*scaleTranslateTex.xy+scaleTranslateTex.zw;
  float z = tex2Dlod(heightmap, float4(uv, 0, 1)).x;
  float2 worldPos = position.xz*scaleTranslate.xy+scaleTranslate.zw;
  //z = 0;
  OUT.hpos = mul(mvp, float4(worldPos.x, z*30, worldPos.y, 1));
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
