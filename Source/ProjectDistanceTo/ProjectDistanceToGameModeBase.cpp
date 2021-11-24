// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectDistanceToGameModeBase.h"
#include <ProjectDistanceTo/Public/DistanceStruct.h>
#include "EngineUtils.h"
#include "DistantActor.h"

float AProjectDistanceToGameModeBase::GetDistanceBetweenVectors(FVector vec1, FVector vec2) {

	FVector distanceVec = vec1 - vec2;
	float distance = distanceVec.Size();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Distance between actors %f"), distance));

	return distance;
}

int AProjectDistanceToGameModeBase::CreateActorsArray(){

	int actorsCount = 0;

	for (TActorIterator<ADistantActor> actor(GetWorld()); actor; ++actor) {

		ActorsArray.Push(*actor);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Added to array Actor: %s"), *actor->GetName()));
		actorsCount++;
	}

	return actorsCount;
}

float AProjectDistanceToGameModeBase::GetAverageDistanceFromOtherActors(AActor* actor){

	FVector vec1 = actor->GetActorLocation();
	FVector vec2;

	int actorsCount = 0;

	for (TActorIterator<ADistantActor> iActor(GetWorld()); iActor; ++iActor) {

		if (*iActor == actor) {
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor %s salteado"), *actor->GetName()));*/
			continue;
		}

		vec2 += iActor->GetActorLocation();
		actorsCount++;
	}

	vec2 /= actorsCount;

	FVector distanceVec = vec1 - vec2;
	float distance = distanceVec.Size();

	return distance;
}

float AProjectDistanceToGameModeBase::GetClosestActorDistance(AActor* skippedActor, FVector sourceLocation, TArray<AActor*> actors) {

	if (actors.Num() <= 0) {
		return 0.0;
	}

	float currentClosestDistance = TNumericLimits<float>::Max();

	for (int i = 0; i < actors.Num(); i++) {

		if (actors[i] == skippedActor) {
			continue;
		}

		float distance = FVector::DistSquared(sourceLocation, actors[i]->GetActorLocation());

		if (distance < currentClosestDistance) {

			currentClosestDistance = distance;
		}
	}

	return currentClosestDistance;
}

float AProjectDistanceToGameModeBase::GetFurthestActorDistance(AActor* skippedActor, FVector sourceLocation, TArray<AActor*> actors) {

	if (actors.Num() <= 0) {
		return 0.0;
	}

	float currentFurthestDistance = 0.0;

	for (int i = 0; i < actors.Num(); i++) {

		if (actors[i] == skippedActor) {
			continue;
		}

		float distance = FVector::DistSquared(sourceLocation, actors[i]->GetActorLocation());

		if (distance > currentFurthestDistance) {

			currentFurthestDistance = distance;
		}
	}

	return currentFurthestDistance;
}

void AProjectDistanceToGameModeBase::BeginPlay(){

	// Call method from superclass
	Super::BeginPlay();

	TMap<FString, FDistanceStruct> ActorsMap;

	// Create actors array
	AProjectDistanceToGameModeBase::CreateActorsArray();

	// Fulfill TMap
	float avgDistanceFromActors = 0.0;
	float closestActorDistance = 0.0;
	float furthestActorDistance = 0.0;
	FDistanceStruct myStruct;

	for (TActorIterator<ADistantActor> actor(GetWorld()); actor; ++actor) {

		// Call methods and store it's values in local variables
		avgDistanceFromActors = AProjectDistanceToGameModeBase::GetAverageDistanceFromOtherActors(*actor);
		closestActorDistance = AProjectDistanceToGameModeBase::GetClosestActorDistance(*actor, actor->GetActorLocation(), ActorsArray);
		furthestActorDistance = AProjectDistanceToGameModeBase::GetFurthestActorDistance(*actor, actor->GetActorLocation(), ActorsArray);

		// Call struct's constructor with the params
		myStruct = FDistanceStruct(avgDistanceFromActors, closestActorDistance, furthestActorDistance);

		// Add to TMap
		ActorsMap.Add(*actor->GetName(), myStruct);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 45.0f, FColor::Red, FString::Printf(TEXT("Actor %s | avgDistanceFromActors: %.2f | closestActorDistance: %.2f | furthestActorDistance: %.2f"), *actor->GetName(), avgDistanceFromActors, closestActorDistance, furthestActorDistance));
	}

	// Print info
	/*FString Message;

	if (ActorsMap.Num() > 0){ 

		if (GEngine){

			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Number of elements in the Map: %d"), ActorsMap.Num()));

			for (auto& amap : ActorsMap){

				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%s - %.2f"), *(amap.Key), amap.Value));
			}
		}
	}*/

}
