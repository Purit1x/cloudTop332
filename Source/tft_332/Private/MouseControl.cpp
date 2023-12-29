// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseControl.h"
#include "EngineUtils.h"
#include "../ChessBoard.h"
#include "HexNode.generated.h"

#define TOSELL 1

//���캯��
AMouseControl::AMouseControl()
{
	//�������ɼ�
	this->bShowMouseCursor = true;

	//�趨Ϊ��δѡ���ʼλ��
	AlreadySelected = false;

	StartNode = nullptr;

}

AChessBoard*
AMouseControl::GetBattleMap()
{
	if (BattleMap != nullptr)//�Ѿ�����BattleMap
	{
		return BattleMap;
	}
	else//������BattleMap��ʹ�õ�����������ȡBattleMap
	{
		for (TActorIterator<AChessBoard> Itr(GetWorld()); Itr; ++Itr)
		{
			if (!*Itr)
				continue;
			BattleMap = *Itr;
			break;
		}
		return BattleMap;
	}
}

//�󶨰���
void
AMouseControl::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMouseControl::LeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMouseControl::RightClick);
}

//��������Ӧ����
void
AMouseControl::LeftClick()
{
	//�ȱ�֤�����Ѿ�����
	if (BattleMap != nullptr)
	{
		return;
	}

	UBoardNode* tHitNode = BattleMap->CheckHitNode(GetMouseCurrentPosition());
	
	//���û��ѡ�������
	if (StartNode == nullptr)
	{
		//�����Ӧ����Ϊ����û��Ԥ��ѡ��������Ӧ����ССӢ���ƶ�
		if (tHitNode->NodeActors.Num() == 0)
		{

		}
		//�����Ӧ����Ϊ�գ������Ϊ����
		else
		{
			StartNode = tHitNode;
		}
	}
	//����Ѿ�ѡ�����������һ���ǽ���
	else
	{
		ExchangeNode(StartNode, tHitNode);
		StartNode = nullptr;
	}
}

void
AMouseControl::RightClick()
{
	//�����������λ��ת���������ϵ�����
	
	//�����Ӧ���Ϊ�գ���ͨ��AlreadySelected������һ���ж�
	//���AlreadySelectedΪtrue����StartNode��գ���Ӧ����ֵΪ0��
	//���AlreadySelectedΪfalse���򲻽����κβ�������Ӧ����ֵΪ1��
	
	//�����Ӧ���Ϊ�գ����Ӧ�������Ӳ���
	//���ѡ�������Ϊ�̵������򲻽����κβ�����������Ļ�Ͻ����������ѣ�����Ӧ����ֵΪ2��
	//���ѡ�������Ϊ�����������Ƴ��õ�����Ӳ�������ң���Ӧ����ֵΪ3��
}

FVector
AMouseControl::GetMouseCurrentPosition()const
{
	if (!BattleMap)
	{
		return FVector::ZeroVector;
	}
	else
	{
		FVector WorldLocation;//������Ϸ�����е�����
		FVector WorldDirection;//����Ӹõ�����ķ���ʵ������Ӧ���ò�����
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			//��ȡZ���ֵ
			float tScale = BattleMap->GetActorLocation().Z - WorldLocation.Z;
			tScale /= WorldDirection.Z;
			WorldLocation += tScale * WorldDirection;
			return WorldLocation;
		}
		return FVector::ZeroVector;
	}
}

bool 
AMouseControl::ExchangeNode(UBoardNode* Start, UBoardNode* End)
{
	return true;
}
