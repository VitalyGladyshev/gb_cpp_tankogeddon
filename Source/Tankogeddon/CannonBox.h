// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonBox.generated.h"

UCLASS()
class TANKOGEDDON_API ACannonBox : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon class")
	TSubclassOf<ACannon> CannonClass;

public:	
	// Sets default values for this actor's properties
	ACannonBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
