// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_Movement.h"

// 创建AI移动任务
UAITask_Movement* UAITask_Movement::CreateAIMovementTask(AAIController* InAIController, UBoardNode* InTargetNode, AActor* InChequer, int AttackRange)
{
	if (!InAIController)
		return nullptr;
	// 创建AIMoveTask
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


// 开始任务时开始移动
void UAITask_Movement::Activate()
{
	Super::Activate();
	ConductChequerMovement();
}

bool UAITask_Movement::GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const
{
	if (!InChequer)  // 指针判空
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	// 由传入弈子的移动组件获取其当前所在棋格与目标棋格
	UChequerMovementComponent* TargetChequerMovementComponent = InChequer->FindComponentByClass<UChequerMovementComponent>();
	if (!TargetChequerMovementComponent)  // 指针判空
	{
		CurrentNodeReceiver = nullptr;
		TargetNodeReceiver = nullptr;
		return false;
	}
	CurrentNodeReceiver = TargetChequerMovementComponent->GetCurrentNode();
	TargetNodeReceiver = TargetChequerMovementComponent->GetTargetNode();

	return true;
}

// 移动任务成功完成
void UAITask_Movement::TaskSucceeded()
{
	if (!OwnerController || !OwnerController->GetPawn())  // 任务无所属AI或AI未控制Pawn
		return;
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// 任务完成，若有函数与委托绑定，解绑之
	if (ChequerMovementComponent && ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.RemoveDynamic(this, &UAITask_Movement::TaskSucceeded);
	if (ChequerMovementComponent && ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.RemoveDynamic(this, &UAITask_Movement::ConductChequerMovement);

	// Stop moving
	ChequerMovementComponent->StopMovementImmediately();
	// 指示移动任务完成
	OnTaskSucceeded.Broadcast();
	EndTask();
}

// 移动任务失败
void UAITask_Movement::TaskFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())  // 任务无所属AI或AI未控制Pawn
		return;
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// 任务失败，TaskSucceed无法调用，解绑之
	if (ChequerMovementComponent && ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.RemoveDynamic(this, &UAITask_Movement::TaskSucceeded);
	// 任务终止， 解绑无用进入途径点的函数
	if (ChequerMovementComponent && ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.RemoveDynamic(this, &UAITask_Movement::ConductChequerMovement);
	// Stop moving
	ChequerMovementComponent->StopMovementImmediately();
	OnTaskFailed.Broadcast();
	EndTask();
}

// 引导弈子进行移动
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
	AICurrentNode = ChequerMovementComponent->GetCurrentNode();  // 获得AI所属弈子当前所在格点
	if (!AICurrentNode)
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("3"));
		return;
	}
	UBoardNode* TargetChequerCurrentNode = nullptr;  // 占位
	if (AITargetChequer && !AITargetChequer->IsPendingKill())  // 追踪的弈子应处于正常状态，并未等待垃圾回收
	{
		GetTargetChequerNode(AITargetChequer, TargetChequerCurrentNode, AITargetNode);  // 预判追踪弈子的位置
	}
	if (!AITargetNode)
		AITargetNode = TargetChequerCurrentNode;  // 若追踪的弈子暂无目标格点，则直接追踪其当前位置
	if (!AITargetNode)  // 起点或终点有一不存在则无法构建路径
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("4"));
		return;
	}
	// 终点
	if (AITargetNode == AICurrentNode)
	{
		TaskSucceeded();
		return;
	}

	TArray<UBoardNode*> FoundPath;
	bool bSearchSucceeded = AICurrentNode->GridMap->FindPath(FoundPath, OwnerController->GetPawn(), AICurrentNode, AITargetNode, ChequerAttackRange);
	if (!bSearchSucceeded)  // 寻路失败
	{
		TaskFailed();
		//UE_LOG(LogTemp, Warning, TEXT("%d, %d\n"), AICurrentNode->Coordinate.X, AICurrentNode->Coordinate.Y);
		//UE_LOG(LogTemp, Warning, TEXT("%d, %d\n"), AITargetNode->Coordinate.X, AITargetNode->Coordinate.Y);
		return;
	}
	if (FoundPath.Num() == 0)  // 已达终点
	{
		TaskSucceeded();
		return;
	}

	ChequerMovementComponent->SetChequerMovePath(FoundPath, true);  // ChequerMovementComponent使用的是Pop模拟栈，需逆转路径

	//进入中途点时，应当及时更新路径，所以将本函数添加到进入中途点的委托中
	if (!ChequerMovementComponent->OnEnteringInterimNode.IsAlreadyBound(this, &UAITask_Movement::ConductChequerMovement))
		ChequerMovementComponent->OnEnteringInterimNode.AddDynamic(this, &UAITask_Movement::ConductChequerMovement);
	// 为进入终点时绑定委托，仅首次进行绑定，后续会进行跳过
	if (!ChequerMovementComponent->OnMovementComplete.IsAlreadyBound(this, &UAITask_Movement::TaskSucceeded))
		ChequerMovementComponent->OnMovementComplete.AddDynamic(this, &UAITask_Movement::TaskSucceeded);
}