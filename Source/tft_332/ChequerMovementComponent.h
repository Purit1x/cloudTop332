// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "HexNodeFinal.h"
#include "ChessBoard.h"
#include "ChequerMovementComponent.generated.h"

// ���ӽ�Ҫ��ת����Ķ���
USTRUCT(BlueprintType)
struct FRotationTarget {

	GENERATED_USTRUCT_BODY()

	FRotationTarget();
	FRotationTarget(UBoardNodeFinal *InNode, const float InAcceptableDeviation);
	FRotationTarget(AActor *InRotationTargetChequer, const float InAcceptableDeviation);

	// ����������ṩ��ת�Ƿ���Ч
	inline bool IsActive() const;

	// ��ת�Ƿ񼤻�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

	// Ѱ·��������ת
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNodeFinal* RotationTargetNode;

	// ս������������ת
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* RotationTargetChequer;

	// ��ת���������̣�Ӧ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableDeviation = 0.0f;

	// ���ڻ�ȡָ��λ��������ƶ����(����������������)����ת��
	FRotator GetTargetRotation(const FVector InLocation);

	// �����תĿ��
	void Clear();


};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveDelegate);

UCLASS()
class TFT_332_API UChequerMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:	
	// �ж��Ƿ�Ӧ��Tick
	bool ShouldTickComponent(const float DeltaTime) const;

	// Tick���ʱ��ת���
	void TickRotateComponent(const float DeltaTime);

	// Tick���ʱ�ƶ����
	void TickMoveComponent(const float DeltaTime);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// �������
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	// ��ֹ�ƶ�
	void StopMovementImmediately() override;

	// ����Ŀ���
	UFUNCTION(BlueprintCallable)
	void SetTargetNode(UBoardNodeFinal *InNode);

	// ���õ�ǰ���
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UBoardNodeFinal *InNode);


	// ��������·��
	UFUNCTION(BlueprintCallable)
	void SetChequerMovePath(TArray<UBoardNodeFinal *> InPath, bool bReverse);

	// ���������ƶ��ٶ�
	UFUNCTION(BlueprintCallable)
	void SetMaxMoveVelocity(const float InVelocity);

	// ����������ת�ٶ�
	UFUNCTION(BlueprintCallable)
	void SetMaxRotationSpeed(const float InSpeed);

	// ��ȡ���ӵ�ǰ�������
	UFUNCTION(BlueprintCallable)
	void CorrectCurrentNode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// ������תĿ��
	FRotationTarget RotationTarget;

	// ���ӵ�ǰ�������
	UPROPERTY(BlueprintReadOnly)
	UBoardNodeFinal* CurrentNode;

	// ����Ŀ�����
	UPROPERTY(BlueprintReadOnly)
	UBoardNodeFinal* TargetNode;

	// �����ƶ�·��
	UPROPERTY(BlueprintReadWrite)
	TArray<UBoardNodeFinal*> ChequerMovePath;

	// ������ת�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRotationSpeed = 360.0f;

	// �����ƶ��ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveVelocity = 400.0f;

	// ת�����ʱִ��һϵ������
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnRotationComplete;

	// ��ʼ�ƶ�ʱִ������
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnBeginMovement;

	// �����м��ʱִ������
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnEnteringInterimNode;

	// ����ƶ�ʱִ������
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMovementComplete;
};
