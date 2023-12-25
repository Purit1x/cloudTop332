// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"
#include "HexNode.h"
#include "ProceduralMeshComponent.h"
//构造函数
AChessBoard::AChessBoard()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("MapMesh");
	this->SetRootComponent(Mesh);
}

//游戏第一帧调用
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	//重新生成棋盘
	GenerateGridMap();
}

//初始化属性时调用
void AChessBoard::PostInitProperties()
{
	Super::PostInitProperties();
	GenerateGridMap();
}

#if WITH_EDITOR
//编辑器中改变属性时调用
void AChessBoard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//当行、列、大小改变时重新生成棋盘
	if (PropertyChangedEvent.Property->GetName() == "Roll"
		|| PropertyChangedEvent.Property->GetName() == "Colume"
		|| PropertyChangedEvent.Property->GetName() == "Size")
	{
		//重新生成棋盘
		GenerateGridMap();
	}

}
#endif

//生成棋盘入口
void AChessBoard::GenerateGridMap()
{
	for (auto Node : NodeMap)
	{
		//标记垃圾回收
		if (Node.Value)
			Node.Value->MarkPendingKill();
	}
	//重置Map
	NodeMap.Reset();
	//重新生成棋格
	GenerateNodes(Size, Roll, Colume);
	//重新生成模型
	GenerateMapMesh();
}

//生成棋格入口
void AChessBoard::GenerateNodes(float InSize, int InRoll, int InColume)
{
	switch (MapType)
	{
	case EGridType::None:
		break;
		//六边形棋格
	case EGridType::Hex:
		GenerateHexNodes(InSize, InRoll, InColume);
		break;
	default:
		break;
	}
	//初始化棋格
	InitNodes();
}

//生成六边形棋格
void AChessBoard::GenerateHexNodes(float InHexSize, int InRoll, int InColume)
{
	FVector tHexLocation;
	for (int i = 0; i < InRoll; ++i)
	{
		for (int j = 0; j < InColume; ++j)
		{
			//UE中横向坐标轴为Y，纵向坐标轴为X，需要调换在二维坐标系XY的值
			FGridVector tHexVector = FGridVector(j, i);
			tHexLocation.X = 1.5 * InHexSize * i;
			tHexLocation.Y = i % 2 == 0 ? (FMath::Sqrt(3) * InHexSize * j) : (FMath::Sqrt(3) * InHexSize * j + FMath::Sqrt(3) * 0.5 * InHexSize);
			tHexLocation.Z = 0;
			tHexLocation += GetActorLocation();
			UHexNode* tNode = NewObject<UHexNode>(this);
			//棋格初始化
			tNode->InitNode(this, tHexLocation, tHexVector, InHexSize);
			NodeMap.Add(tHexVector, tNode);
		}
	}
}

//初始化棋格入口
void AChessBoard::InitNodes()
{
	switch (MapType)
	{
	case EGridType::None:
		break;
		//六边形棋格
	case EGridType::Hex:
		InitHexNodes();
		break;
	default:
		break;
	}
}

//初始化六边形棋格
void AChessBoard::InitHexNodes()
{
	//设置六边形棋格相邻棋格，判断对应坐标棋格是否存在，并赋值
	for (auto CurrentNode : NodeMap)
	{
		UHexNode* TemHexNode = Cast<UHexNode>(CurrentNode.Value);
		if (!TemHexNode)
			continue;
		FGridVector tRightUp = CurrentNode.Key.Y % 2 == 0 ? CurrentNode.Key + FGridVector(0, 1) : CurrentNode.Key + FGridVector(1, 1);
		if (NodeMap.Contains(tRightUp) && NodeMap[tRightUp]->IsA(UHexNode::StaticClass()))
			TemHexNode->RightUpNode = Cast<UHexNode>(NodeMap[tRightUp]);

		FGridVector tRight = CurrentNode.Key + FGridVector(1, 0);
		if (NodeMap.Contains(tRight) && NodeMap[tRight]->IsA(UHexNode::StaticClass()))
			TemHexNode->RightNode = Cast<UHexNode>(NodeMap[tRight]);

		FGridVector tRightDown = CurrentNode.Key.Y % 2 == 0 ? CurrentNode.Key + FGridVector(0, -1) : CurrentNode.Key + FGridVector(1, -1);
		if (NodeMap.Contains(tRightDown) && NodeMap[tRightDown]->IsA(UHexNode::StaticClass()))
			TemHexNode->RightDownNode = Cast<UHexNode>(NodeMap[tRightDown]);

		FGridVector tLeftDown = CurrentNode.Key.Y % 2 == 0 ? CurrentNode.Key + FGridVector(-1, -1) : CurrentNode.Key + FGridVector(0, -1);
		if (NodeMap.Contains(tLeftDown) && NodeMap[tLeftDown]->IsA(UHexNode::StaticClass()))
			TemHexNode->LeftDownNode = Cast<UHexNode>(NodeMap[tLeftDown]);

		FGridVector tLeft = CurrentNode.Key + FGridVector(-1, 0);
		if (NodeMap.Contains(tLeft) && NodeMap[tLeft]->IsA(UHexNode::StaticClass()))
			TemHexNode->LeftNode = Cast<UHexNode>(NodeMap[tLeft]);

		FGridVector tLeftUp = CurrentNode.Key.Y % 2 == 0 ? CurrentNode.Key + FGridVector(-1, 1) : CurrentNode.Key + FGridVector(0, 1);
		if (NodeMap.Contains(tLeftUp) && NodeMap[tLeftUp]->IsA(UHexNode::StaticClass()))
			TemHexNode->LeftUpNode = Cast<UHexNode>(NodeMap[tLeftUp]);

	}
}

