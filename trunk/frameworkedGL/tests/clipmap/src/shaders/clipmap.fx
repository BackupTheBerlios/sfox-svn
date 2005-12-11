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
  float2 workAround = tex2D(heightmap, texCoord.xy).xy;
  float3 N = computeNormal(texCoord);
//  return workAround.xxxx;
//  return workAround.yyyy;

  return computeLighting(N, texCoord);
}

struct vertout {
  float4 hpos : POSITION;
  float4 color : COLOR;
  float4 texCoord : TEXCOORD0;
};


float computeAlpha(float2 pos)
{
  float w = scaleTranslate.x*256./10.;
  float offset = 0.5*(scaleTranslate.x*256.-1)-w-1;
  float2 alpha;
  alpha.x = clamp((abs(pos.x-128.0f)-offset)/w, 0, 1);
  alpha.y = clamp((abs(pos.y-128.0f)-offset)/w, 0, 1);

  return max(alpha.x, alpha.y);
}


vertout clipmapVert(float3 position : POSITION,
                    float4 color : COLOR)
{
  vertout OUT;

  float2 worldPos = position.xz*scaleTranslate.xy+scaleTranslate.zw;
  float2 uv = position.xz*scaleTranslateTex.xy+scaleTranslateTex.zw;
  float alpha = computeAlpha(worldPos);

//   float z0 = tex2Dlod(heightmapCoarser, float4(uvCoarser.xy, 0, 1)).x;
//   float z1 = tex2Dlod(heightmapCoarser, float4(uvCoarser.zw, 0, 1)).x;
  float2 zz = tex2Dlod(heightmap, float4(uv, 0, 1)).xy;
//   float z = tex2Dlod(heightmap, float4(uv, 0, 1)).x;
  float z = (1-alpha)*zz.x+alpha*zz.y;
//  z = zz.x-zz.x;
//   float2 uvDiv2 = uv/2;
//   float4 uvCoarser = float4(uvDiv2+float2(0.5*scaleTranslateTex.x, 0), uvDiv2+float2(0, 0.5*scaleTranslateTex.y));

//  z = z+alpha*(z0+z1)*0.5;
//  z = 0;
  OUT.hpos = mul(mvp, float4(worldPos.x, zz.x*50, worldPos.y, 1));
  OUT.color = color;
//  OUT.color = float4(uv, 0, 1);
  OUT.texCoord = float4(uv, 0, 1);
  OUT.color = alpha*color;

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
