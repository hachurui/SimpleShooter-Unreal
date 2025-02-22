// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

	//ACTORS
#include "Gun.h"
#include "SmallTownWorldGameMode.h"

	//COMPONENTS
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
	//INPUT
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//PCPlayerController = Cast<APlayerController>(GetController());
	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	PCPlayerController = Cast<APlayerController>(GetController());
	if(PCPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PCPlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if(EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForwardFunction);
			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRightFunction);
			EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUpFunction);
			EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRightFunction);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpFunction);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SprintFunction);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintFunction);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::FireFunction);
		}
	}

}
void APlayerCharacter::MoveForwardFunction(const FInputActionValue& pValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("MoveForwardFunction pValue: %f"), pValue.Get<float>());
	AddMovementInput(GetActorForwardVector() * pValue.Get<float>());
}
void APlayerCharacter::MoveRightFunction(const FInputActionValue& pValue)
{
	AddMovementInput(GetActorRightVector() * pValue.Get<float>());
}
void APlayerCharacter::LookUpFunction(const FInputActionValue& pValue)
{
	float Direction = pValue.Get<float>();
	AddControllerPitchInput(Direction);
		//if the camera is looking too high or low and player is trying to look even higher or lower respectively, can't rotate any further
	/*float MaxRotation = 60.f;
	float MinRotation = -60.f;
	if((SpringArm->GetComponentRotation().Pitch > MaxRotation && Direction > 0.0f) || (SpringArm->GetComponentRotation().Pitch < MinRotation && Direction < 0.0f))
	{
		return;
	}
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Pitch = Direction;
	SpringArm->AddRelativeRotation(DeltaRotation);*/
	
}
void APlayerCharacter::LookRightFunction(const FInputActionValue& pValue)
{
	float Direction = pValue.Get<float>();
	AddControllerYawInput(Direction);
}

void APlayerCharacter::JumpFunction(const FInputActionValue& pValue)
{
	Jump();
}

void APlayerCharacter::SprintFunction(const FInputActionValue& pValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("IsSprint?: %d"), pValue.Get<bool>());
	/*IsSprinting = pValue.Get<bool>();
	if(IsSprinting) Speed = SprintSpeed;
	else Speed = JogSpeed;*/
}

void APlayerCharacter::FireFunction()
{
	Gun->PullTrigger();
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("DamageTaken = %f, Health Left = %f"), DamageToApply, Health);
	if(IsDead())
	{
		ASmallTownWorldGameMode* GameMode = GetWorld()->GetAuthGameMode<ASmallTownWorldGameMode>();
		if(!GameMode) return DamageToApply;
		GameMode->PawnKilled(this);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}

bool APlayerCharacter::IsDead() const
{
	if(Health <= 0) return true;
	else return false;
}

float APlayerCharacter::GetHealthPercentage() const
{
	return Health / MaxHealth;
}