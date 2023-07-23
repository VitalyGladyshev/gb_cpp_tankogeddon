// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
	ACannon* Cannon;
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 750;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;

	int _ammunition;

	//const FString BodyMeshPath = "C:/Unreal Projects/Tankogeddon/Content/Assets/Meshes/SM_TowerBase.uasset";
	//	 // "/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TowerBase.SM_TowerBase'";		// "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'"
	//const FString TurretMeshPath = "C:/Unreal Projects/Tankogeddon/Content/Assets/Meshes/SM_TowerTurret.uasset";
	//	 // "/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TowerTurret.SM_TowerTurret'";		// "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'"

public:	
	// Sets default values for this actor's properties
	ATurret();

	UFUNCTION()
	void TakeDamage(FDamageData DamageData);
	UFUNCTION()
	void Die();
	UFUNCTION()
	void DamageTaked(float DamageValue);

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	void Targeting();
	
	void RotateToPlayer();
	
	bool IsPlayerInRange();
	
	bool CanFire();
	
	void Fire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
