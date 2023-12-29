// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseControl.h"
#include "EngineUtils.h"
#include "../ChessBoard.h"
#include "HexNode.generated.h"

#define TOSELL 1

//构造函数
AMouseControl::AMouseControl()
{
	//设置鼠标可见
	this->bShowMouseCursor = true;

	//设定为还未选择初始位置
	AlreadySelected = false;

	StartNode = nullptr;

}

AChessBoard*
AMouseControl::GetBattleMap()
{
	if (BattleMap != nullptr)//已经存在BattleMap
	{
		return BattleMap;
	}
	else//不存在BattleMap，使用迭代器遍历获取BattleMap
	{
		for (TActorIterator<AChessBoard> Itr(GetWorld()); Itr; ++Itr)
		{
			if (!*Itr)
				continue;
			BattleMap = *Itr;
			break;
		}
		return BattleMap;
	}
}

//绑定按键
void
AMouseControl::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMouseControl::LeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMouseControl::RightClick);
}

//鼠标左键对应操作
void
AMouseControl::LeftClick()
{
	//先保证棋盘已经存在
	if (BattleMap != nullptr)
	{
		return;
	}

	UBoardNode* tHitNode = BattleMap->CheckHitNode(GetMouseCurrentPosition());
	
	//如果没有选择好起点格
	if (StartNode == nullptr)
	{
		//如果对应网格为空且没有预先选择起点格，则应控制小小英雄移动
		if (tHitNode->NodeActors.Num() == 0)
		{

		}
		//如果对应网格不为空，则将其变为起点格
		else
		{
			StartNode = tHitNode;
		}
	}
	//如果已经选择好了起点格，则一定是交换
	else
	{
		ExchangeNode(StartNode, tHitNode);
		StartNode = nullptr;
	}
}

void
AMouseControl::RightClick()
{
	//将鼠标点击到的位置转化成棋盘上的网格
	
	//如果对应棋格为空，则通过AlreadySelected进行下一步判断
	//如果AlreadySelected为true，则将StartNode清空（对应返回值为0）
	//如果AlreadySelected为false，则不进行任何操作（对应返回值为1）
	
	//如果对应棋格不为空，则对应出售弈子操作
	//如果选定的棋格为商店区，则不进行任何操作（或在屏幕上进行文字提醒）（对应返回值为2）
	//如果选定的棋格为其他区域，则移除该点的棋子并返还金币（对应返回值为3）
}

FVector
AMouseControl::GetMouseCurrentPosition()const
{
	if (!BattleMap)
	{
		return FVector::ZeroVector;
	}
	else
	{
		FVector WorldLocation;//代表游戏世界中的坐标
		FVector WorldDirection;//代表从该点出发的方向（实际上这应该用不到）
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			//获取Z轴差值
			float tScale = BattleMap->GetActorLocation().Z - WorldLocation.Z;
			tScale /= WorldDirection.Z;
			WorldLocation += tScale * WorldDirection;
			return WorldLocation;
		}
		return FVector::ZeroVector;
	}
}

bool 
AMouseControl::ExchangeNode(UBoardNode* Start, UBoardNode* End)
{
	return true;
}
