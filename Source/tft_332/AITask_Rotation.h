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
	//����AITask����
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Controller", BlueprintInternalUseOnly="TRUE", DisplayName="AITask_Rotation"))
	static UAITask_Rotation* CreateAIRotationTask(AAIController* InController, AActor* InTargetChequer, UBoardNode* InTargetNode,
		FRotator InTargetRotation, float InAcceptableDeviation);

	// ��ȡAIĿ������(����)
	AActor* GetAITargetChequer()const;

	// ��ȡAIĿ���㣨���У�
	UBoardNode* GetAITargetNode()const;

	// ��ȡAIĿ����ת��
	FRotator GetAITargetRotation()const;

	// ��ȡAI�ɽ��ܵ���ת���
	float GetAIAcceptableDeviation()const;

protected:
	virtual void Activate()override;

	// ��ȡĿ�����ӵ�ǰ�����Ŀ��ڵ�
	bool GetTargetChequerNode(AActor* InChequer, UBoardNode*& CurrentNodeReceiver, UBoardNode*& TargetNodeReceiver)const;

	// ����ɹ����ʱִ�еĺ���
	UFUNCTION()
	void TaskSucceeded();

	// ����ʧ��ʱִ�еĺ���
	UFUNCTION()
	void TaskFailed();

	// �������ӽ�����ת
	UFUNCTION(BlueprintCallable)
	void ConductChequerRotation();

	// Ŀ������
	AActor* AITargetChequer;

	// Ŀ����
	UBoardNode* AITargetNode;

	// Ŀ����ת��
	FRotator AITargetRotation;

	// �ɽ��ܵ���תƫ��
	float AIAcceptableDeviation;

	// ����ɹ�ʱִ�е�ί��
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskSucceeded;

	// ����ʧ��ʱִ�е�ί��
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTaskFailed;
};
