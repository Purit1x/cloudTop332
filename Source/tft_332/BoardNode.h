// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNode.generated.h"

class AChessBoard;
class AActor;
/*�������*/
USTRUCT(BlueprintType)
struct  FGridVector
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;

	FGridVector() {};
	FGridVector(int InX, int InY)
	{
		this->X = InX;
		this->Y = InY;
	}

	//���������==������TMap��Key�Ĺ淶
	FORCEINLINE friend bool operator==(const FGridVector& Lhs, const FGridVector& Rhs)
	{
		return (Lhs.X == Rhs.X) && (Lhs.Y == Rhs.Y);
	}

	//���������+����������ӷ�����
	FGridVector operator+ (const FGridVector& F)
	{
		return FGridVector(this->X + F.X, this->Y + F.Y);
	}

};


//GetTypeHash������TMap��Key�Ĺ淶
FORCEINLINE uint32 GetTypeHash(const FGridVector& Key)
{
	return HashCombine(GetTypeHash(Key.X), GetTypeHash(Key.Y));
}

/*�����״*/
UENUM(BlueprintType)
enum class EGridType : uint8
{
	None,
	Hex,
};

/*���ͨ��״̬*/
UENUM(BlueprintType)
enum class ENodePassFlag : uint8
{
	Pass,
	Block,
};


UCLASS()
class TFT_332_API UBoardNode : public UObject
{
	GENERATED_BODY()
public:
	//����-����С
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;

	//����-����ռ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	//����-���̿ռ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridVector Coordinate;

	//����-�����״
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridType NodeType = EGridType::None;

	//����-��������
	UPROPERTY(BlueprintReadWrite)
	AChessBoard* GridMap;

	//����-��ʼ������
	UFUNCTION(BlueprintCallable)
	virtual void InitNode(AChessBoard* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize);

	//����-��ȡ�������
	UFUNCTION(BlueprintCallable)
	virtual TArray<UBoardNode*> GetNeighbors();

	//Ѱ·-ͨ��״̬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	ENodePassFlag PassFlag = ENodePassFlag::Pass;

	//Ѱ·-Ѱ·����
	UBoardNode* PreNode;

	//Ѱ·-�ж�����Ƿ���ͨ��
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool CanPass(AActor* InActor) const;

	//Ѱ·-����еĵ�λ
	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TArray<AActor*> NodeActors;

	//Ѱ·-��ȡ��������ʵ�ʾ���
	UFUNCTION(BlueprintCallable)
	virtual float GetRealRadiusSize() const { return Size; };

	//ģ��-���ģ������
	UPROPERTY()
	int MeshIndex;

	//ģ��-����ģ��
	UFUNCTION()
	virtual void DrawNode(TArray<FVector>& InVertices,//����
		TArray<int32>& InIndecies,//����
		TArray<FVector>& InNormals,//����
		TArray<FVector2D>& InUV,//ӳ��
		TArray<FColor>& InVertexColors,//������ɫ
		TArray<FVector>& InTangents,//
		FVector InOffset) {};//ƫ����



	//���ӽ������
	void EnterNode(AActor* InActor);

	//�����뿪���
	void LeaveNode(AActor* InActor);

	//���ӽ������ΪĿ��㣬��ֹ����֮���λ�ó�ͻ
	void ReserveNode(AActor* InActor);

	//ȡ�����
	void CancelReservation(AActor* InActor);
};