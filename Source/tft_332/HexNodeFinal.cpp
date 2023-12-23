// Fill out your copyright notice in the Description page of Project Settings.


#include "HexNodeFinal.h"

//��ȡ���ڵ����
TArray<UBoardNodeFinal*>UHexNodeFinal::GetNeighbors()
{
	TArray< UBoardNodeFinal*> AdjoingNodes;
	if (TopRightNode)
		AdjoingNodes.Add(TopRightNode);
	if(RightNode)
		AdjoingNodes.Add(RightNode);
	if (BottomRightNode)
		AdjoingNodes.Add(BottomRightNode);
	if (LeftNode)
		AdjoingNodes.Add(LeftNode);
	if (TopLeftNode)
		AdjoingNodes.Add(TopLeftNode);
	if (BottomLeftNode)
		AdjoingNodes.Add(BottomLeftNode);
	return AdjoingNodes;
}



