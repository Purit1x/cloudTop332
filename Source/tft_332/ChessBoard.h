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
	//���̵���״
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EBoardType MapType = EBoardType::None;
	//���̴�С
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoardSize;
	//��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardRow;
	//��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BoardColumn;
	//�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap< FGridCoordinates, UBoardNodeFinal*> BoardMapNode;





protected:
	//��дBeginPlay����
	virtual void BeginPlay() override;
	//��д��ʼ�����Ժ�����
	virtual void PostInitProperties()override;

#if WITH_EDITOR
	//��д��Editor���޸����Ժ�����
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)override;
#endif
public:	
	//�������̺���,�����ຯ������һ����װ��
	UFUNCTION(BlueprintCallable)//BlueprintCallable������������ͼ�༭���е���
	void GenerateChessBoard();
	//�������������
	UFUNCTION()
	void GenerateHexNodes(float InHexSize, int InRow, int InColumn);
	//��ʼ�����������
	UFUNCTION()
	void InitHexNode();
	//��ȡ�������
	UFUNCTION(BlueprintCallable)
	UBoardNodeFinal* GetNode(FGridCoordinates InCoord) const;
	//�Զ�Ѱ·�㷨,StopSteps�������Ŀ��λ�õľ��룻
	UFUNCTION(BlueprintCallable)
	bool FindPath(TArray<UBoardNodeFinal*>& Path, AActor* InActor, UBoardNodeFinal* FromNode, UBoardNodeFinal* ToNode, int StopSteps = 0);
	//Ѱ·-��ȡĿ��㷶Χ�ڵ����
	UFUNCTION()
	TArray<UBoardNodeFinal*> GetNodeNeighbors(UBoardNodeFinal* InNode, int InStep = 0) const;
	//ģ��-����ģ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	//ģ��-ͨ�в���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PassMaterial;

	//ģ��-�赲����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BlockMaterial;

	//ģ��-�赲����1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial1;

	//ģ��-�赲����2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DebugMaterial2;

	//ģ��-��������ģ��
	UFUNCTION(BlueprintCallable)
	void GenerateMapMesh();

	//ģ��-����������
	UFUNCTION(BlueprintCallable)
	void SetNodeMaterial(UBoardNodeFinal* InNode, UMaterialInterface* InMaterial);

	//ģ��-����������
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterial(UBoardNodeFinal* InNode);

	//ģ��-��������������
	UFUNCTION(BlueprintCallable)
	void ResetNodeMaterialAll();

	//����-�ж��Ƿ��������
	UFUNCTION(BlueprintCallable)
	UBoardNodeFinal* CheckHitNode(FVector InPosition);

	//����-�ж��Ƿ��������������
	UFUNCTION()
	UBoardNodeFinal* CheckHitHexNode(FVector InPosition);




};
