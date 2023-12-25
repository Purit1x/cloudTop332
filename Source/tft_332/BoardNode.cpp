// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardNode.h"
#include"ChessBoard.h"

//初始化函数
void UBoardNode::InitNode(AChessBoard* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize)
{
	GridMap = InGridMap;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}

//获取相邻棋格
TArray<UBoardNode*> UBoardNode::GetNeighbors()
{
	TArray<UBoardNode*> Nodes;
	return Nodes;
}

//判断棋格是否能通行
bool UBoardNode::CanPass(AActor* InActor) const
{
	if (PassFlag >= ENodePassFlag::Block)
		return false;
	//判断是否有其他棋子站在当前棋格上
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
void UBoardNode::EnterNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// 弈子离开棋格
void UBoardNode::LeaveNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}

// 弈子将棋格设为目标点，棋格预存弈子
void UBoardNode::ReserveNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);

	}
}

// 弈子遇到敌人等事件触发，取消寻路
void UBoardNode::CancelReservation(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);

	}
}