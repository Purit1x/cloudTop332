// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Script.h"

// 用于在动画通知时执行相关的函数，如造成伤害等
void UAnimNotify_Script::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (!MeshComp || !Animation)  // 指针判空
		return;
	AActor* Chequer = MeshComp->GetOwner();
	if (Chequer)  // 指针判空
	{
		UFunction* FunctionToBeProcessed = Chequer->FindFunction(FunctionName);
		if (FunctionToBeProcessed)
		{
			Chequer->ProcessEvent(FunctionToBeProcessed, nullptr);
		}
	}
}

