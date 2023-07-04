// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this,
		&ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this,
		&ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this,
		&ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, 
		&ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this,
		&ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ChangeCannon", IE_Pressed, this,
		&ATankPlayerController::ChangeCannon);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::MoveRight(float AxisValue)
{
	TankPawn->MoveRight(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	
	FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);
	FVector pawnPos = TankPawn->GetActorLocation();
	//DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.3f, 0, 5);
	MousePos.Z = pawnPos.Z;
	FVector dir = MousePos - pawnPos;
	dir.Normalize();
	MousePos = pawnPos + dir * 1000;
	DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.3f, 0, 5);
}

void ATankPlayerController::Fire()
{
	TankPawn->Fire();
}

void ATankPlayerController::FireSpecial()
{
	TankPawn->Fire(true);
}

void ATankPlayerController::ChangeCannon()
{
	TankPawn->ChangeCannon();
}
