// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNodeFinal.h"
#include"ChessBoard.h"

//��ʼ��������
void UBoardNodeFinal::InitNode(AChessBoard* InChessBoard, FVector InLocation, FGridCoordinates InCoordinate, float InSize)
{
	ChessBoard = InChessBoard;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}


//��ȡ�������
TArray<UBoardNodeFinal*>UBoardNodeFinal::GetNeighbors()
{
	TArray<UBoardNodeFinal*>Nodes;
	return Nodes;
}
//�ж�����Ƿ����ϰ���(�з���False����֮����True��
bool UBoardNodeFinal::CanPass(AActor* InActor)const
{
	if (PassFlag >= EBoardPassFlag::Block)
		return false;
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
void UBoardNodeFinal::EnterNode(AActor *InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// �����뿪���
void UBoardNodeFinal::LeaveNode(AActor *InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}

// ���ӽ������ΪĿ��㣬���Ԥ������
void UBoardNodeFinal::ReserveNode(AActor *InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		
	}
}

// �����������˵��¼�������ȡ��Ѱ·
void UBoardNodeFinal::CancelReservation(AActor *InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}
