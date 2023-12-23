// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoardNodeFinal.h"
#include "HexNodeFinal.generated.h"

/**
 * 
 */
UCLASS()
class TFT_332_API UHexNodeFinal : public UBoardNodeFinal
{
	GENERATED_BODY()
public:
	//无参数构造函数
	UHexNodeFinal() {};

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UHexNodeFinal* TopRightNode;//右上角棋格；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* RightNode;//右棋格；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* BottomRightNode;//右下棋格；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* TopLeftNode;//左上角棋格；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* LeftNode;//左棋格；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* BottomLeftNode;//左下角棋格；
	//重写获取相邻棋格函数
	virtual TArray<UBoardNodeFinal*>GetNeighbors()override;

	//重写计算两两棋格的实际距离函数
	virtual float GetRealRadiusSize()const override
	{
		return Size * FMath::Sqrt(3) * 0.5;
	}
	
};