//根据棋盘坐标获取棋格
UBoardNode* AChessBoard::GetNode(FGridVector InCoord) const
{
	if (NodeMap.Contains(InCoord))
		return NodeMap[InCoord];
	else
		return nullptr;
}

bool AChessBoard::FindPath(TArray<UBoardNode*>& Path, AActor* InActor, UBoardNode* FromNode, UBoardNode* ToNode, int StopSteps)
{
	Path.Empty();
	//安全判断
	if (!FromNode || !ToNode)
		return false;
	if (!NodeMap.FindKey(FromNode) || !NodeMap.FindKey(ToNode))
		return false;
	//获取实际所有终点
	TArray<UBoardNode*> ToNodes = GetNodeNeighbors(ToNode, StopSteps);
	for (int i = ToNodes.Num() - 1; i >= 0; i--)
	{
		if (!ToNodes[i]->CanPass(InActor))
			ToNodes.RemoveAt(i);
	}
	//判断起点终点是否存在
	if (!FromNode->CanPass(InActor))
		return false;
	if (ToNodes.Num() == 0)
		return false;
	//判断是否已经到达终点
	if (ToNodes.Contains(FromNode))
		return true;

	//将要遍历的路点
	TArray<UBoardNode*> openList;
	//已经完成遍历的路点
	TArray<UBoardNode*> closeList;
	//当前所在路点
	UBoardNode* nowNode;
	nowNode = FromNode;
	openList.Add(nowNode);
	bool bFinded = false;
	//A*寻路
	while (!bFinded)
	{
		//移除openList，加入closeList
		openList.Remove(nowNode);
		closeList.Add(nowNode);
		//获取相邻路点
		TArray<UBoardNode*> neighbors = nowNode->GetNeighbors();
		for (auto neighbor : neighbors)
		{
			if (!neighbor)
				continue;
			//判断相邻路点是否为终点
			if (ToNodes.Contains(neighbor))
			{
				bFinded = true;
				ToNode = neighbor;
				neighbor->PreNode = nowNode;
			}
			//如果在closeList或不能通行则跳过
			if (closeList.Contains(neighbor) || !neighbor->CanPass(InActor))
				continue;
			//如果不在openlist，则加入openlist的队尾，以备后用
			if (!openList.Contains(neighbor))
			{
				openList.Add(neighbor);
				neighbor->PreNode = nowNode;
			}
		}
		//取出队首的路点，设置为下次循环遍历的路点
		if (openList.Num() <= 0)
			break;
		else
			nowNode = openList[0];
	}
	openList.Empty();
	closeList.Empty();
	//找到了路径
	if (bFinded)
	{
		UBoardNode* tNode = ToNode;
		while (tNode != FromNode)
		{
			Path.Add(tNode);
			tNode = Cast<UBoardNode>(tNode->PreNode);
		}
		//获取的路径时从终点->起点的路径，需要反转回起点->终点的路径
		Algo::Reverse(Path);

		return true;
	}

	return false;

}

//判断路径是否存在
bool AChessBoard::IsPathExist(AActor* InActor, UBoardNode* FromNode, UBoardNode* ToNode, int StopSteps)
{
	//回传FindPath的值
	TArray<UBoardNode*> Path;
	return FindPath(Path, InActor, FromNode, ToNode, StopSteps);
}

