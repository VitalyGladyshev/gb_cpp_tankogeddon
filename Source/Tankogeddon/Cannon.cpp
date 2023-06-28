// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
//#include "Misc/CString.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceeneCpm = 
		CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = 
		CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::Fire(int &iAmmunition, const bool bSpecial)
{
	if (!ReadyToFire)
	{
		return;
	}
	ReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{
		if (iAmmunition)
		{
			if (bSpecial)
			{
				GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red,
					"Fire - projectile Ammo: " +
					FString::FromInt(iAmmunition) +
					" Series: 1");

				iCurrentSeries = 1;

				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
					&ACannon::SeriesFire, 0.5f, false);

				iAmmunition--;
				return;
			}
			else
				GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green,
					"Fire - projectile Ammo: " + FString::FromInt(iAmmunition));
			iAmmunition--;
		}
		else
			GEngine->AddOnScreenDebugMessage(10, 1, FColor::Black, 
				"Empty ammo");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
		&ACannon::Reload, 1 / FireRate, false);
}

void ACannon::SeriesFire()
{
	iCurrentSeries++;
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red,
		"Fire - projectile Series: " + FString::FromInt(iCurrentSeries));

	if (iCurrentSeries < FireSeries)
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
			&ACannon::SeriesFire, 0.5f, false);
	else
		Reload();
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}
