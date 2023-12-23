// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardNodeFinal.h"
#include "ChessBoard.generated.h"
UCLASS()
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
	UFUNCTION()
	UBoardNodeFinal* GetNode(FGridCoordinates InCoord) const;
	//�Զ�Ѱ·�㷨,StopSteps�������Ŀ��λ�õľ��룻
	UFUNCTION(BlueprintCallable)
	bool FindPath(TArray<UBoardNodeFinal*>& Path, AActor* InActor, UBoardNodeFinal* FromNode, UBoardNodeFinal* ToNode, int StopSteps = 0);
	//Ѱ·-��ȡĿ��㷶Χ�ڵ����
	UFUNCTION()
	TArray<UBoardNodeFinal*> GetNodeNeighbors(UBoardNodeFinal* InNode, int InStep = 0) const;

};
