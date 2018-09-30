// Fill out your copyright notice in the Description page of Project Settings.

#include "PathfindingGrid.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

// Sets default values
APathfindingGrid::APathfindingGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void APathfindingGrid::BeginPlay()
{
	Super::BeginPlay();
	BuildGrid();
}

// Called every frame
void APathfindingGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugGrid();
	//UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation() + FVector(x, y, z)*NodeSize, FVector::OneVector*NodeSize / 2.3f, FLinearColor::Red, FRotator::ZeroRotator, 0);
}

void APathfindingGrid::BuildGrid()
{
	for (int x = 0; x < GridSize.X; x++)
	{
		NodeGrid.Add(TArray<TArray<PathfindingNode*>>());
		for (int y = 0; y < GridSize.Y; y++)
		{
			NodeGrid[x].Add(TArray<PathfindingNode*>());
			for (int z = 0; z < GridSize.Z; z++)
			{


				bool hit;
				hit = UKismetSystemLibrary::BoxTraceSingleForObjects(this, GetActorLocation() + FVector(x, y, z)*NodeSize, GetActorLocation() + FVector(x, y, z)*NodeSize, FVector::OneVector*NodeSize / 2, FRotator::ZeroRotator, Trace, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
				PathfindingNode* currentNode = new PathfindingNode( x, y, z);
				if (hit)
				{
					NodeGrid[x][y].Add(nullptr);
				}
				else
				{
					NodeGrid[x][y].Add(currentNode);
				}

				
				
				
			}
		}
	}
	
}

void APathfindingGrid::DebugGrid()
{
	if (DebugDraw)
	{
		for (int x = 0; x < GridSize.X; x++)
		{
			for (int y = 0; y < GridSize.Y; y++)
			{
				for (int z = 0; z < GridSize.Z; z++)
				{
					if (NodeGrid[x][y][z] == nullptr)
					{
						UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation() + FVector(x, y, z)*NodeSize, FVector::OneVector*NodeSize / 2.3f, FLinearColor::Red, FRotator::ZeroRotator, 0);
					}
					else
					{
						UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation() + FVector(x, y, z)*NodeSize, FVector::OneVector*NodeSize / 2.3f, FLinearColor::Green, FRotator::ZeroRotator, 0);
					}
				}
			}
		}

		
	}
	

}

TArray<PathfindingNode*> APathfindingGrid::GetNeighbourNodes(PathfindingNode* const &Node)
{
	//UKismetSystemLibrary::DrawDebugBox(this, LocationFromNode(Node), FVector::OneVector*NodeSize / 2.3f, FLinearColor::Black, FRotator::ZeroRotator, 0);
	TArray<PathfindingNode*> Neighbours;
	
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				
				if (x == 1 && y == 1 && z == 1) { continue; } // Node we came from
				
				int XIndex = Node->GridX + x - 1;
				int ZIndex = Node->GridZ + z - 1;
				int YIndex = Node->GridY + y - 1;

				if (XIndex >= 0 && XIndex <= GridSize.X - 1 && ZIndex >= 0 && ZIndex <= GridSize.Z - 1 && YIndex >= 0 && YIndex <= GridSize.Y - 1)	// Check the neighbour node is withing the array bounds
				{
					if (NodeGrid[XIndex][YIndex][ZIndex] != nullptr)
					{
						Neighbours.Add(NodeGrid[XIndex][YIndex][ZIndex]);
					}
				}
				
			}
		}
	}
	for (int i = 0; i < Neighbours.Num(); i++)
	{
		if(Neighbours[i] != nullptr)
		UKismetSystemLibrary::DrawDebugBox(this, LocationFromNode(Neighbours[i]), FVector::OneVector*NodeSize / 2.3f, FLinearColor::Blue, FRotator::ZeroRotator, 0);
	}
	
	return Neighbours;

}

PathfindingNode* APathfindingGrid::NodeFromLocation(FVector const &loc)
{
	FVector GridLocation = ((loc - GetActorLocation()) / NodeSize);
	FVector RoundedGridLocation = FVector(
		UKismetMathLibrary::Clamp(UKismetMathLibrary::Round(GridLocation.X), 0, GridSize.X - 1),
		UKismetMathLibrary::Clamp(UKismetMathLibrary::Round(GridLocation.Y), 0, GridSize.Y - 1),
		UKismetMathLibrary::Clamp(UKismetMathLibrary::Round(GridLocation.Z), 0, GridSize.Z - 1));

	PathfindingNode* test = NodeGrid[RoundedGridLocation.X][RoundedGridLocation.Y][RoundedGridLocation.Z];

	return NodeGrid[RoundedGridLocation.X][RoundedGridLocation.Y][RoundedGridLocation.Z];
}

FVector APathfindingGrid::LocationFromNode(PathfindingNode* const &Node)
{

	return GetActorLocation() + FVector(Node->GridX, Node->GridY, Node->GridZ)*NodeSize;

}

