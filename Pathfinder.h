// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "PathfindingGrid.h"
#include "RunnableThread.h"
#include "GameFramework/Actor.h"
#include "Pathfinder.generated.h"

UENUM(BlueprintType)
enum class EHeuristic : uint8
{
	Heuristic_Manhatten 	UMETA(DisplayName = "Manhatten"),
	Heuristic_Euclidean 	UMETA(DisplayName = "Euclidean"),
	
};


UCLASS()
class RACINGGAME_API APathfinder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfinder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EHeuristic Heuristic;

	UFUNCTION(BlueprintCallable, Category = "AStar")
		FVector FindPath(FVector Start, FVector End);

	//UFUNCTION(BlueprintCallable, Category = "Navigation-GL")
		//void DrawPath(TArray<PathfindingNode*> Path);

	FVector RetracePath(PathfindingNode* StartNode, PathfindingNode* EndNode);
	float GetDistance(PathfindingNode* A, PathfindingNode* B);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = Pathfinding)
		APathfindingGrid* GridInstance;

	UPROPERTY(EditAnywhere, Category = Pathfinding)
		bool Debug;
	
};
