// Fill out your copyright notice in the Description page of Project Settings.


#include "HexNodeFinal.h"

//获取相邻的棋格
TArray<UBoardNodeFinal*>UHexNodeFinal::GetNeighbors()
{
	TArray< UBoardNodeFinal*> AdjoingNodes;
	if (TopRightNode)
		AdjoingNodes.Add(TopRightNode);
	if(RightNode)
		AdjoingNodes.Add(RightNode);
	if (BottomRightNode)
		AdjoingNodes.Add(BottomRightNode);
	if (LeftNode)
		AdjoingNodes.Add(LeftNode);
	if (TopLeftNode)
		AdjoingNodes.Add(TopLeftNode);
	if (BottomLeftNode)
		AdjoingNodes.Add(BottomLeftNode);
	return AdjoingNodes;
}

void UHexNodeFinal::DrawNode(TArray<FVector> &InVertices, TArray<int32> &InIndecies,
	TArray<FVector> &InNormals, TArray<FVector2D> &InUV, TArray<FColor> &InVertexColors,
	TArray<FVector> &InTangents, FVector InOffset)
{
	//顶点部分；顺时针的命名；一共七个顶点
	TArray<FVector>HexagonVertex;
	HexagonVertex[0] = Location - InOffset;//中心点
	HexagonVertex[1] = FVector(Size, 0, 0) + Location - InOffset;
	HexagonVertex[2] = FVector(0.5 * Size, 0.5 * FMath::Sqrt(3) * Size, 0) + Location - InOffset;
	HexagonVertex[3] = FVector(-0.5 * Size, 0.5 * FMath::Sqrt(3) * Size, 0) + Location - InOffset;
	HexagonVertex[4] = FVector(-Size, 0, 0) + Location - InOffset;
	HexagonVertex[5] = FVector(-0.5 * Size, -0.5 * FMath::Sqrt(3) * Size, 0) + Location - InOffset;
	HexagonVertex[6] = FVector(0.5 * Size, -0.5 * FMath::Sqrt(3) * Size, 0) + Location - InOffset;
	//顶点索引,使用顺时针进行绘制；
	TArray<int32> Indices;
	Indices.AddUninitialized(18);
	Indices[0] = 0; Indices[1] = 2; Indices[2] = 1;
	Indices[3] = 0; Indices[4] = 3; Indices[5] = 2;
	Indices[6] = 0; Indices[7] = 4; Indices[8] = 3;
	Indices[9] = 0; Indices[10] = 5; Indices[11] = 4;
	Indices[12] = 0; Indices[13] = 6; Indices[14] = 5;
	Indices[15] = 0; Indices[16] = 1; Indices[17] = 6;
	//法线
	TArray<FVector> Normals;
	Normals.Init(FVector(0, 0, 1), 7);
	//UV
	TArray<FVector2D> UV;
	UV.Init(FVector2D(0.5f, 0.5f), 7);//最中心的Uv偏移量是（0.5，0.5）；
	UV[1] += FVector2D(0.0f, 0.5f);
	UV[2] += FVector2D(0.25f * FMath::Sqrt(3), 0.25f);
	UV[3] += FVector2D(0.25f * FMath::Sqrt(3), -0.25f);
	UV[4] += FVector2D(0.0f, -0.5f);
	UV[5] += FVector2D(-0.25f * FMath::Sqrt(3), -0.25f);
	UV[6] += FVector2D(-0.25f * FMath::Sqrt(3), 0.25f);
	//顶点颜色
	TArray<FColor> VertexColors;
	VertexColors.Init(FColor::White, 7);
	//切线
	TArray<FVector> Tangents;
	Tangents.Init(FVector(1, 0, 0), 7);
	//回传
	InVertices = HexagonVertex;
	InIndecies = Indices;
	InNormals = Normals;
	InUV = UV;
	InVertexColors = VertexColors;
	InTangents = Tangents;
}





