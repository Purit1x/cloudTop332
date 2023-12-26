// Fill out your copyright notice in the Description page of Project Settings.


#include "AITask_Rotation.h"

// 创建AI旋转任务
UAITask_Rotation* UAITask_Rotation::CreateAIRotationTask(AAIController* InController, AActor* InTargetChequer, UBoardNode* InTargetNode,
	FRotator InTargetRotation, float InAcceptableDeviation)
{
	if (!InController && !InController->GetPawn())  // 需指定AI控制器，且控制器实际控制了角色
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

// 获取旋转目标弈子
AActor* UAITask_Rotation::GetAITargetChequer()const
{
	return AITargetChequer ? AITargetChequer : nullptr;
}

// 获取旋转目标格点
UBoardNode* UAITask_Rotation::GetAITargetNode()const
{
	return AITargetNode ? AITargetNode : nullptr;
}

// 获取AI目标旋转度
FRotator UAITask_Rotation::GetAITargetRotation()const
{
	return AITargetRotation;
}

// 获取AI可接受的旋转误差
float UAITask_Rotation::GetAIAcceptableDeviation()const
{
	return AIAcceptableDeviation;
}

// 任务触发时执行函数
void UAITask_Rotation::Activate()
{
	Super::Activate();
	ConductChequerRotation();
}

// 获取目标弈子的当前与目标格点
bool UAITask_Rotation::GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const
{
	if (!InChequer)  // 指针判空
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

// 任务成功完成时执行的函数
void UAITask_Rotation::TaskSucceeded()
{
	if (!OwnerController || !OwnerController->GetPawn())  // 任务无所属或未控制Pawn
		return;
	// 获取弈子移动组件
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// 任务完成，解绑委托
	if (ChequerMovementComponent && ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.RemoveDynamic(this, &UAITask_Rotation::TaskSucceeded);

	OnTaskSucceeded.Broadcast();
	EndTask();  // 任务结束
}

// 任务失败时执行的函数
void UAITask_Rotation::TaskFailed()
{
	if (!OwnerController || !OwnerController->GetPawn())  // 任务无所属或未控制Pawn
		return;
	// 获取弈子移动组件
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	// 任务失败，与旋转完成绑定的委托不可能完成，解绑
	if (ChequerMovementComponent && ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.RemoveDynamic(this, &UAITask_Rotation::TaskSucceeded);

	OnTaskFailed.Broadcast();
	EndTask();  // 任务结束
}

// 引导弈子进行旋转
void UAITask_Rotation::ConductChequerRotation()
{
	if (!OwnerController || !OwnerController->GetPawn()) // AI未被控制或未控制Pawn
	{
		TaskFailed();
		return;
	}
	UChequerMovementComponent* ChequerMovementComponent = OwnerController->GetPawn()->FindComponentByClass<UChequerMovementComponent>();
	if (!ChequerMovementComponent)  // 未找到移动组件
	{
		TaskFailed();
		return;
	}
	if (AITargetNode)  // 提供旋转目标结点
		ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetNode, AIAcceptableDeviation);
	else if (AITargetChequer)  // 提供旋转目标弈子
		ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetChequer, AIAcceptableDeviation);
	else  // 提供目标旋转度
			ChequerMovementComponent->RotationTarget = FRotationTarget(AITargetRotation, AIAcceptableDeviation);
	
	// 绑定委托，在完成旋转后执行，仅首次进行绑定
	if (!ChequerMovementComponent->OnRotationComplete.IsAlreadyBound(this, &UAITask_Rotation::TaskSucceeded))
		ChequerMovementComponent->OnRotationComplete.AddDynamic(this, &UAITask_Rotation::TaskSucceeded);
}

