
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "DistanceStruct.generated.h"

USTRUCT(BlueprintType)
struct FDistanceStruct {

	GENERATED_BODY()

	FORCEINLINE FDistanceStruct();

	explicit FORCEINLINE FDistanceStruct(float FloatValue, float FloatDistanceMin, float FloatDistanceMax);

	UPROPERTY(BlueprintReadWrite)
	float averageDistance;

	UPROPERTY(BlueprintReadWrite)
	float distanceMin;

	UPROPERTY(BlueprintReadWrite)
	float distanceMax;


};

FORCEINLINE FDistanceStruct::FDistanceStruct()
{
}

FORCEINLINE FDistanceStruct::FDistanceStruct(const float FloatValue, const float FloatDistanceMin, const float FloatDistanceMax) : averageDistance(FloatValue), distanceMin(FloatDistanceMin), distanceMax(FloatDistanceMax)
{
}

FORCEINLINE uint32 GetTypeHash(const FDistanceStruct& b) {
	return FCrc::MemCrc_DEPRECATED(&b, sizeof(FDistanceStruct));
}