// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinder.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Algo/Reverse.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

// Sets default values
APathfinder::APathfinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathfinder::BeginPlay()
{
	Super::BeginPlay();
	//FRunnableThread::Create(this)
	
}

// Called every frame
void APathfinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FVector APathfinder::FindPath(FVector Start, FVector End)
{
	PathfindingNode* StartNode = GridInstance->NodeFromLocation(Start);	// Start node of the agent
	PathfindingNode* EndNode = GridInstance->NodeFromLocation(End);	// End goal node of the agent



	if (StartNode == nullptr) return Start;
	
	TArray<PathfindingNode*> OpenSet; // Nodes to be checked
	TArray<PathfindingNode*> ClosedSet; // Nodes already checked

	OpenSet.Add(StartNode); // Load our intial Node into the algorithm

	while (OpenSet.Num()) // While there is still nodes to be checked keep running the algorithm
	{
		
		PathfindingNode* CurrentNode = OpenSet[0]; // Run each cycle from the beginning element of the open list as the previous first element will have been processed and place into the closed list

		for (int i = 0; i < OpenSet.Num(); i++)
		{
			if (OpenSet[i] != nullptr && OpenSet[i]->FCost() <= CurrentNode->FCost() && OpenSet[i]->HCost < CurrentNode->HCost) 
			{
				CurrentNode = OpenSet[i]; // Set Current node to be the closest node to the target
			}
		}
		//UKismetSystemLibrary::DrawDebugBox(this, GridInstance->LocationFromNode(CurrentNode), FVector::OneVector*GridInstance->NodeSize / 1, FLinearColor::Green, FRotator::ZeroRotator, 0);
		OpenSet.Remove(CurrentNode); // Once node has been processed remove it from the OpenSet
		ClosedSet.Add(CurrentNode); // Then add it to the closed set

		if (CurrentNode == EndNode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(red));
			return RetracePath(StartNode, EndNode); // If the CurrentNode is also the EndNode then we have reached out destination in the algorithm
		}

		TArray<PathfindingNode*> Neighbours = GridInstance->GetNeighbourNodes(CurrentNode); // Gather CurrentNodes neighbours into the list

		for (int i = 0; i < Neighbours.Num(); i++)
		{
			if (Neighbours[i] == nullptr || ClosedSet.Contains(Neighbours[i]))
			{
				continue;
			}

			float NewMovementCostToNeighbour = CurrentNode->GCost + GetDistance(CurrentNode, Neighbours[i]);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(NewMovementCostToNeighbour));
			if (NewMovementCostToNeighbour < Neighbours[i]->GCost || !OpenSet.Contains(Neighbours[i]))
			{
				Neighbours[i]->GCost = NewMovementCostToNeighbour;
				Neighbours[i]->HCost = GetDistance(Neighbours[i], EndNode);
				Neighbours[i]->ParentNode = CurrentNode;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Neighbours[i]->GCost));
				if (!OpenSet.Contains(Neighbours[i]))
				{
					OpenSet.Add(Neighbours[i]);
				}
			}

		}

	}

	

	return GridInstance->LocationFromNode(StartNode);	// No path found
}

float APathfinder::GetDistance(PathfindingNode* A, PathfindingNode* B)
{

	if (Heuristic == EHeuristic::Heuristic_Manhatten)
	{

		FVector ALoc, BLoc, Loc;
		ALoc = GridInstance->LocationFromNode(A);
		BLoc = GridInstance->LocationFromNode(B);

		Loc = ALoc - BLoc;

		return UKismetMathLibrary::Abs(Loc.X) + UKismetMathLibrary::Abs(Loc.Y) + UKismetMathLibrary::Abs(Loc.Z);
	}
	else if(Heuristic == EHeuristic::Heuristic_Euclidean)
	{
		return FVector::Distance(GridInstance->LocationFromNode(A), GridInstance->LocationFromNode(B));

	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HEURISTIC MISSING, RETURNING 0 DISTANCE"));
	return 0; 

}

FVector APathfinder::RetracePath(PathfindingNode* StartNode, PathfindingNode* EndNode)
{

	if (StartNode == EndNode)
	{
		return GridInstance->LocationFromNode(StartNode);
	}

	TArray<PathfindingNode*> Path;

	PathfindingNode* CurrentNode = EndNode;

	while (CurrentNode != StartNode)
	{
		Path.Add(CurrentNode);
		CurrentNode = CurrentNode->ParentNode;
		
	}

	Algo::Reverse(Path);
	for (int i = 0; i < Path.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugBox(this, GridInstance->LocationFromNode(Path[i]), FVector::OneVector*GridInstance->NodeSize / 2.3f, FLinearColor::Green, FRotator::ZeroRotator,0);
	}

	//if (Debug) DrawPath(Path);
	
	return GridInstance->LocationFromNode(Path[1]);
} 