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
  // maxAnisotropy = 8;
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

float3 computeNormal(float4 texCoord)
{
  float2 dx = float2(scaleTranslateTex.x, 0);
  float2 dy = float2(0, scaleTranslateTex.y);
  float3 N;
  N.x = tex2D(heightTex, texCoord.xy+dx).x-tex2D(heightTex, texCoord.xy-dx).x;
  N.z = tex2D(heightTex, texCoord.xy+dy).x-tex2D(heightTex, texCoord.xy-dy).x;
  N.y = 0.1;
  float norm = dot(N,N);
  if(norm == 0.0)
    return float3(0,1,0);
  else
    return normalize(N);
}

float4 computeLighting(float3 N, float4 texCoord)
{
  float3 light = normalize(float3(-1.0, 1.5, 0));
  float diff = max(dot(N, light), 0);

  float z = tex2D(heightTex, texCoord.xy).x;
  float alpha = 1.-f1(z);
  //return diff.xxxx;
  return diff*lerp(tex2D(grass, texCoord.xy*10), tex2D(cliff, texCoord.xy*5), alpha);
}

float4 clipmapFrag(in float4 color : COLOR,
                   in float4 texCoord : TEXCOORD0) : COLOR
{
  float workAround = tex2D(heightmap, texCoord.xy).x;
  float3 N = computeNormal(texCoord);
  //return workAround.xxxx;
  return computeLighting(N, texCoord);
//  return float4((N.xzy+1)*0.5, 1);
}

struct vertout {
  float4 hpos : POSITION;
  float4 color : COLOR;
  float4 texCoord : TEXCOORD0;
};


float2 computeAlpha(float2 pos)
{
  float w = 256./10.;
  float2 alpha;
  alpha.x = clamp((abs(pos.x+scaleTranslate.z-128.0f)-(255./2.-w-1))/w, 0, 1);
  alpha.y = clamp((abs(pos.y+scaleTranslate.w-128.0f)-(255./2.-w-1))/w, 0, 1);
  return alpha;
}


vertout clipmapVert(float3 position : POSITION,
                    float4 color : COLOR)
{
  vertout OUT;

  float2 uv = position.xz*scaleTranslateTex.xy+scaleTranslateTex.zw;
  float z = tex2Dlod(heightmap, float4(uv, 0, 1)).x;
  float2 worldPos = position.xz*scaleTranslate.xy+scaleTranslate.zw;
  //z = 0;
  OUT.hpos = mul(mvp, float4(worldPos.x, z*50, worldPos.y, 1));
  OUT.color = color;
//  OUT.color = float4(uv, 0, 1);
  OUT.texCoord = float4(uv, 0, 1);
  OUT.color = (computeAlpha(position.xz).x+computeAlpha(position.xz).y)*color;

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
