#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardNodeFinal.h"
#include "ChessBoard.generated.h"
class UProceduralMeshComponent;
class UMaterial;
UCLASS(hideCategories = (Materials))
class TFT_332_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoard();
	//棋盘的形状
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EBoardType MapType = EBoardType::None;
	//棋盘大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoardSize;
	//棋盘行数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardRow;
	//棋盘列数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardColumn;
	//棋盘棋格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap< FGridCoordinates, UBoardNodeFinal*> BoardMapNode;





protected:
	//重写BeginPlay函数
	virtual void BeginPlay() override;
	//重写初始化属性函数；
	virtual void PostInitProperties()override;

#if WITH_EDITOR
	//重写在Editor中修改属性函数；
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)override;
#endif
public:	
	//生成棋盘函数,将其余函数进行一个封装；
	UFUNCTION(BlueprintCallable)//BlueprintCallable函数可以在蓝图编辑器中调用
	void GenerateChessBoard();
	//生成六边形棋格；
	UFUNCTION()
	void GenerateHexNodes(float InHexSize, int InRow, int InColumn);
	//初始化六边形棋格
	UFUNCTION()
	void InitHexNode();
	//获取坐标棋格
	UFUNCTION(BlueprintCallable)
	UBoardNodeFinal* GetNode(FGridCoordinates InCoord) const;
	//自动寻路算法,StopSteps代表距离目标位置的距离；
	UFUNCTION(BlueprintCallable)
	bool FindPath(TArray<UBoardNodeFinal*>& Path, AActor* InActor, UBoardNodeFinal* FromNode, UBoardNodeFinal* ToNode, int StopSteps = 0);
	//寻路-获取目标点范围内的棋格
	UFUNCTION()
	TArray<UBoardNodeFinal*> GetNodeNeighbors(UBoardNodeFinal* InNode, int InStep = 0) const;
	//模型-棋盘模型
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	//模型-通行材质
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PassMaterial;

	//模型-阻挡材质
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BlockMaterial;

	//模型-阻挡材质1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial1;

	//模型-阻挡材质2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial2;

	//模型-生成棋盘模型
	UFUNCTION(BlueprintCallable)
	void GenerateMapMesh();

	//模型-设置棋格材质
	UFUNCTION(BlueprintCallable)
	void SetNodeMaterial(UBoardNodeFinal* InNode, UMaterialInterface* InMaterial);

	//模型-重置棋格材质
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterial(UBoardNodeFinal* InNode);

	//模型-重置所有棋格材质
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterialAll();

	//基础-判断是否在棋格内
	UFUNCTION(BlueprintCallable)
	UBoardNodeFinal* CheckHitNode(FVector InPosition);

	//基础-判断是否在六边形棋格内
	UFUNCTION()
	UBoardNodeFinal* CheckHitHexNode(FVector InPosition);




};
