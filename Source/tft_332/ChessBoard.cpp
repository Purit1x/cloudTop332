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
	//��������
	GenerateChessBoard();
}
//��ʼ������ʱ����
void AChessBoard::PostInitProperties()
{
	Super::PostInitProperties();
	GenerateChessBoard();
}
#if WITH_EDITOR
//�༭���иı�����ʱ����
void AChessBoard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//���С��С���С�ı�ʱ������������
	if (PropertyChangedEvent.Property->GetName() == "Roll"
		|| PropertyChangedEvent.Property->GetName() == "Colume"
		|| PropertyChangedEvent.Property->GetName() == "Size")
	{
		//������������
		GenerateChessBoard();
	}

}
#endif
//�������̣�
void AChessBoard::GenerateChessBoard()
{
	for (auto a : BoardMapNode)
	{
		if(a.Value)
			a.Value->MarkPendingKill();
	}
	//����Map
	BoardMapNode.Reset();
	//�����������
	GenerateHexNodes(BoardSize, BoardRow, BoardColumn);
}
//�������������
void AChessBoard::GenerateHexNodes(float InHexSize, int InRow, int InColumn)
{
	FVector TemHexLocation;
	for (int i = 0; i < InRow; i++)
	{
		for (int j = 0; j < InColumn; j++)
		{
			FGridCoordinates TemHexVector = FGridCoordinates(j, i);
			TemHexLocation.X = 1.5 * InHexSize * i;//��ue��X�����άƽ���y�ᣬ�����㷢�֣���С��ϵΪ1.5���Դ�С��
			if (i % 2 == 0)
			{
				TemHexLocation.Y = FMath::Sqrt(3) * InHexSize * j;//�����㷢�֣��߳�Ϊ1ʱ���������������λ�õ�������3��
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
//��ʼ�����������
void AChessBoard::InitHexNode()
{
	for (auto CurrentNode : BoardMapNode)
	{
		UHexNodeFinal* TemHexNode = Cast<UHexNodeFinal>(CurrentNode.Value);
		if (!TemHexNode)
			continue;
		//���Ͻ����
		FGridCoordinates TemTopRight;
		if (CurrentNode.Key.Y % 2 == 0)
			TemTopRight = CurrentNode.Key + FGridCoordinates(0, 1);
		else
			TemTopRight = CurrentNode.Key + FGridCoordinates(1, 1);
		//�ж��ǲ���HexNodeFinal���ͣ�������ӽ�TopRightNode��
		if (BoardMapNode.Contains(TemTopRight) && BoardMapNode[TemTopRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->TopRightNode = Cast<UHexNodeFinal>(BoardMapNode[TemTopRight]);
		}

		//�ұ����
		FGridCoordinates TemRight=CurrentNode.Key + FGridCoordinates(1, 0);
		if (BoardMapNode.Contains(TemRight) && BoardMapNode[TemRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->RightNode = Cast<UHexNodeFinal>(BoardMapNode[TemRight]);
		}

		//���½����
		FGridCoordinates TemBottomRight;
		if (CurrentNode.Key.Y % 2 == 0)
			TemBottomRight = CurrentNode.Key + FGridCoordinates(0, -1);
		else
			TemBottomRight = CurrentNode.Key + FGridCoordinates(1, -1);
		if (BoardMapNode.Contains(TemBottomRight) && BoardMapNode[TemBottomRight]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->BottomRightNode = Cast<UHexNodeFinal>(BoardMapNode[TemBottomRight]);
		}

		//���½����
		FGridCoordinates TemBottomLeft;
		if (CurrentNode.Key.Y % 2 == 0)
			TemBottomLeft = CurrentNode.Key + FGridCoordinates(-1, -1);
		else
			TemBottomLeft = CurrentNode.Key + FGridCoordinates(0, -1);
		//�ж��ǲ���HexNodeFinal���ͣ�������ӽ�BottomLeftNode��
		if (BoardMapNode.Contains(TemBottomLeft) && BoardMapNode[TemBottomLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->BottomLeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemBottomLeft]);
		}

		//������
		FGridCoordinates TemLeft = CurrentNode.Key + FGridCoordinates(-1, 0);
		//�ж��ǲ���HexNodeFinal���ͣ�������ӽ�LeftNode��
		if (BoardMapNode.Contains(TemLeft) && BoardMapNode[TemLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->LeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemLeft]);
		}

		//���Ͻ����
		FGridCoordinates TemTopLeft;
		if (CurrentNode.Key.Y % 2 == 0)
			TemTopLeft = CurrentNode.Key + FGridCoordinates(-1, 1);
		else
			TemTopLeft = CurrentNode.Key + FGridCoordinates(0, 1);
		//�ж��ǲ���HexNodeFinal���ͣ�������ӽ�TopLeftNode��
		if (BoardMapNode.Contains(TemTopLeft) && BoardMapNode[TemTopLeft]->IsA(UHexNodeFinal::StaticClass()))
		{
			TemHexNode->TopLeftNode = Cast<UHexNodeFinal>(BoardMapNode[TemTopLeft]);
		}
	}
}

//�������������ȡ���
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
	//�����ж���ʼ����յ��Ƿ���Ч��
	if (!FromNode || !ToNode)
		return false;
	if (!BoardMapNode.FindKey(FromNode) || !BoardMapNode.FindKey(ToNode))
		return false;
	//���յ��Լ�StopSteps�����ڵ�����ҵ���
	TArray<UBoardNodeFinal*>ToNodes = GetNodeNeighbors(ToNode, StopSteps);
	for (int i = ToNodes.Num() - 1; i >= 0; i--)
	{
		if (!ToNodes[i]->CanPass(InActor))
			ToNodes.RemoveAt(i);
	}
	//�ж�����յ��Ƿ����
	if (!FromNode->CanPass(InActor)|| ToNodes.Num() == 0)
		return false;
	//�ж��Ƿ��Ѿ������յ�
	if (ToNodes.Contains(FromNode))
		return true;
	//��Ҫ�����Ľڵ㣻
	TArray<UBoardNodeFinal*>PreList;
	//���������Ľڵ㣻
	TArray<UBoardNodeFinal*>PostList;
	//��ǰ�ڵ�
	UBoardNodeFinal* CurrentNode;
	CurrentNode = FromNode;
	//����ʼ����뽫Ҫ�����Ľڵ��У�
	PreList.Add(CurrentNode);
	bool IsFound = false;
	while (!IsFound)
	{
		//����ʼ����뵽�Ѿ������Ľڵ����棬���ӽ�Ҫ�����Ľڵ����Ƴ���
		PreList.Remove(CurrentNode);
		PostList.Add(CurrentNode);
		//�ҵ����ڽڵ�����ڽڵ㣻
		TArray< UBoardNodeFinal*>CurrentNeighbors = CurrentNode->GetNeighbors();
		for (auto Neighbor : CurrentNeighbors)
		{
			//���ڽڵ�Ϊ�գ�������һ���ڵ㣻
			if (!Neighbor)
				continue;
			if (ToNodes.Contains(Neighbor))
			{
				IsFound = true;
				ToNode = Neighbor;
				Neighbor->PreNode = CurrentNode;
			}
			//�����PostList���߲���ͨ����������
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
	//�ҵ���·��
	if (IsFound)
	{
		UBoardNodeFinal* TemNode = ToNode;
		while (TemNode != FromNode)
		{
			Path.Add(TemNode);
			TemNode = Cast<UBoardNodeFinal>(TemNode->PreNode);
		}
		//��ȡ��·��ʱ���յ�->����·������Ҫ��ת�����->�յ��·��
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

	//ʹ��While���������ж�
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
