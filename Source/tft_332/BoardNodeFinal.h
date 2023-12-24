#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoardNodeFinal.generated.h"
class AChessBoard;
class AActor;

//棋盘格子；
USTRUCT(BlueprintType)
struct FGridCoordinates
{
public:

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//BlueprintReadWrite这个标记允许变量在蓝图中既被读取也被修改
	int X = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;
	FGridCoordinates() {}
	FGridCoordinates(int InX,int InY) 
	{
		this->X = InX;
		this->Y = InY;

	}
	//重载等号运算符，判断在不在一个棋格内部；
	FORCEINLINE friend bool operator==(const FGridCoordinates& LeftObject, const FGridCoordinates& RightObject)
	{
		return (LeftObject.X == RightObject.X) && (LeftObject.Y == RightObject.Y);
	}

	//重载加号运算符，变换棋格的位置；
	FGridCoordinates operator+(const FGridCoordinates& Lhs)
	{
		return FGridCoordinates(this->X + Lhs.X, this->Y + Lhs.Y);
	}



};

//得到棋盘顶点；
FORCEINLINE uint32 GetTypeHash(const FGridCoordinates& Key)
{
	return HashCombine(GetTypeHash(Key.X), GetTypeHash(Key.Y));
}

//规定棋格的形状；
UENUM(BlueprintType)
enum class EBoardType :uint8
{
	None,
	Hex,//六边形；

};
//棋格上是否具有障碍物；
UENUM(BlueprintType)
enum class EBoardPassFlag :uint8
{
	Pass,//无障碍物
	Block,//有障碍物；
};

UCLASS()
class UBoardNodeFinal :public UObject
{
	GENERATED_BODY()
public:
	//棋格的大小
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Size;
	//世界空间坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	//棋盘空间坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridCoordinates Coordinate;
	//棋格形状；
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoardType BoardType = EBoardType::Hex;
	//所属的棋盘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AChessBoard* ChessBoard;
	//初始化函数；
	UFUNCTION(BlueprintCallable)
	virtual void InitNode(AChessBoard* InChessBoard,FVector InLocation, FGridCoordinates InCoordinate,float InSize);

	//获取相邻的棋格；
	UFUNCTION(BlueprintCallable)
	virtual TArray<UBoardNodeFinal*>GetNeighbors();

	//是否有障碍物（用来进行寻路算法）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoardPassFlag PassFlag = EBoardPassFlag::Pass;

	//寻路缓存（用于回溯时重构路径）
	UBoardNodeFinal* PreNode;
	//判断棋格是否能通行
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual bool CanPass(AActor* InActor) const;
	//棋格中的单位（用于寻路算法）
	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TArray<AActor*> NodeActors;
	

	//寻路-获取两两棋格的实际距离
	UFUNCTION(BlueprintCallable)
	virtual float GetRealRadiusSize() const 
	{ 
		return Size;
	}

	//模型-棋格模型索引
	UPROPERTY()
	int MeshIndex;

	//模型-绘制模型
	UFUNCTION()
	virtual void DrawNode(TArray<FVector>& InVertices,
		TArray<int32>& InIndecies,
		TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV,
		TArray<FColor>& InVertexColors,
		TArray<FVector>& InTangents,
		FVector InOffset) {};

	//弈子进入棋格
	void EnterNode(AActor *InActor);

	//弈子离开棋格
	void LeaveNode(AActor *InActor);

	//弈子将棋格设为目标点，防止弈子之间的位置冲突
	void ReserveNode(AActor *InActor);

	//取消标记
	void CancelReservation(AActor *InActor);
};

