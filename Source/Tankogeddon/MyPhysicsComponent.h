// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyPhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UMyPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	float Gravity = -9.8f;

public:	
	// Sets default values for this component's properties
	UMyPhysicsComponent();

	TArray<FVector> GenerateTrajectory(FVector StartPos, FVector Velocity, 
		float MaxTime, float TimeStep, float MinZValue = 0);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

		
};
