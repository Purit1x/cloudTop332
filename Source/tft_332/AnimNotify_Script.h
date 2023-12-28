// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Script.generated.h"

/**
 * 
 */
UCLASS()
class TFT_332_API UAnimNotify_Script : public UAnimNotify
{
	GENERATED_BODY()
public:
	// 执行与动画匹配的函数
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;

	// 存储与动画匹配的函数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FunctionName;
};
