#include "Buffers.hlsli"
#include "Utils.hlsli"

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float2 lightUV : TEXCOORD1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint4 indices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
    
    uint instanceID : SV_INSTANCEID;
    row_major matrix world : INST;
};

struct VertexOut
{
    float4 posH : SV_Position;
    float4 posV : POSITION;
    float4 posW : POSITION2;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

row_major matrix GetAnimationMatrix(VertexIn input)
{
    float indices[4] = { input.indices.x, input.indices.y, input.indices.z, input.indices.w };
    float weights[4] = { input.weight.x, input.weight.y, input.weight.z, input.weight.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];

    animIndex[0] = transitionDesc[input.instanceID].curr.animIndex;
    currFrame[0] = transitionDesc[input.instanceID].curr.currFrame;
    nextFrame[0] = transitionDesc[input.instanceID].curr.nextFrame;
    ratio[0] = transitionDesc[input.instanceID].curr.ratio;
    
    animIndex[1] = transitionDesc[input.instanceID].next.animIndex;
    currFrame[1] = transitionDesc[input.instanceID].next.currFrame;
    nextFrame[1] = transitionDesc[input.instanceID].next.nextFrame;
    ratio[1] = transitionDesc[input.instanceID].next.ratio;
    
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

        // 현재 애니메이션 보간
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
            // 다음 애니메이션 보간
            matrix nextResult = lerp(curr, next, ratio[1]);
            
            // 현재, 다음 보간
            //result = lerp(result, nextResult, transitionDesc[input.instanceID].transitionRatio);
            result = lerp(nextResult, result, transitionDesc[input.instanceID].transitionRatio);
        }

        transform += mul(weights[i], result);
    }

    return transform;
}


// temp_float0 : Add
// temp_float1 : uv animation 

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    row_major matrix _WV = mul(input.world, VTM);
    row_major matrix _WVP = mul(_WV, PTM);
    
  
    row_major matrix boneMat = GetAnimationMatrix(input);
        
    output.posH = mul(float4(input.pos, 1.f), boneMat);
    output.posH = output.posH / output.posH.w;
        
    output.posH = mul(output.posH, input.world);
    output.posW = output.posH;
    float4 vertexWorldPos = output.posH;
    vertexWorldPos.y /= 2;
    vertexWorldPos.y += temp_float0;
    vertexWorldPos.y = saturate(vertexWorldPos.y);
        
    float2 tempUV = (input.uv + float2(temp_float1, temp_float1)) * 2048;
    if (tempUV.x > 2048)
        tempUV.x = tempUV.x - 2048;
    if (tempUV.y > 2048)
        tempUV.y = tempUV.y - 2048;
        
    float noise = Temp0Map.Load(float3(tempUV, 0)).r;
        
    float3 result = BlendHardLight(float3(noise, noise, noise), float3(vertexWorldPos.y, vertexWorldPos.y, vertexWorldPos.y));
        
    float4 vertexNormalWS = normalize(mul(float4(input.normal, 0.f), boneMat));
    vertexNormalWS = normalize(mul(vertexNormalWS, input.world));
        
    float3 vertexOffset = mul(result, vertexNormalWS.xyz);
    vertexOffset *= 0.01;
    float3 axis = float3(0, 0.5, 0.5);
    axis *= 4;
        
    vertexOffset += lerp(float3(0, 0, 0), axis, result);

    output.posH.xyz += vertexOffset;
    output.posH = mul(output.posH, mul(VTM, PTM));
        
        
    
    output.posV = mul(float4(input.pos, 1.f), boneMat);
    output.posV = mul(output.posV, _WV);
    
    output.color = input.color;
  
    
    return output;
}