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
	// ����һ��AITask���󣬴����������Ŀ�����Ŀ�����������ӵĹ�����Χ�����ڿ�������ֹͣ��ʱ�̣�
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Controller", BlueprintInternalUseOnly = "TRUE", DisplayName="AITask_Movement"))
	static UAITask_Movement* CreateAIMovementTask(AAIController *InAIController, UBoardNode* InTargetNode, AActor *InChequer, int AttackRange = 1);

	// ��ȡAIĿ�����ӣ����У�
	AActor* GetAITargetChequer() const;

	// ��ȡAIĿ���㣨���У�
	UBoardNode* GetAITargetNode() const;

	// ��ȡAI�������ӹ�����Χ
	int GetAttackRange() const;

	// ���ڲ��Խ�����;���ί���Ƿ����������ĺ���
	//UFUNCTION()
	//void DebugEnterInterimNodeDelegate();

protected:
	// ��������ʱִ�У�����ʼ�ƶ�
	virtual void Activate() override;

	// ��ȡĿ�����ӵ�ǰ�����Ŀ��ڵ�
	bool GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const;

	// ����ɹ����ʱִ�еĺ���
	UFUNCTION()
	void TaskSucceeded();

	// ����ʧ��ʱִ�еĺ���
	UFUNCTION()
	void TaskFailed();

	// �����ƶ�
	UFUNCTION(BlueprintCallable)
	void ConductChequerMovement();

	// Ŀ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor *AITargetChequer;

	// Ŀ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNode *AITargetNode;

	// ��ǰ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNode *AICurrentNode;

	// ���ӹ�����Χ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChequerAttackRange;

	// ����ɹ�ʱִ�е�ί��
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskSucceeded;

	// ����ʧ��ʱִ�е�ί��
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskFailed;

	/*UPROPERTY()
	int EnterInterimDelegateExecNum = 0;

	UPROPERTY()
	int ConductMovementExecNum = 0;*/
};
