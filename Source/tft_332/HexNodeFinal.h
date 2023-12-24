// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoardNodeFinal.h"
#include "HexNodeFinal.generated.h"

/**
 * 
 */
UCLASS()
class TFT_332_API UHexNodeFinal : public UBoardNodeFinal
{
	GENERATED_BODY()
public:
	//�޲������캯��
	UHexNodeFinal() {};

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UHexNodeFinal* TopRightNode;//���Ͻ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* RightNode;//�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* BottomRightNode;//�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* TopLeftNode;//���Ͻ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* LeftNode;//�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNodeFinal* BottomLeftNode;//���½����
	//��д��ȡ���������
	virtual TArray<UBoardNodeFinal*>GetNeighbors()override;

	//��д������������ʵ�ʾ��뺯��
	virtual float GetRealRadiusSize()const override
	{
		return Size * FMath::Sqrt(3) * 0.5;
	}
	
	//��д���ƺ���
	virtual void DrawNode(TArray<FVector> &InVertices,//����
		TArray<int32> &InIndecies,//����
		TArray<FVector> &InNormals,//����
		TArray<FVector2D> &InUV,//�������ͼ��ӳ��
		TArray<FColor> &InVertexColors,//������ɫ
		TArray<FVector> &InTangents,//���ߣ�
		FVector InOffset) override;//ƫ��ֵ��

};
