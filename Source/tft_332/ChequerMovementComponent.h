// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "HexNode.h"
#include "ChessBoard.h"
#include "ChequerMovementComponent.generated.h"

// 弈子将要旋转面向的对象
USTRUCT(BlueprintType)
struct FRotationTarget {

	GENERATED_USTRUCT_BODY()

	FRotationTarget();
	FRotationTarget(UBoardNode *InNode, const float InAcceptableDeviation);
	FRotationTarget(AActor *InRotationTargetChequer, const float InAcceptableDeviation);

	// 用于向外界提供旋转是否有效
	inline bool IsActive() const;

	// 旋转是否激活
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

	// 寻路面向结点旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoardNode* RotationTargetNode;

	// 战斗面向弈子旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* RotationTargetChequer;

	// 旋转是连续过程，应允许误差
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableDeviation = 0.0f;

	// 用于获取指定位置相对于移动组件(即传入参数所属组件)的旋转度
	FRotator GetTargetRotation(const FVector InLocation);

	// 清空旋转目标
	void Clear();


};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveDelegate);

UCLASS(meta=(BlueprintSpawnableComponent), BlueprintType)
class TFT_332_API UChequerMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:	
	// 判断是否应该Tick
	bool ShouldTickComponent(const float DeltaTime) const;

	// Tick组件时旋转组件
	void TickRotateComponent(const float DeltaTime);

	// Tick组件时移动组件
	void TickMoveComponent(const float DeltaTime);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// 组件销毁
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	// 终止移动
	void StopMovementImmediately() override;

	// 设置目标点
	UFUNCTION(BlueprintCallable)
	void SetTargetNode(UBoardNode *InNode);

	// 设置当前棋格
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UBoardNode *InNode);


	// 设置弈子路径
	UFUNCTION(BlueprintCallable)
	void SetChequerMovePath(TArray<UBoardNode *> InPath, bool bReverse);

	// 设置弈子移动速度
	UFUNCTION(BlueprintCallable)
	void SetMaxMoveVelocity(const float InVelocity);

	// 设置弈子旋转速度
	UFUNCTION(BlueprintCallable)
	void SetMaxRotationSpeed(const float InSpeed);

	// 获取弈子当前所在棋格
	UFUNCTION(BlueprintCallable)
	void CorrectCurrentNode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 弈子旋转目标
	FRotationTarget RotationTarget;

	// 弈子当前所处棋格
	UPROPERTY(BlueprintReadOnly)
	UBoardNode* CurrentNode;

	// 弈子目标棋格
	UPROPERTY(BlueprintReadOnly)
	UBoardNode* TargetNode;

	// 弈子移动路径
	UPROPERTY(BlueprintReadWrite)
	TArray<UBoardNode*> ChequerMovePath;

	// 弈子旋转速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRotationSpeed = 360.0f;

	// 弈子移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveVelocity = 400.0f;

	// 转向完成时执行一系列任务
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnRotationComplete;

	// 开始移动时执行任务
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnBeginMovement;

	// 进入中间点时执行任务
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnEnteringInterimNode;

	// 完成移动时执行任务
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMovementComplete;
};