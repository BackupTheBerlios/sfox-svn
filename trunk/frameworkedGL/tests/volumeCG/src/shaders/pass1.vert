struct outputs {
  float4 hpos : POSITION;
  float4 color : COLOR;
  float4 texCoord1 : TEXCOORD1;
};

struct inputs {
  float4 position : POSITION;
  float4 texCoord : TEXCOORD0;
};

outputs main(inputs IN,
             uniform float4x4 mvp)
{
  outputs OUT;
  OUT.color = IN.texCoord;
  OUT.hpos = mul(mvp, IN.position);
  OUT.texCoord1 = OUT.hpos;
  
  return OUT;
}
