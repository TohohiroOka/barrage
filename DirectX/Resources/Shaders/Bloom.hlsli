cbuffer cbuff0 : register(b0)
{
    float strength; //�A�E�g���C���̐F
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};