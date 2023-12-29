// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AIController.h"
#include "BoardNode.h"
#include "ChessBoard.h"
#include "ChequerMovementComponent.h"
#include "AITask_Rotation.generated.h"

/**
 * 
 */
UCLASS()
class TFT_332_API UAITask_Rotation : public UAITask
{
	GENERATED_BODY()
	
public:
	//创建AITask对象
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Controller", BlueprintInternalUseOnly="TRUE", DisplayName="AITask_Rotation"))
	static UAITask_Rotation* CreateAIRotationTask(AAIController* InController, AActor* InTargetChequer, UBoardNode* InTargetNode,
		FRotator InTargetRotation, float InAcceptableDeviation);

	// 获取AI目标弈子(如有)
	AActor* GetAITargetChequer()const;

	// 获取AI目标格点（如有）
	UBoardNode* GetAITargetNode()const;

	// 获取AI目标旋转度
	FRotator GetAITargetRotation()const;

	// 获取AI可接受的旋转误差
	float GetAIAcceptableDeviation()const;

protected:
	virtual void Activate()override;

	// 获取目标弈子当前格点与目标节点
	bool GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const;

	// 任务成功完成时执行的函数
	UFUNCTION()
	void TaskSucceeded();

	// 任务失败时执行的函数
	UFUNCTION()
	void TaskFailed();

	// 引导弈子进行旋转
	UFUNCTION(BlueprintCallable)
	void ConductChequerRotation();

	// 目标弈子
	AActor* AITargetChequer;

	// 目标格点
	UBoardNode* AITargetNode;

	// 目标旋转度
	FRotator AITargetRotation;

	// 可接受的旋转偏差
	float AIAcceptableDeviation;

	// 任务成功时执行的委托
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskSucceeded;

	// 任务失败时执行的委托
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskFailed;
};
