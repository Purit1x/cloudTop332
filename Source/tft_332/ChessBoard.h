// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardNodeFinal.h"
#include "ChessBoard.generated.h"
UCLASS()
class TFT_332_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoard();
	//棋盘的形状
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EBoardType MapType = EBoardType::None;
	//棋盘大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoardSize;
	//棋盘行数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardRow;
	//棋盘列数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardColumn;
	//棋盘棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap< FGridCoordinates, UBoardNodeFinal*> BoardMapNode;





protected:
	//重写BeginPlay函数
	virtual void BeginPlay() override;
	//重写初始化属性函数；
	virtual void PostInitProperties()override;

#if WITH_EDITOR
	//重写在Editor中修改属性函数；
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)override;
#endif
public:	
	//生成棋盘函数,将其余函数进行一个封装；
	UFUNCTION(BlueprintCallable)//BlueprintCallable函数可以在蓝图编辑器中调用
		void GenerateChessBoard();
	//生成六边形棋格；
	UFUNCTION()
	void GenerateHexNodes(float InHexSize, int InRow, int InColumn);
	//初始化六边形棋格
	UFUNCTION()
	void InitHexNode();
	//获取坐标棋格
	UFUNCTION()
	UBoardNodeFinal* GetNode(FGridCoordinates InCoord) const;
	//自动寻路算法,StopSteps代表距离目标位置的距离；
	UFUNCTION(BlueprintCallable)
	bool FindPath(TArray<UBoardNodeFinal*>& Path, AActor* InActor, UBoardNodeFinal* FromNode, UBoardNodeFinal* ToNode, int StopSteps = 0);
	//寻路-获取目标点范围内的棋格
	UFUNCTION()
	TArray<UBoardNodeFinal*> GetNodeNeighbors(UBoardNodeFinal* InNode, int InStep = 0) const;

};
