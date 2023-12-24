// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"
#include"HexNodeFinal.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	//生成棋盘
	GenerateChessBoard();
}
//初始化属性时调用
void AChessBoard::PostInitProperties()
{
	Super::PostInitProperties();
	GenerateChessBoard();
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
		GenerateChessBoard();
	}

}
#endif
//生成棋盘；
void AChessBoard::GenerateChessBoard()
{
	for (auto a : BoardMapNode)
	{
		if(a.Value)
			a.Value->MarkPendingKill();
	}
	//重置Map
	BoardMapNode.Reset();
	//重新生成棋格
	GenerateHexNodes(BoardSize, BoardRow, BoardColumn);
}
//生成六边形棋格；
void AChessBoard::GenerateHexNodes(float InHexSize, int InRow, int InColumn)
{
	FVector TemHexLocation;
	for (int i = 0; i < InRow; i++)
	{
		for (int j = 0; j < InColumn; j++)
		{
			FGridCoordinates TemHexVector = FGridCoordinates(j, i);
			TemHexLocation.X = 1.5 * InHexSize * i;//在ue中X代表二维平面的y轴，经计算发现，大小关系为1.5乘以大小；
			if (i % 2 == 0)
			{
				TemHexLocation.Y = FMath::Sqrt(3) * InHexSize * j;//经计算发现，边长为1时相邻两个点的中心位置的相距根号3；
			}
			else
				TemHexLocation.Y = (FMath::Sqrt(3) * InHexSize * j + FMath::Sqrt(3) * 0.5 * InHexSize);
			TemHexLocation.Z = 0;
			TemHexLocation+= GetActorLocation();
			UHexNodeFinal* TemNode= NewObject<UHexNodeFinal>(this);
			TemNode->InitNode(this, TemHexLocation, TemHexVector, InHexSize);
			BoardMapNode.Add(TemHexVector, TemNode);
			

		}
	}
}
//初始化六边形棋格
void AChessBoard::InitHexNode()
{
	for (auto CurrentNode : BoardMapNode)
	{
		UHexNodeFinal* TemHexNode = Cast<UHexNodeFinal>(CurrentNode.Value);
		if (!TemHexNode)
			continue;
		//右上角棋格；
		FGridCoordinates TemTopRight;
		if (CurrentNode.Key.Y % 2 == 0)
			TemTopRight = CurrentNode.Key + FGridCoordinates(0, 1);
		else
			TemTopRight = CurrentNode.Key + FGridCoordinates(1, 1);
		//判断是不是HexNodeFinal类型，是则添加进TopRightNode；
		if (BoardMapNode.Contains(TemTopRight) && BoardMapNode[TemTopRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->TopRightNode = Cast<UHexNodeFinal>(BoardMapNode[TemTopRight]);
		}

		//右边棋格；
		FGridCoordinates TemRight=CurrentNode.Key + FGridCoordinates(1, 0);
		if (BoardMapNode.Contains(TemRight) && BoardMapNode[TemRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->RightNode = Cast<UHexNodeFinal>(BoardMapNode[TemRight]);
		}

		//右下角棋格；
		FGridCoordinates TemBottomRight;
		if (CurrentNode.Key.Y % 2 == 0)
			TemBottomRight = CurrentNode.Key + FGridCoordinates(0, -1);
		else
			TemBottomRight = CurrentNode.Key + FGridCoordinates(1, -1);
		if (BoardMapNode.Contains(TemBottomRight) && BoardMapNode[TemBottomRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->BottomRightNode = Cast<UHexNodeFinal>(BoardMapNode[TemBottomRight]);
		}

		//左下角棋格
		FGridCoordinates TemBottomLeft;
		if (CurrentNode.Key.Y % 2 == 0)
			TemBottomLeft = CurrentNode.Key + FGridCoordinates(-1, -1);
		else
			TemBottomLeft = CurrentNode.Key + FGridCoordinates(0, -1);
		//判断是不是HexNodeFinal类型，是则添加进BottomLeftNode；
		if (BoardMapNode.Contains(TemBottomLeft) && BoardMapNode[TemBottomLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->BottomLeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemBottomLeft]);
		}

		//左边棋格
		FGridCoordinates TemLeft = CurrentNode.Key + FGridCoordinates(-1, 0);
		//判断是不是HexNodeFinal类型，是则添加进LeftNode；
		if (BoardMapNode.Contains(TemLeft) && BoardMapNode[TemLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->LeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemLeft]);
		}

		//左上角棋格
		FGridCoordinates TemTopLeft;
		if (CurrentNode.Key.Y % 2 == 0)
			TemTopLeft = CurrentNode.Key + FGridCoordinates(-1, 1);
		else
			TemTopLeft = CurrentNode.Key + FGridCoordinates(0, 1);
		//判断是不是HexNodeFinal类型，是则添加进TopLeftNode；
		if (BoardMapNode.Contains(TemTopLeft) && BoardMapNode[TemTopLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->TopLeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemTopLeft]);
		}
	}
}

