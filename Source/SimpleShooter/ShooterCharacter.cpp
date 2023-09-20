// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealthAmount;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(SCMapCont, 0);
		}
	}

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Custom_Weapon_Socket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCombatReadyStatus == false && CombatReadyStatus != 0.f)
	{
		CombatReadyStatus = FMath::FInterpConstantTo(CombatReadyStatus, 0.f, DeltaTime, 3);
		//UE_LOG(LogTemp, Warning, TEXT("Combat Ready Status = %f"), CombatReadyStatus);
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);7
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IAMoving, ETriggerEvent::Triggered, this, &AShooterCharacter::Moving);
		EnhancedInputComponent->BindAction(IALooking, ETriggerEvent::Triggered, this, &AShooterCharacter::Looking);
		EnhancedInputComponent->BindAction(IAJumping, ETriggerEvent::Triggered, this, &AShooterCharacter::Jumping);
		EnhancedInputComponent->BindAction(IAPrimaryFire, ETriggerEvent::Triggered /*ETriggerEvent::Started*/, this, &AShooterCharacter::PrimaryFire);
	}
}



float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(CurrentHealth, DamageApplied);
	CurrentHealth -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("You took %f damage, and your current health is %f!"), DamageApplied, CurrentHealth);
	
	FHitResult HitResultTemp;
	//LastHitDirection = DamageEvent.GetBestHitInfo.;
	DamageEvent.GetBestHitInfo(this, EventInstigator, HitResultTemp, LastHitDirection);
	//LastHitDirection = DamageEvent.PointDamageEvent::ShotDirection;

	if (IsAlive())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Gun->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		/*Next function should be executed last, otherwise GameMode would not be able to execute GetPlayerController();*/
		DetachFromControllerPendingDestroy(); 
	}

	return DamageApplied;
}

bool AShooterCharacter::IsAlive() const
{
	return CurrentHealth <= 0;
}

void AShooterCharacter::Moving(const FInputActionValue& Value)
{
	const FVector2D MoveDirection = Value.Get<FVector2D>();
	if (GetController())
	{
		AddMovementInput(GetActorForwardVector(), MoveDirection.X);
		AddMovementInput(GetActorRightVector(), MoveDirection.Y);
	}
}

void AShooterCharacter::Looking(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void AShooterCharacter::Jumping()
{
	Super::JumpMaxCount = 2;
	Super::Jump();
}

void AShooterCharacter::PrimaryFire()
{
	Gun->PullTrigger();
	bCombatReadyStatus = true;
	CombatReadyStatus = 1.f;
	//UE_LOG(LogTemp, Warning, TEXT("Combat Ready Status = %f"), CombatReadyStatus);
	GetWorldTimerManager().SetTimer(CombatReadyTimerHandle, this, &AShooterCharacter::SetCombatReadyStatus, CombatReadyDuration, false);
}

float AShooterCharacter::GetCurrentHealthPercent()
{
	return CurrentHealth / MaxHealthAmount;
}

void AShooterCharacter::SetCombatReadyStatus()
{
	bCombatReadyStatus = false;
	//UE_LOG(LogTemp, Warning, TEXT("Combat Ready Status = %f"), CombatReadyStatus);
}