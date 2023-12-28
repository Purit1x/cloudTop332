// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_Movement.h"

// ����AI�ƶ�����
UAITask_Movement* UAITask_Movement::CreateAIMovementTask(AAIController* InAIController, UBoardNode* InTargetNode, AActor* InChequer, int AttackRange)
{
	if (!InAIController)
		return nullptr;
	// ����AIMoveTask
	UAITask_Movement* ThisTask = UAITask::NewAITask<UAITask_Movement>(*InAIController, EAITaskPriority::High);
	ThisTask->AITargetChequer = InChequer;
	ThisTask->AITargetNode = InTargetNode;
	ThisTask->ChequerAttackRange = AttackRange;
	return ThisTask;
}

AActor* UAITask_Movement::GetAITargetChequer()const
{
	if (AITargetChequer)
		return AITargetChequer;
	else
		return nullptr;
}

UBoardNode* UAITask_Movement::GetAITargetNode()const
{
	return AITargetNode ? AITargetNode : nullptr;
}

int UAITask_Movement::GetAttackRange()const
{
	return ChequerAttackRange;
}


// ��ʼ����ʱ��ʼ�ƶ�
void UAITask_Movement::Activate()
{
	Super::Activate();
	ConductChequerMovement();
}

bool UAITask_Movement::GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const
{
	if (!InChequer)  // ָ���п�
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	// �ɴ������ӵ��ƶ������ȡ�䵱ǰ���������Ŀ�����
	UChequerMovementComponent* TargetChequerMovementComponent = InChequer->FindComponentByClass<UChequerMovementComponent>();
	if (!TargetChequerMovementComponent)  // ָ���п�
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	CurrentNodeReceiver = TargetChequerMovementComponent->GetCurrentNode();
	TargetNodeReceiver = TargetChequerMovementComponent->GetTargetNode();

	return true;
}

// �ƶ�����ɹ����
void UAITask_Movement::TaskSucceeded()
{
	if (!OwnerController || !OwnerController->GetPawn())  // ����������AI��AIδ����Pawn
		return;
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// ������ɣ����к�����ί�а󶨣����֮
	if (ChequerMovementComponent && ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.RemoveDynamic(this, &UAITask_Movement::TaskSucceeded);
	if (ChequerMovementComponent && ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.RemoveDynamic(this, &UAITask_Movement::ConductChequerMovement);

	// Stop moving
	ChequerMovementComponent->StopMovementImmediately();
	// ָʾ�ƶ��������
	OnTaskSucceeded.Broadcast();
	EndTask();
}

// �ƶ�����ʧ��
void UAITask_Movement::TaskFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())  // ����������AI��AIδ����Pawn
		return;
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// ����ʧ�ܣ�TaskSucceed�޷����ã����֮
	if (ChequerMovementComponent && ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.RemoveDynamic(this, &UAITask_Movement::TaskSucceeded);
	// ������ֹ�� ������ý���;����ĺ���
	if (ChequerMovementComponent && ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.RemoveDynamic(this, &UAITask_Movement::ConductChequerMovement);
	// Stop moving
	ChequerMovementComponent->StopMovementImmediately();
	OnTaskFailed.Broadcast();
	EndTask();
}

// �������ӽ����ƶ�
void UAITask_Movement::ConductChequerMovement()
{
	if (!OwnerController || !OwnerController->GetPawn())
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("1"));
		return;
	}
	// Get Chequer Movement Component
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	if (!ChequerMovementComponent)
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("2"));
		return;
	}
	AICurrentNode = ChequerMovementComponent->GetCurrentNode();  // ���AI�������ӵ�ǰ���ڸ��
	if (!AICurrentNode)
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("3"));
		return;
	}
	UBoardNode* TargetChequerCurrentNode = nullptr;  // ռλ
	if (AITargetChequer && !AITargetChequer->IsPendingKill())  // ׷�ٵ�����Ӧ��������״̬����δ�ȴ���������
	{
		GetTargetChequerNode(AITargetChequer, TargetChequerCurrentNode, AITargetNode);  // Ԥ��׷�����ӵ�λ��
	}
	if (!AITargetNode)
		AITargetNode = TargetChequerCurrentNode;  // ��׷�ٵ���������Ŀ���㣬��ֱ��׷���䵱ǰλ��
	if (!AITargetNode)  // �����յ���һ���������޷�����·��
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("4"));
		return;
	}
	// �յ�
	if (AITargetNode == AICurrentNode)
	{
		TaskSucceeded();
		return;
	}

	TArray<UBoardNode*> FoundPath;
	bool bSearchSucceeded = AICurrentNode->GridMap->FindPath(FoundPath, OwnerController->GetPawn(), AICurrentNode, AITargetNode, ChequerAttackRange);
	if (!bSearchSucceeded)  // Ѱ·ʧ��
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("%d, %d\n"), AICurrentNode->Coordinate.X, AICurrentNode->Coordinate.Y);
		//UE_LOG(LogTemp, Warning, TEXT("%d, %d\n"), AITargetNode->Coordinate.X, AITargetNode->Coordinate.Y);
		return;
	}
	if (FoundPath.Num() == 0)  // �Ѵ��յ�
	{
		TaskSucceeded();
		return;
	}

	ChequerMovementComponent->SetChequerMovePath(FoundPath, true);  // ChequerMovementComponentʹ�õ���Popģ��ջ������ת·��

	//������;��ʱ��Ӧ����ʱ����·�������Խ���������ӵ�������;���ί����
	if (!ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.AddDynamic(this, &UAITask_Movement::ConductChequerMovement);
	// Ϊ�����յ�ʱ��ί�У����״ν��а󶨣��������������
	if (!ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.AddDynamic(this, &UAITask_Movement::TaskSucceeded);
}