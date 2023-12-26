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

// ��ȡ�����Ŀ������ת��
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

// ������תĿ��
void FRotationTarget::Clear()
{
	bActive = false;
	RotationTargetChequer = nullptr;
	RotationTargetNode = nullptr;
	AcceptableDeviation = 0.0f;
}

// �ж��Ƿ�Ӧ��Tick
bool UChequerMovementComponent::ShouldTickComponent(const float DeltaTime) const
{
	if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return false;
	AActor* UpdatedComponentOwner = UpdatedComponent->GetOwner();
	// ���ӵ���߽�Ҫ����������
	if (!UpdatedComponentOwner || UpdatedComponentOwner->IsPendingKill())
		return false;
	// ���ڽ�������ģ��
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

// Tick���ʱ��ת���
void UChequerMovementComponent::TickRotateComponent(const float DeltaTime)
{
	FRotator RotationOrigin = UpdatedComponent->GetComponentRotation();
	FRotator RotationDestination = RotationTarget.GetTargetRotation(UpdatedComponent->GetComponentLocation());
	// ֻ���о������
	RotationOrigin.Roll = RotationOrigin.Pitch = 0;
	RotationDestination.Roll = RotationDestination.Pitch = 0;
	// �Ƚϵ�ǰ�����֮���Ƿ��ڿɽ��ܵķ�Χ��
	if (FMath::Abs(RotationDestination.Yaw - RotationOrigin.Yaw) <= FMath::Max(RotationTarget.AcceptableDeviation, 0.1f))
	{
		RotationTarget.Clear();
		// ��ת��ɣ�ִ��ί��
		OnRotationComplete.Broadcast();
	}
	else // ƫ��Ƕȹ�����Ҫ����������ת
	{
		// ���Բ�ֵ��ƽ����ת
		FRotator NewRotation = FMath::RInterpConstantTo(RotationOrigin, RotationDestination, DeltaTime, MaxRotationSpeed);
		MoveUpdatedComponent(FVector::ZeroVector, NewRotation, false);
	}
}

// Tick���ʱ�ƶ����
void UChequerMovementComponent::TickMoveComponent(const float DeltaTime)
{
	UBoardNode* NextPathNode;  // Ҫȥ������һ�����
	if (ChequerMovePath.Num() == 0)  // �ѵ��յ㣬ֱ��return
		return;
	NextPathNode = ChequerMovePath.Top();
	if (!NextPathNode)  // ָ���зǿ�
	{
		ChequerMovePath.Pop();
		return;
	}
	// �����Ҫȥ���ĸ�㲢�ǵ�ǰ���ڸ�㣬������ΪĿ���
	if (NextPathNode != TargetNode)
		SetTargetNode(NextPathNode);

	float PossibleVelocity = FMath::Max(0.0f, MaxMoveVelocity);
	AActor* Chequer = UpdatedComponent->GetOwner();  // ��ȡ����ָ��
	FVector Direction = TargetNode->Location - Chequer->GetActorLocation();  // ��ȡ������ָ���������
	float Distance = Direction.Size2D();  // ��ȡ���ӵ����ľ���
	Direction.Normalize();  // ��׼������

	FVector MovementDelta;
	if (Distance < PossibleVelocity * DeltaTime)  // ���Ե�����һ���
		MovementDelta = NextPathNode->Location - Chequer->GetActorLocation();
	else  // �޷�������һ��㣬 ��DeltaTime�����ߵ����ֵ
		MovementDelta = Direction * PossibleVelocity * DeltaTime;

	FRotator ComponentRotation = UpdatedComponent->GetComponentRotation();  // �����ǰ��ת��
	FRotator RotationCorrespondingToMovement = MovementDelta.Rotation();  // �ƶ������Ӧ����ת��
	// ƽ����ת���ƶ������Ӧ�ĽǶ�
	FRotator NewRotation = FMath::RInterpConstantTo(ComponentRotation, RotationCorrespondingToMovement, DeltaTime, MaxRotationSpeed);
	// ���ñ���Tick���ƶ��ٶ�
	Velocity = MovementDelta / DeltaTime;
	// �ƶ����
	MoveUpdatedComponent(MovementDelta, NewRotation, false);

	// �ƶ���ɺ󣬶�������������������
	CorrectCurrentNode();
	if ((Chequer->GetActorLocation() - NextPathNode->Location).IsNearlyZero())  // �ӽ���0, �������ӽ����˸��
	{
		ChequerMovePath.Pop();
		if (ChequerMovePath.Num() == 0)  // �ѵ��յ�
		{
			Velocity = FVector::ZeroVector;  // ֹͣ�ƶ�
			// ִ���ƶ������ص�ί��
			OnMovementComplete.Broadcast();
		}
		else
			OnEnteringInterimNode.Broadcast();  // ִ�н�����;�����ί��
	}
}

// Called every frame
void UChequerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ����TickӦ�ñ�����
	if (!ShouldTickComponent(DeltaTime))
		return;
	if (RotationTarget.IsActive())
	{
		TickRotateComponent(DeltaTime);
		return;  // һֻ֡����һ�ֶ���
	}
	TickMoveComponent(DeltaTime);
}