//根据棋盘坐标获取棋格
UBoardNodeFinal* AChessBoard::GetNode(FGridCoordinates InCoord) const
{
	if (BoardMapNode.Contains(InCoord))
		return BoardMapNode[InCoord];
	else
		return nullptr;
};
bool AChessBoard::FindPath(TArray<UBoardNodeFinal*>& Path, AActor* InActor, UBoardNodeFinal* FromNode, UBoardNodeFinal* ToNode, int StopSteps)
{
	Path.Empty();
	//首先判断起始点和终点是否有效；
	if (!FromNode || !ToNode)
		return false;
	if (!BoardMapNode.FindKey(FromNode) || !BoardMapNode.FindKey(ToNode))
		return false;
	//将终点以及StopSteps距离内的棋格都找到；
	TArray<UBoardNodeFinal*>ToNodes = GetNodeNeighbors(ToNode, StopSteps);
	for (int i = ToNodes.Num() - 1; i >= 0; i--)
	{
		if (!ToNodes[i]->CanPass(InActor))
			ToNodes.RemoveAt(i);
	}
	//判断起点终点是否存在
	if (!FromNode->CanPass(InActor)|| ToNodes.Num() == 0)
		return false;
	//判断是否已经到达终点
	if (ToNodes.Contains(FromNode))
		return true;
	//将要遍历的节点；
	TArray<UBoardNodeFinal*>PreList;
	//遍历结束的节点；
	TArray<UBoardNodeFinal*>PostList;
	//当前节点
	UBoardNodeFinal* CurrentNode;
	CurrentNode = FromNode;
	//将起始点加入将要遍历的节点中；
	PreList.Add(CurrentNode);
	bool IsFound = false;
	while (!IsFound)
	{
		//将起始点加入到已经遍历的节点里面，并从将要遍历的节点中移除；
		PreList.Remove(CurrentNode);
		PostList.Add(CurrentNode);
		//找到现在节点的相邻节点；
		TArray< UBoardNodeFinal*>CurrentNeighbors = CurrentNode->GetNeighbors();
		for (auto Neighbor : CurrentNeighbors)
		{
			//相邻节点为空，进行下一个节点；
			if (!Neighbor)
				continue;
			if (ToNodes.Contains(Neighbor))
			{
				IsFound = true;
				ToNode = Neighbor;
				Neighbor->PreNode = CurrentNode;
			}
			//如果在PostList或者不能通行则跳过；
			if (PostList.Contains(Neighbor) || !Neighbor->CanPass(InActor))
				continue;
			if (!PreList.Contains(Neighbor))
			{
				PreList.Add(Neighbor);
				Neighbor->PreNode =CurrentNode;
			}
		}
		if (PreList.Num() <= 0)
			break;
		else
			CurrentNode = PreList[0];
	}
	PreList.Empty();
	PostList.Empty();
	//找到了路径
	if (IsFound)
	{
		UBoardNodeFinal* TemNode = ToNode;
		while (TemNode != FromNode)
		{
			Path.Add(TemNode);
			TemNode = Cast<UBoardNodeFinal>(TemNode->PreNode);
		}
		//获取的路径时从终点->起点的路径，需要反转回起点->终点的路径
		Algo::Reverse(Path);

		return true;
	}

	return false;
}
TArray<UBoardNodeFinal*> AChessBoard::GetNodeNeighbors(UBoardNodeFinal* InNode, int InStep) const 
{
	int neighborSteps = InStep;
	TArray<UBoardNodeFinal*> nowCheckList;
	TArray<UBoardNodeFinal*> nextCheckList;
	TArray<UBoardNodeFinal*> findList;
	nextCheckList.AddUnique(InNode);
	findList.AddUnique(InNode);

	//使用While，向外层层判断
	while (neighborSteps > 0)
	{
		nowCheckList = nextCheckList;
		nextCheckList.Empty();
		for (UBoardNodeFinal* a : nowCheckList)
		{
			if (!a)
				continue;
			TArray<UBoardNodeFinal*> neighbors = a->GetNeighbors();
			for (UBoardNodeFinal* b : neighbors)
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
};
