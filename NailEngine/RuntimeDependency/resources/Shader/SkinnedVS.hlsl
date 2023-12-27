#include "Buffers.hlsli"

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint4 indices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct VertexOut
{
    float4 posH : SV_Position;
    float4 posV : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 normalV : NORMAL;
    float3 tangentV : TANGENT;
    float3 biNormalV : BINORMAL;
};

row_major matrix GetAnimationMatrix(VertexIn input)
{
    float indices[4] = { input.indices.x, input.indices.y, input.indices.z, input.indices.w };
    float weights[4] = { input.weight.x, input.weight.y, input.weight.z, input.weight.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];

    animIndex[0] = transitionDesc.curr.animIndex;
    currFrame[0] = transitionDesc.curr.currFrame;
    nextFrame[0] = transitionDesc.curr.nextFrame;
    ratio[0] = transitionDesc.curr.ratio;

    animIndex[1] = transitionDesc.next.animIndex;
    currFrame[1] = transitionDesc.next.currFrame;
    nextFrame[1] = transitionDesc.next.nextFrame;
    ratio[1] = transitionDesc.next.ratio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
        curr = matrix(c0, c1, c2, c3);

        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio[0]);

		// 다음 애니메이션
        if (animIndex[1] >= 0)
        {
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);

            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);

            matrix nextResult = lerp(curr, next, ratio[1]);
            result = lerp(result, nextResult, transitionDesc.transitionRatio);
        }

        transform += mul(weights[i], result);
    }

    return transform;
}

VertexOut main(VertexIn input)
{
    VertexOut output;
    row_major matrix WV = mul(WTM, VTM);
    
    row_major matrix boneMat = GetAnimationMatrix(input);
    
    output.posH = mul(float4(input.pos, 1.f), boneMat);
    output.posH = mul(output.posH, WVP);
    output.posV = mul(float4(input.pos, 1.f), boneMat);
    output.posV = mul(output.posV, WV);
    output.color = input.color;
    output.uv = input.uv;
    output.normalV = normalize(mul(float4(input.normal, 0.f), boneMat));
    output.normalV = normalize(mul(float4(output.normalV, 0.f), WV));
    
    output.tangentV = normalize(mul(float4(input.tangent, 0.f), boneMat));
    output.tangentV = normalize(mul(float4(output.tangentV, 0.f), WV));
    
    output.biNormalV = normalize(cross(output.tangentV, output.normalV));
    
    return output;
}