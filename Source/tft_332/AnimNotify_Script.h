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
	// ִ���붯��ƥ��ĺ���
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;

	// �洢�붯��ƥ��ĺ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FunctionName;
};
