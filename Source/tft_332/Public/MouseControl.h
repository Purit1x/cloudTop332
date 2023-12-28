// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../ChessBoard.h"
#include "MouseControl.generated.h"

/**
 * 
 */
class AChessBoard;
class UBoardNode;
class APawn;

UCLASS()
class TFT_332_API AMouseControl : public APlayerController
{
	GENERATED_BODY()
public:
	virtual	void SetupInputComponent()override;

	//���캯��
	AMouseControl();

	//��ǰ����
	UPROPERTY(BlueprintReadWrite)
	AChessBoard* BattleMap;

	UFUNCTION(BlueprintCallable)
	AChessBoard* GetBattleMap();


	//�����ݵĳ�ʼλ�ã�����Ҫ�յ�λ�ã�
	UPROPERTY(BlueprintReadWrite)
	UBoardNode* StartNode;

	//�����ж��Ƿ��Ѿ���ѡȡ��λ��
	bool AlreadySelected;

	//��ȡ����Ӧλ��
	FVector GetMouseCurrentPosition()const;

	//����¼�-�����ƶ����ӻ��ƶ�ССӢ��
	UFUNCTION(BlueprintCallable)
	void LeftClick();

	//�Ҽ��¼�-���ڳ������ӻ��������ѡ�в���
	UFUNCTION(BlueprintCallable)
	void RightClick();

	//����ѡ�е�����λ��-���ѡ�к��һ�ֿ������
	UFUNCTION(BlueprintCallable)
	bool ExchangeNode(UBoardNode* Start, UBoardNode* End);
};
