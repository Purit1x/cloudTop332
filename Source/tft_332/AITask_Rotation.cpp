// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_Rotation.h"

// ����AI��ת����
UAITask_Rotation* UAITask_Rotation::CreateAIRotationTask(AAIController* InController, AActor* InTargetChequer, UBoardNode* InTargetNode,
	FRotator InTargetRotation, float InAcceptableDeviation)
{
	if (!InController && !InController->GetPawn())  // ��ָ��AI���������ҿ�����ʵ�ʿ����˽�ɫ
		return nullptr;
	UAITask_Rotation* ThisTask = NewAITask<UAITask_Rotation>(*InController, EAITaskPriority::High);
	if (!ThisTask)
		return nullptr;
	ThisTask->AITargetChequer = InTargetChequer;
	ThisTask->AITargetNode = InTargetNode;
	ThisTask->AITargetRotation = InTargetRotation;
	ThisTask->AIAcceptableDeviation = InAcceptableDeviation;
	return ThisTask;
}

// ��ȡ��תĿ������
AActor* UAITask_Rotation::GetAITargetChequer()const
{
	return AITargetChequer ? AITargetChequer : nullptr;
}

// ��ȡ��תĿ����
UBoardNode* UAITask_Rotation::GetAITargetNode()const
{
	return AITargetNode ? AITargetNode : nullptr;
}

// ��ȡAIĿ����ת��
FRotator UAITask_Rotation::GetAITargetRotation()const
{
	return AITargetRotation;
}

// ��ȡAI�ɽ��ܵ���ת���
float UAITask_Rotation::GetAIAcceptableDeviation()const
{
	return AIAcceptableDeviation;
}

// ���񴥷�ʱִ�к���
void UAITask_Rotation::Activate()
{
	Super::Activate();
	ConductChequerRotation();
}

// ��ȡĿ�����ӵĵ�ǰ��Ŀ����
bool UAITask_Rotation::GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const
{
	if (!InChequer)  // ָ���п�
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	UChequerMovementComponent* ChequerMovementComponent = InChequer->FindComponentByClass<UChequerMovementComponent>();
	if (!ChequerMovementComponent)
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	CurrentNodeReceiver = ChequerMovementComponent->GetCurrentNode();
	TargetNodeReceiver = ChequerMovementComponent->GetTargetNode();
	return true;
}

// ����ɹ����ʱִ�еĺ���
void UAITask_Rotation::TaskSucceeded()
{
	if (!OwnerController || !OwnerController->GetPawn())  // ������������δ����Pawn
		return;
	// ��ȡ�����ƶ����
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// ������ɣ����ί��
	if (ChequerMovementComponent && ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.RemoveDynamic(this, &UAITask_Rotation::TaskSucceeded);

	OnTaskSucceeded.Broadcast();
	EndTask();  // �������
}

// ����ʧ��ʱִ�еĺ���
void UAITask_Rotation::TaskFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())  // ������������δ����Pawn
		return;
	// ��ȡ�����ƶ����
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// ����ʧ�ܣ�����ת��ɰ󶨵�ί�в�������ɣ����
	if (ChequerMovementComponent && ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.RemoveDynamic(this, &UAITask_Rotation::TaskSucceeded);

	OnTaskFailed.Broadcast();
	EndTask();  // �������
}

// �������ӽ�����ת
void UAITask_Rotation::ConductChequerRotation()
{
	if (!OwnerController || !OwnerController->GetPawn()) // AIδ�����ƻ�δ����Pawn
	{
		TaskFailed();
		return;
	}
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	if (!ChequerMovementComponent)  // δ�ҵ��ƶ����
	{
		TaskFailed();
		return;
	}
	if (AITargetNode)  // �ṩ��תĿ����
		ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetNode, AIAcceptableDeviation);
	else if (AITargetChequer)  // �ṩ��תĿ������
		ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetChequer, AIAcceptableDeviation);
	else  // �ṩĿ����ת��
			ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetRotation, AIAcceptableDeviation);
	
	// ��ί�У��������ת��ִ�У����״ν��а�
	if (!ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.AddDynamic(this, &UAITask_Rotation::TaskSucceeded);
}

