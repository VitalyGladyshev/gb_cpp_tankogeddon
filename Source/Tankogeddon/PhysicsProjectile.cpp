// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "DrawDebugHelpers.h"
#include "MyPhysicsComponent.h"
#include "DamageTaker.h"
#include "Kismet/KismetMathLibrary.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = 
		CreateDefaultSubobject<UMyPhysicsComponent>(TEXT("MyPhysicsComponent"));
	TrailEffect = 
		CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);

	//Damage = 5;
}

void APhysicsProjectile::Start()
{
	Damage = 5;
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), 
		MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
	
	if (ShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5, 8, 
				FColor::Purple, true, 1, 0, 2);
		}
	}
	
	TragectoryPointIndex = 0;
	Super::Start();
}

void APhysicsProjectile::Move()
{
	FVector currentMoveVector = 
		CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
	currentMoveVector.Normalize();
	FVector newLocation = 
		GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);

	if (FVector::Distance(newLocation, 
		CurrentTrajectory[TragectoryPointIndex]) <=	MoveAccurency)
	{
		TragectoryPointIndex++;
		if (TragectoryPointIndex >= CurrentTrajectory.Num())
		{
			if (UseExplode)
				Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation =
				UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), 
					CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}
