// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNodeFinal.h"
#include"ChessBoard.h"

//初始化函数；
void UBoardNodeFinal::InitNode(AChessBoard* InChessBoard, FVector InLocation, FGridCoordinates InCoordinate, float InSize)
{
	ChessBoard = InChessBoard;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}


//获取相邻棋格
TArray<UBoardNodeFinal*>UBoardNodeFinal::GetNeighbors()
{
	TArray<UBoardNodeFinal*>Nodes;
	return Nodes;
}
//判断棋格是否有障碍物(有返回False，反之返回True；
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

// 弈子进入棋格
void UBoardNodeFinal::EnterNode(AActor *InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// 弈子离开棋格
void UBoardNodeFinal::LeaveNode(AActor *InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}

// 弈子将棋格设为目标点，棋格预存弈子
void UBoardNodeFinal::ReserveNode(AActor *InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		
	}
}

// 弈子遇到敌人等事件触发，取消寻路
void UBoardNodeFinal::CancelReservation(AActor *InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}
