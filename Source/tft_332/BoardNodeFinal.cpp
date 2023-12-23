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


