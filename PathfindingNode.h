// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RACINGGAME_API PathfindingNode
{
public:
	PathfindingNode(int GridXPos, int GridYPos, int GridZPos);
	~PathfindingNode();
	
	

	PathfindingNode* ParentNode;
	int GridX, GridY, GridZ;
	int GCost = 0; // Cost so far
	int HCost = 0; // Heuristic 
	float FCost(); // Total cost
};
