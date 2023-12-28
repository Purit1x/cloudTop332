// Fill out your copyright notice in the Description page of Project Settings.


#include "ChequerMovementComponent.h"

FRotationTarget::FRotationTarget() :bActive(false), RotationTargetChequer(nullptr), RotationTargetNode(nullptr), AcceptableDeviation(0.0f)
{

}

FRotationTarget::FRotationTarget(UBoardNode* InNode, const float InAcceptableDeviation) :bActive(false), RotationTargetChequer(nullptr),
RotationTargetNode(InNode), AcceptableDeviation(InAcceptableDeviation)
{

}

FRotationTarget::FRotationTarget(AActor* InRotationTargetChequer, const float InAcceptableDeviation) :bActive(false),
RotationTargetChequer(InRotationTargetChequer), RotationTargetNode(nullptr), AcceptableDeviation(InAcceptableDeviation)
{

}

FRotationTarget::FRotationTarget(FRotator InRotation, const float InAcceptableDeviation):bActive(false), 
RotationTargetChequer(nullptr), RotationTargetNode(nullptr), AcceptableDeviation(InAcceptableDeviation)
{

}

inline bool FRotationTarget::IsActive() const
{
	return bActive;
}

// 获取组件到目标点的旋转度
FRotator FRotationTarget::GetTargetRotation(const FVector InLocation)
{
	if (RotationTargetNode)
	{
		return FRotationMatrix::MakeFromX(RotationTargetNode->Location - InLocation).Rotator();
	}
	else if (RotationTargetChequer)
	{
		return FRotationMatrix::MakeFromX(RotationTargetChequer->GetActorLocation() - InLocation).Rotator();
	}
	else
	{
		return FRotator::ZeroRotator;
	}
}

// 重置旋转目标
void FRotationTarget::Clear()
{
	bActive = false;
	RotationTargetChequer = nullptr;
	RotationTargetNode = nullptr;
	AcceptableDeviation = 0.0f;
}

// 判断是否应该Tick
bool UChequerMovementComponent::ShouldTickComponent(const float DeltaTime) const
{
	if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return false;
	AActor* UpdatedComponentOwner = UpdatedComponent->GetOwner();
	// 组件拥有者将要被垃圾回收
	if (!UpdatedComponentOwner || UpdatedComponentOwner->IsPendingKill())
		return false;
	// 正在进行物理模拟
	if (UpdatedComponent->IsSimulatingPhysics())
		return false;

	return true;
}

// Called when the game starts
void UChequerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Tick组件时旋转组件
void UChequerMovementComponent::TickRotateComponent(const float DeltaTime)
{
	FRotator RotationOrigin = UpdatedComponent->GetComponentRotation();
	FRotator RotationDestination = RotationTarget.GetTargetRotation(UpdatedComponent->GetComponentLocation());
	// 只需研究航向角
	RotationOrigin.Roll = RotationOrigin.Pitch = 0;
	RotationDestination.Roll = RotationDestination.Pitch = 0;
	// 比较当前航向角之差是否在可接受的范围内
	if (FMath::Abs(RotationDestination.Yaw - RotationOrigin.Yaw) <= FMath::Max(RotationTarget.AcceptableDeviation, 0.1f))
	{
		RotationTarget.Clear();
		// 旋转完成，执行委托
		OnRotationComplete.Broadcast();
	}
	else // 偏差角度过大，需要继续进行旋转
	{
		// 线性插值，平滑旋转
		FRotator NewRotation = FMath::RInterpConstantTo(RotationOrigin, RotationDestination, DeltaTime, MaxRotationSpeed);
		MoveUpdatedComponent(FVector::ZeroVector, NewRotation, false);
	}
}

