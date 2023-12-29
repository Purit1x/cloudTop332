// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "BoardNode.h"
#include "ChessBoard.h"
#include "AIController.h"
#include "ChequerMovementComponent.h"
#include "AITask_Movement.generated.h"

/**
 * 
 */
UCLASS()
class TFT_332_API UAITask_Movement : public UAITask
{
	GENERATED_BODY()

public:
	// 创建一个AITask对象，传入控制器、目标棋格、目标弈子与弈子的攻击范围（用于控制弈子停止的时刻）
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Controller", BlueprintInternalUseOnly = "TRUE", DisplayName="AITask_Movement"))
	static UAITask_Movement* CreateAIMovementTask(AAIController *InAIController, UBoardNode* InTargetNode, AActor *InChequer, int AttackRange = 1);

	// 获取AI目标弈子（如有）
	AActor* GetAITargetChequer() const;

	// 获取AI目标格点（如有）
	UBoardNode* GetAITargetNode() const;

	// 获取AI所属弈子攻击范围
	int GetAttackRange() const;

	// 用于测试进入中途点的委托是否正常触发的函数
	//UFUNCTION()
	//void DebugEnterInterimNodeDelegate();

protected:
	// 触发任务时执行，即开始移动
	virtual void Activate() override;

	// 获取目标弈子当前格点与目标节点
	bool GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const;

	// 任务成功完成时执行的函数
	UFUNCTION()
	void TaskSucceeded();

	// 任务失败时执行的函数
	UFUNCTION()
	void TaskFailed();

	// 进行移动
	UFUNCTION(BlueprintCallable)
	void ConductChequerMovement();

	// 目标弈子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor *AITargetChequer;

	// 目标棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNode *AITargetNode;

	// 当前棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNode *AICurrentNode;

	// 弈子攻击范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChequerAttackRange;

	// 任务成功时执行的委托
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskSucceeded;

	// 任务失败时执行的委托
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskFailed;

	/*UPROPERTY()
	int EnterInterimDelegateExecNum = 0;

	UPROPERTY()
	int ConductMovementExecNum = 0;*/
};
