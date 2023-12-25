// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNode.h"
#include"ChessBoard.h"

//��ʼ������
void UBoardNode::InitNode(AChessBoard* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize)
{
	GridMap = InGridMap;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}

//��ȡ�������
TArray<UBoardNode*> UBoardNode::GetNeighbors()
{
	TArray<UBoardNode*> Nodes;
	return Nodes;
}

//�ж�����Ƿ���ͨ��
bool UBoardNode::CanPass(AActor* InActor) const
{
	if (PassFlag >= ENodePassFlag::Block)
		return false;
	//�ж��Ƿ�����������վ�ڵ�ǰ�����
	if (NodeActors.Num() > 0 && InActor)
	{
		if (NodeActors.Contains(InActor))
			return true;
		else
			return false;
	}
	return true;
}

// ���ӽ������
void UBoardNode::EnterNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// �����뿪���
void UBoardNode::LeaveNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}

// ���ӽ������ΪĿ��㣬���Ԥ������
void UBoardNode::ReserveNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// �����������˵��¼�������ȡ��Ѱ·
void UBoardNode::CancelReservation(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}