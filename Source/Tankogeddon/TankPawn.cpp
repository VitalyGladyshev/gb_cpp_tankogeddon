// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/ArrowComponent.h"

DECLARE_LOG_CATEGORY_EXTERN(TankLog, All, All);
DEFINE_LOG_CATEGORY(TankLog);

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_ammunition = 30;
	_ammunitionSecond = 50;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = 
		CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, 
		FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	_bCurrentCannonMain = true;

	HealthComponent = 
		CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::DamageTaked);
	
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(CannonClass);
	SetupSecondCannon(SecondCannonClass);

	GetWorld()->GetTimerManager().SetTimer(ChangeCannonTimerHandle, this,
		&ATankPawn::ChangeCannonTimer, ChangeCannonInterval, false);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankMovement(DeltaTime);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Перемещение танка
void ATankPawn::TankMovement(float DeltaTime)
{
	// Tank movement
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = currentLocation + forwardVector * MoveSpeed *
		_targetForwardAxisValue * DeltaTime + rightVector * MoveSpeed *
		_targetRightAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);

	// Tank rotation
	_currentYawAxisValue = FMath::Lerp(_currentYawAxisValue, _targetYawAxisValue, InterpolationKey);
	UE_LOG(LogTemp, Warning, TEXT("_currentYawAxisValue = %f _targetYawAxisValue = % f"),
		_currentYawAxisValue, _targetYawAxisValue);
	float yawRotation = RotationSpeed * _currentYawAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);

	// Turret rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePos();
		RotateTurretTo(mousePos);
	}
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

// Огонь из пушки
void ATankPawn::Fire(bool bSpecial)
{
	if (_bCurrentCannonMain)
	{
		if (Cannon)
		{
			Cannon->Fire(_ammunition, bSpecial);
		}
	}
	else
	{
		if (SecondCannon)
		{
			SecondCannon->Fire(_ammunitionSecond, bSpecial);
		}
	}
}

void ATankPawn::ChangeCannon()
{
	_bCurrentCannonMain = !_bCurrentCannonMain;
}

void ATankPawn::ChangeCannonTimer()
{
	_bCurrentCannonMain = !_bCurrentCannonMain;

	GetWorld()->GetTimerManager().SetTimer(ChangeCannonTimerHandle, this,
		&ATankPawn::ChangeCannonTimer, ChangeCannonInterval, false);
}

// Установка пушки
void ATankPawn::SetupCannon(TSubclassOf<ACannon> clCannonClass)
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(clCannonClass, params);
	if (Cannon)
	{
		Cannon->AttachToComponent(CannonSetupPoint,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Blue,
			"Error!");
	}
}

// Установка второй пушки
void ATankPawn::SetupSecondCannon(TSubclassOf<ACannon> clCannonClass)
{
	if (SecondCannon)
	{
		SecondCannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	SecondCannon = GetWorld()->SpawnActor<ACannon>(clCannonClass, params);
	if (SecondCannon)
	{
		SecondCannon->AttachToComponent(CannonSetupPoint,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Blue,
			"Error!");
	}
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> points;
	for (ATargetPoint* point : PatrollingPoints)
	{
		points.Add(point->GetActorLocation());
	}
	return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation =
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation,
		TurretRotationInterpolationKey));
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

// Танк подбили
void ATankPawn::Die()
{
	Destroy();
}

// Танк повредили
void ATankPawn::DamageTaked(float DamageValue)
{
	UE_LOG(TankLog, Warning, TEXT("Tank %s taked damage:%f Health:%f"), 
		*GetName(), DamageValue, HealthComponent->GetHealth());
}