// Tick组件时移动组件
void UChequerMovementComponent::TickMoveComponent(const float DeltaTime)
{
	UBoardNode* NextPathNode;  // 要去往的下一个格点
	if (ChequerMovePath.Num() == 0)  // 已到终点，直接return
		return;
	NextPathNode = ChequerMovePath.Top();
	if (!NextPathNode)  // 指针判非空
	{
		ChequerMovePath.Pop();
		return;
	}
	// 如果将要去往的格点并非当前所在格点，则将其标记为目标点
	if (NextPathNode != TargetNode)
		SetTargetNode(NextPathNode);

	float PossibleVelocity = FMath::Max(0.0f, MaxMoveVelocity);
	AActor* Chequer = UpdatedComponent->GetOwner();  // 获取弈子指针
	FVector Direction = TargetNode->Location - Chequer->GetActorLocation();  // 获取从弈子指向格点的向量
	float Distance = Direction.Size2D();  // 获取弈子到格点的距离
	Direction.Normalize();  // 标准化向量

	FVector MovementDelta;
	if (Distance < PossibleVelocity * DeltaTime)  // 可以到达下一格点
		MovementDelta = NextPathNode->Location - Chequer->GetActorLocation();
	else  // 无法到达下一格点， 走DeltaTime内能走的最大值
		MovementDelta = Direction * PossibleVelocity * DeltaTime;

	FRotator ComponentRotation = UpdatedComponent->GetComponentRotation();  // 组件当前旋转度
	FRotator RotationCorrespondingToMovement = MovementDelta.Rotation();  // 移动方向对应的旋转度
	// 平滑旋转至移动方向对应的角度
	FRotator NewRotation = FMath::RInterpConstantTo(ComponentRotation, RotationCorrespondingToMovement, DeltaTime, MaxRotationSpeed);
	// 设置本次Tick的移动速度
	Velocity = MovementDelta / DeltaTime;
	// 移动组件
	MoveUpdatedComponent(MovementDelta, NewRotation, false);

	// 移动完成后，对弈子所在棋格进行修正
	CorrectCurrentNode();
	if ((Chequer->GetActorLocation() - NextPathNode->Location).IsNearlyZero())  // 接近于0, 表明弈子进入了格点
	{
		ChequerMovePath.Pop();
		if (ChequerMovePath.Num() == 0)  // 已到终点
		{
			Velocity = FVector::ZeroVector;  // 停止移动
			// 执行移动完成相关的委托
			OnMovementComplete.Broadcast();
		}
		else
			OnEnteringInterimNode.Broadcast();  // 执行进入中途点相关委托
	}
}

// Called every frame
void UChequerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 本次Tick应该被跳过
	if (!ShouldTickComponent(DeltaTime))
		return;
	if (RotationTarget.IsActive())
	{
		TickRotateComponent(DeltaTime);
		return;  // 一帧只进行一种动作
	}
	TickMoveComponent(DeltaTime);
}

// 组件销毁时，进行资源回收
void UChequerMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	AActor* Chequer = GetOwner();
	if (TargetNode)
		TargetNode->CancelReservation(Chequer);  // 取消标记
	if (CurrentNode)
		CurrentNode->LeaveNode(Chequer);
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UChequerMovementComponent::StopMovementImmediately()
{
	if (TargetNode)
		TargetNode->CancelReservation(GetOwner());
	RotationTarget.Clear();
	ChequerMovePath.Empty();
	Super::StopMovementImmediately();
}

// 设置目标点
void UChequerMovementComponent::SetTargetNode(UBoardNode* InNode)
{
	// 取消对原目标点的标记
	if (TargetNode)
		TargetNode->CancelReservation(GetOwner());
	TargetNode = InNode;
	if (TargetNode)
		TargetNode->ReserveNode(GetOwner());
}

// 设置当前棋格
void UChequerMovementComponent::SetCurrentNode(UBoardNode* InNode)
{
	// 逐格移动，进入新格时重置目标棋格
	if (TargetNode)
		SetTargetNode(nullptr);
	// 离开当前棋格
	if (CurrentNode)
		CurrentNode->LeaveNode(GetOwner());
	CurrentNode = InNode;
	// 若新棋格非空，进入之
	if (CurrentNode)
		CurrentNode->EnterNode(GetOwner());
}

// 获取弈子当前结点
UBoardNode* UChequerMovementComponent::GetCurrentNode()const
{
	return CurrentNode;
}

// 获取弈子目标结点
UBoardNode* UChequerMovementComponent::GetTargetNode()const
{
	return TargetNode;
}

// 设置移动路径
void UChequerMovementComponent::SetChequerMovePath(TArray<UBoardNode*> InPath, bool bReverse)
{
	if (bReverse)
		Algo::Reverse(InPath);
	ChequerMovePath = InPath;
	if (ChequerMovePath.Num() > 0)
	{
		// 设置初始目标点位
		SetTargetNode(ChequerMovePath.Top());
		// 执行开始移动的相关委托
		OnBeginMovement.Broadcast();
	}
}

// 设置移动速度
void UChequerMovementComponent::SetMaxMoveVelocity(const float InVelocity)
{
	MaxMoveVelocity = FMath::Max(0.0f, InVelocity);
}

// 设置旋转速度
void UChequerMovementComponent::SetMaxRotationSpeed(const float InSpeed)
{
	MaxRotationSpeed = FMath::Max(0.0f, InSpeed);
}

// 未达终点时，修正弈子当前所在棋格
void UChequerMovementComponent::CorrectCurrentNode()
{
	AActor* Chequer = GetOwner();
	if (!Chequer || Chequer->IsPendingKill())  // 空指针或弈子将要被垃圾回收
		return;
	if (!ChequerMovePath.Num())  // 已达终点
		return;
	UBoardNode* NextPathNode = ChequerMovePath.Top();
	if (!NextPathNode)
	{
		ChequerMovePath.Pop();
		return;
	}
	FVector Distance = NextPathNode->Location - Chequer->GetActorLocation();
	if (Distance.Size2D() < NextPathNode->GetRealRadiusSize())  // 粗略修正
		SetCurrentNode(NextPathNode);
}