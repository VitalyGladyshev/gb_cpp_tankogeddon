// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBox.h"
#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACannonBox::ACannonBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceeneCpm =
		CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACannonBox::OnMeshOverlapBegin);
	//Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void ACannonBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Blue,
		"Collision!");

	UE_LOG(LogTemp, Warning, TEXT("Tank %s collided with %s. "), *GetName(),
		*OtherActor->GetName());

	//!!!
	Destroy();

	ATankPawn* playerPawn =
		Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (OtherActor == playerPawn)
	{
		playerPawn->SetupCannon(CannonClass);
		Destroy();
	}
}

// Called when the game starts or when spawned
void ACannonBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannonBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

