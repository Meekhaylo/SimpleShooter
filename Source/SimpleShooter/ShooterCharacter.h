// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"
class AGun; //#include "Gun.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LastHitDirection;

	void SetCombatReadyStatus();

	FTimerHandle CombatReadyTimerHandle;
	float CombatReadyDuration = 2.f;
	bool bCombatReadyStatus = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CombatReadyStatus = 0.f;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SCMapCont;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IAMoving;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IALooking;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IAJumping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IAPrimaryFire;


	void Moving(const FInputActionValue& Value);
	void Looking(const FInputActionValue& Value);
	void Jumping();

public: void PrimaryFire();
private:

	// void MoveForward(float AxisValue);
	// void LookUp(float AxisValue);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealthAmount = 10;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UFUNCTION(BlueprintPure)
	float GetCurrentHealthPercent();
	

};