//获取相邻棋格
TArray<UBoardNode*> AChessBoard::GetNodeNeighbors(UBoardNode* InNode, int InStep) const
{
	int neighborSteps = InStep;
	TArray<UBoardNode*> nowCheckList;
	TArray<UBoardNode*> nextCheckList;
	TArray<UBoardNode*> findList;
	nextCheckList.AddUnique(InNode);
	findList.AddUnique(InNode);

	//使用While，向外层层判断
	while (neighborSteps > 0)
	{
		nowCheckList = nextCheckList;
		nextCheckList.Empty();
		for (UBoardNode* a : nowCheckList)
		{
			if (!a)
				continue;
			TArray<UBoardNode*> neighbors = a->GetNeighbors();
			for (UBoardNode* b : neighbors)
			{
				if (findList.Contains(b))
					continue;
				findList.AddUnique(b);
				nextCheckList.AddUnique(b);
			}
		}
		neighborSteps -= 1;
	}

	return findList;
}

//生成棋盘模型
void AChessBoard::GenerateMapMesh()
{
	//模型索引，表示棋格是Mesh上第几个分区
	int Index = 0;
	//重置
	Mesh->ClearAllMeshSections();
	for (auto a : NodeMap)
	{
		if (!a.Value)
			continue;
		TArray<FVector> Vertices;
		TArray<int32> Indecies;
		TArray<FVector> Normals;
		TArray<FVector2D> UV1, EmptyArray;
		TArray<FColor> VertexColors;
		TArray<FVector> Tangents;
		TArray<FProcMeshTangent> MeshTangents;
		//索引赋值
		a.Value->MeshIndex = Index;
		//调用DrawNode，以引用方式回传绘制信息
		a.Value->DrawNode(Vertices, Indecies, Normals, UV1, VertexColors, Tangents, GetActorLocation());
		//用切线向量生成切线结构体
		for (FVector& b : Tangents)
			MeshTangents.Add(FProcMeshTangent(b, false));
		//生成索引区块模型
		Mesh->CreateMeshSection(Index, Vertices, Indecies, Normals, UV1, EmptyArray, EmptyArray, EmptyArray, VertexColors, MeshTangents, false);
		//设置材质
		ResetNodeMaterial(a.Value);
		Index++;
	}

}

//设置模型材质
void AChessBoard::SetNodeMaterial(UBoardNode* InNode, UMaterialInterface* InMaterial)
{
	if (!InNode || !InMaterial)
		return;
	Mesh->SetMaterial(InNode->MeshIndex, InMaterial);
}

//重置模型材质
void AChessBoard::ResetNodeMaterial(UBoardNode* InNode)
{
	if (!InNode)
		return;

	if (InNode->NodeActors.Num() > 0 && DebugMaterial1)
	{
		SetNodeMaterial(InNode, DebugMaterial1);
		return;
	}
	//根据通行状态设置材质
	if (InNode->PassFlag == ENodePassFlag::Pass && PassMaterial)
		SetNodeMaterial(InNode, PassMaterial);
	else if (InNode->PassFlag == ENodePassFlag::Block && BlockMaterial)
		SetNodeMaterial(InNode, BlockMaterial);

}

//重置所有模型材质
void AChessBoard::ResetNodeMaterialAll()
{
	for (auto a : NodeMap)
	{
		if (!a.Value)
			continue;
		ResetNodeMaterial(a.Value);
	}
}

//判断是否在棋格内
UBoardNode* AChessBoard::CheckHitNode(FVector InPosition)
{
	UBoardNode* tHitNode = nullptr;
	//判断Z轴，差距过大直接返回
	if (FMath::Abs(InPosition.Z - GetActorLocation().Z) > 1)
		return tHitNode;
	switch (MapType)
	{
	case EGridType::None:
		break;
	case EGridType::Hex:
		tHitNode = CheckHitHexNode(InPosition);
		break;
	default:
		break;
	}
	return tHitNode;
}

//判断是否在六边形内
UBoardNode* AChessBoard::CheckHitHexNode(FVector InPosition)
{
	UBoardNode* tHitNode = nullptr;
	for (auto h : NodeMap)
	{
		if (!h.Value)
			continue;
		float x = FMath::Abs(InPosition.X - h.Value->Location.X);
		float y = FMath::Abs(InPosition.Y - h.Value->Location.Y);
		float a = h.Value->Size;
		if (x > a || y > FMath::Sqrt(3) * 0.5 * a)
			continue;
		if (a - x > y / FMath::Sqrt(3))
		{
			tHitNode = h.Value;
			break;
		}
	}
	return tHitNode;
}