// �������ʱ��������Դ����
void UChequerMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	AActor* Chequer = GetOwner();
	if (TargetNode)
		TargetNode->CancelReservation(Chequer);  // ȡ�����
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

// ����Ŀ���
void UChequerMovementComponent::SetTargetNode(UBoardNode* InNode)
{
	// ȡ����ԭĿ���ı��
	if (TargetNode)
		TargetNode->CancelReservation(GetOwner());
	TargetNode = InNode;
	if (TargetNode)
		TargetNode->ReserveNode(GetOwner());
}

// ���õ�ǰ���
void UChequerMovementComponent::SetCurrentNode(UBoardNode* InNode)
{
	// ����ƶ��������¸�ʱ����Ŀ�����
	if (TargetNode)
		SetTargetNode(nullptr);
	// �뿪��ǰ���
	if (CurrentNode)
		CurrentNode->LeaveNode(GetOwner());
	CurrentNode = InNode;
	// �������ǿգ�����֮
	if (CurrentNode)
		CurrentNode->EnterNode(GetOwner());
}

// ��ȡ���ӵ�ǰ���
UBoardNode* UChequerMovementComponent::GetCurrentNode()const
{
	return CurrentNode;
}

// ��ȡ����Ŀ����
UBoardNode* UChequerMovementComponent::GetTargetNode()const
{
	return TargetNode;
}

// �����ƶ�·��
void UChequerMovementComponent::SetChequerMovePath(TArray<UBoardNode*> InPath, bool bReverse)
{
	if (bReverse)
		Algo::Reverse(InPath);
	ChequerMovePath = InPath;
	if (ChequerMovePath.Num() > 0)
	{
		// ���ó�ʼĿ���λ
		SetTargetNode(ChequerMovePath.Top());
		// ִ�п�ʼ�ƶ������ί��
		OnBeginMovement.Broadcast();
	}
}

// �����ƶ��ٶ�
void UChequerMovementComponent::SetMaxMoveVelocity(const float InVelocity)
{
	MaxMoveVelocity = FMath::Max(0.0f, InVelocity);
}

// ������ת�ٶ�
void UChequerMovementComponent::SetMaxRotationSpeed(const float InSpeed)
{
	MaxRotationSpeed = FMath::Max(0.0f, InSpeed);
}

// δ���յ�ʱ���������ӵ�ǰ�������
void UChequerMovementComponent::CorrectCurrentNode()
{
	AActor* Chequer = GetOwner();
	if (!Chequer || Chequer->IsPendingKill())  // ��ָ������ӽ�Ҫ����������
		return;
	if (!ChequerMovePath.Num())  // �Ѵ��յ�
		return;
	UBoardNode* NextPathNode = ChequerMovePath.Top();
	if (!NextPathNode)
	{
		ChequerMovePath.Pop();
		return;
	}
	FVector Distance = NextPathNode->Location - Chequer->GetActorLocation();
	if (Distance.Size2D() < NextPathNode->GetRealRadiusSize())  // ��������
		SetCurrentNode(NextPathNode);
}