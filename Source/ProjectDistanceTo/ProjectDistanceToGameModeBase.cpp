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

float AProjectDistanceToGameModeBase::GetAbsoluteDistanceToDistance(float d1, float d2) {

	return abs(d1 - d2);
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

	float distanceSum = 0.0;

	int actorsCount = 0;

	for (TActorIterator<ADistantActor> iActor(GetWorld()); iActor; ++iActor) {

		if (*iActor == actor) {
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor %s salteado"), *actor->GetName()));*/
			continue;
		}

		distanceSum += GetDistanceBetweenVectors(vec1, iActor->GetActorLocation());

		//vec2 += iActor->GetActorLocation();
		actorsCount++;
	}

	float avgDistance = distanceSum / actorsCount;

	return avgDistance;
}

FVector AProjectDistanceToGameModeBase::GetAverageDistanceVectorFromOtherActors(AActor* actor) {

	FVector vec1 = actor->GetActorLocation();

	FVector distanceSum = FVector(0,0,0);

	int actorsCount = 0;

	for (TActorIterator<ADistantActor> iActor(GetWorld()); iActor; ++iActor) {

		if (*iActor == actor) {
			continue;
		}

		distanceSum += vec1 - iActor->GetActorLocation();
		actorsCount++;
	}

	FVector avgDistance = distanceSum / actorsCount;

	return avgDistance;
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

		float distance = (sourceLocation - actors[i]->GetActorLocation()).Size();

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

		float distance = (sourceLocation - actors[i]->GetActorLocation()).Size();

		if (distance > currentFurthestDistance) {

			currentFurthestDistance = distance;
		}
	}

	return currentFurthestDistance;
}

AActor* AProjectDistanceToGameModeBase::GetClosestActorToDistanceNumber(AActor* skippedActor, float avgDistance, float distanceToActor, TArray<AActor*> actors) {

	if (actors.Num() <= 0) {
		return nullptr;
	}

	AActor* currentClosestActorToAvg = nullptr;
	float currentClosestDistance = TNumericLimits<float>::Max();

	for (int i = 0; i < actors.Num(); i++) {

		if (actors[i] == skippedActor) {
			continue;
		}

		float distance = AProjectDistanceToGameModeBase::GetAbsoluteDistanceToDistance(avgDistance, distanceToActor);

		if (distance < currentClosestDistance) {

			if (distanceToActor == (GetAverageDistanceVectorFromOtherActors(actors[i]).Size())) {

				currentClosestDistance = distance;
				currentClosestActorToAvg = actors[i];
			}
		}
	}

	return currentClosestActorToAvg;

}

void AProjectDistanceToGameModeBase::BeginPlay(){

	// Call method from superclass
	Super::BeginPlay();

	TMap<AActor*, FDistanceStruct> ActorsMap;

	// Create actors array
	AProjectDistanceToGameModeBase::CreateActorsArray();

	// Fulfill TMap
	float avgDistanceFromActors = 0.0;
	float closestActorDistance = TNumericLimits<float>::Max();
	float furthestActorDistance = 0.0;
	AActor* closestToMyAvgActor = nullptr;
	FDistanceStruct myStruct;

	for (TActorIterator<ADistantActor> actor(GetWorld()); actor; ++actor) {

		// Call methods and store it's values in local variables
		avgDistanceFromActors = AProjectDistanceToGameModeBase::GetAverageDistanceFromOtherActors(*actor);
		closestActorDistance = AProjectDistanceToGameModeBase::GetClosestActorDistance(*actor, actor->GetActorLocation(), ActorsArray);
		furthestActorDistance = AProjectDistanceToGameModeBase::GetFurthestActorDistance(*actor, actor->GetActorLocation(), ActorsArray);
		closestToMyAvgActor = AProjectDistanceToGameModeBase::GetClosestActorToDistanceNumber(*actor, avgDistanceFromActors, closestActorDistance, ActorsArray);

		// Call struct's constructor with the params
		myStruct = FDistanceStruct(avgDistanceFromActors, closestActorDistance, furthestActorDistance, closestToMyAvgActor);

		// Add to TMap
		ActorsMap.Add(*actor, myStruct);

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
