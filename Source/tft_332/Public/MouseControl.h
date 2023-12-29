// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../ChessBoard.h"
#include "MouseControl.generated.h"

/**
 * 
 */
class AChessBoard;
class UBoardNode;
class APawn;

UCLASS()
class TFT_332_API AMouseControl : public APlayerController
{
	GENERATED_BODY()
public:
	virtual	void SetupInputComponent()override;

	//构造函数
	AMouseControl();

	//当前棋盘
	UPROPERTY(BlueprintReadWrite)
	AChessBoard* BattleMap;

	UFUNCTION(BlueprintCallable)
	AChessBoard* GetBattleMap();


	//鼠标操纵的初始位置（不需要终点位置）
	UPROPERTY(BlueprintReadWrite)
	UBoardNode* StartNode;

	//用于判断是否已经有选取的位置
	bool AlreadySelected;

	//获取鼠标对应位置
	FVector GetMouseCurrentPosition()const;

	//左键事件-用于移动弈子或移动小小英雄
	UFUNCTION(BlueprintCallable)
	void LeftClick();

	//右键事件-用于出售弈子或消除左键选中操作
	UFUNCTION(BlueprintCallable)
	void RightClick();

	//交换选中的两个位置-左键选中后的一种可能情况
	UFUNCTION(BlueprintCallable)
	bool ExchangeNode(UBoardNode* Start, UBoardNode* End);
};
