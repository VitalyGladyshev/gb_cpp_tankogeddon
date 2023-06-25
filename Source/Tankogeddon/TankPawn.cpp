// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankMovement(DeltaTime);
}

//Перемещение танка
void ATankPawn::TankMovement(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = currentLocation + forwardVector * MoveSpeed *
		_targetForwardAxisValue * DeltaTime + rightVector * MoveSpeed *
		_targetRightAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);

	float yawRotation = RotationSpeed * _targetYawAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Движение танка вперёд-назад
void ATankPawn::MoveForward(float AxisValue)
{
	_targetForwardAxisValue = AxisValue;
}

// Движение танка вправо-влево
void ATankPawn::MoveRight(float AxisValue)
{
	_targetRightAxisValue = AxisValue;
}

// Поворот танка направо-налево
void ATankPawn::RotateRight(float AxisValue)
{
	_targetYawAxisValue = AxisValue;
}
