// Fill out your copyright notice in the Description page of Project Settings.

#include "PathfindingNode.h"

PathfindingNode::PathfindingNode(int GridXPos, int GridYPos, int GridZPos)
{

	

	
	GridX = GridXPos;
	GridY = GridYPos;
	GridZ = GridZPos;

}

PathfindingNode::~PathfindingNode()
{
}

float PathfindingNode::FCost()
{
	return HCost + GCost;
}
