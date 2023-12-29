// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Script.h"

// �����ڶ���֪ͨʱִ����صĺ�����������˺���
void UAnimNotify_Script::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (!MeshComp || !Animation)  // ָ���п�
		return;
	AActor* Chequer = MeshComp->GetOwner();
	if (Chequer)  // ָ���п�
	{
		UFunction* FunctionToBeProcessed = Chequer->FindFunction(FunctionName);
		if (FunctionToBeProcessed)
		{
			Chequer->ProcessEvent(FunctionToBeProcessed, nullptr);
		}
	}
}

