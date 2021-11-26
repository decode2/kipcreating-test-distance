// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectDistanceToGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDISTANCETO_API AProjectDistanceToGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	TArray<AActor*> ActorsArray;

	virtual float GetDistanceBetweenVectors(FVector vec1, FVector vec2);

	virtual int CreateActorsArray();
	
	virtual float GetAverageDistanceFromOtherActors(AActor* actor);

	virtual float GetClosestActorDistance(AActor* skippedActor, FVector sourceLocation, TArray<AActor*> actors);
	virtual float GetFurthestActorDistance(AActor* skippedActor, FVector sourceLocation, TArray<AActor*> actors);

	float GetAbsoluteDistanceToDistance(float d1, float d2);
	FVector GetAverageDistanceVectorFromOtherActors(AActor* actor);
	AActor* AProjectDistanceToGameModeBase::GetClosestActorToDistanceNumber(AActor* skippedActor, float avgDistance, float distanceToActor, TArray<AActor*> actors);

protected:
	virtual void BeginPlay() override;
};
