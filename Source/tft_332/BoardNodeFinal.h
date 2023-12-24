#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNodeFinal.generated.h"
class AChessBoard;
class AActor;

//���̸��ӣ�
USTRUCT(BlueprintType)
struct FGridCoordinates
{
public:

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//BlueprintReadWrite�����������������ͼ�мȱ���ȡҲ���޸�
	int X = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;
	FGridCoordinates() {}
	FGridCoordinates(int InX,int InY) 
	{
		this->X = InX;
		this->Y = InY;

	}
	//���صȺ���������ж��ڲ���һ������ڲ���
	FORCEINLINE friend bool operator==(const FGridCoordinates& LeftObject, const FGridCoordinates& RightObject)
	{
		return (LeftObject.X == RightObject.X) && (LeftObject.Y == RightObject.Y);
	}

	//���ؼӺ���������任����λ�ã�
	FGridCoordinates operator+(const FGridCoordinates& Lhs)
	{
		return FGridCoordinates(this->X + Lhs.X, this->Y + Lhs.Y);
	}



};

//�õ����̶��㣻
FORCEINLINE uint32 GetTypeHash(const FGridCoordinates& Key)
{
	return HashCombine(GetTypeHash(Key.X), GetTypeHash(Key.Y));
}

//�涨������״��
UENUM(BlueprintType)
enum class EBoardType :uint8
{
	None,
	Hex,//�����Σ�

};
//������Ƿ�����ϰ��
UENUM(BlueprintType)
enum class EBoardPassFlag :uint8
{
	Pass,//���ϰ���
	Block,//���ϰ��
};

UCLASS()
class UBoardNodeFinal :public UObject
{
	GENERATED_BODY()
public:
	//���Ĵ�С
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Size;
	//����ռ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	//���̿ռ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridCoordinates Coordinate;
	//�����״��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoardType BoardType = EBoardType::Hex;
	//����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AChessBoard* ChessBoard;
	//��ʼ��������
	UFUNCTION(BlueprintCallable)
	virtual void InitNode(AChessBoard* InChessBoard,FVector InLocation, FGridCoordinates InCoordinate,float InSize);

	//��ȡ���ڵ����
	UFUNCTION(BlueprintCallable)
	virtual TArray<UBoardNodeFinal*>GetNeighbors();

	//�Ƿ����ϰ����������Ѱ·�㷨��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoardPassFlag PassFlag = EBoardPassFlag::Pass;

	//Ѱ·���棨���ڻ���ʱ�ع�·����
	UBoardNodeFinal* PreNode;
	//�ж�����Ƿ���ͨ��
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool CanPass(AActor* InActor) const;
	//����еĵ�λ������Ѱ·�㷨��
	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TArray<AActor*> NodeActors;
	

	//Ѱ·-��ȡ��������ʵ�ʾ���
	UFUNCTION(BlueprintCallable)
	virtual float GetRealRadiusSize() const 
	{ 
		return Size;
	}

	//ģ��-���ģ������
	UPROPERTY()
	int MeshIndex;

	//ģ��-����ģ��
	UFUNCTION()
	virtual void DrawNode(TArray<FVector>& InVertices,
		TArray<int32>& InIndecies,
		TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV,
		TArray<FColor>& InVertexColors,
		TArray<FVector>& InTangents,
		FVector InOffset) {};

	//���ӽ������
	void EnterNode(AActor *InActor);

	//�����뿪���
	void LeaveNode(AActor *InActor);

	//���ӽ������ΪĿ��㣬��ֹ����֮���λ�ó�ͻ
	void ReserveNode(AActor *InActor);

	//ȡ�����
	void CancelReservation(AActor *InActor);
};

