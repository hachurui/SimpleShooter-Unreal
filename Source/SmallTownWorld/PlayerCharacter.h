// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AGun;
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
UCLASS()
class SMALLTOWNWORLD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	bool IsDead() const;
	
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;
	/*-------------------------GETTERS AND SETTERS----------------------------*/

	/*-------------------------------INPUT-----------------------------------*/
	void FireFunction();
private:
/*----------------------------COMPONENTS----------------------------------*/
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
/*-----------------------GENERAL CHARACTERISTICS--------------------------*/
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed = 142.f;
	float JogSpeed = 142.f;
	bool IsSprinting = false;
	float SprintSpeed = 474.f;

		//This is set in blueprint and dictates what type of gun player will start with
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;
		//This is a reference to the gun the player starts with itself
	UPROPERTY()
	AGun* Gun;

/*----------------------INPUT ACTION AND MAPPINGS-------------------------*/
	APlayerController* PCPlayerController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveForwardAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveRightAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookUpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookRightAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

/*------------INPUT FUNCTION--------------*/
	void MoveForwardFunction(const FInputActionValue& pValue);
	void MoveRightFunction(const FInputActionValue& pValue);
	void LookUpFunction(const FInputActionValue& pValue);
	void LookRightFunction(const FInputActionValue& pValue);
	void JumpFunction(const FInputActionValue& pValue);
	void SprintFunction(const FInputActionValue& pValue);
};
