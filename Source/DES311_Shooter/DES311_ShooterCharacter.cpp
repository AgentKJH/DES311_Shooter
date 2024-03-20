// Copyright Epic Games, Inc. All Rights Reserved.

#include "DES311_ShooterCharacter.h"
#include "DES311_ShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"

//////////////////////////////////////////////////////////////////////////
// ADES311_ShooterCharacter

ADES311_ShooterCharacter::ADES311_ShooterCharacter()
{
	// Character doesnt have a rifle at start
	//bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create Gun Mesh Component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(FirstPersonCameraComponent);
	GunMesh->SetRelativeLocation(FVector(0, 0.f, 0));


	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);
	//Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	//Mesh1P->bCastDynamicShadow = false;
	//Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	//Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ADES311_ShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ADES311_ShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADES311_ShooterCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADES311_ShooterCharacter::Look);

		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADES311_ShooterCharacter::Shoot);

		// Hammer
		EnhancedInputComponent->BindAction(HammerAction, ETriggerEvent::Triggered, this, &ADES311_ShooterCharacter::Hammer);
	}
}


void ADES311_ShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADES311_ShooterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADES311_ShooterCharacter::Shoot()
{
	GEngine->AddOnScreenDebugMessage(30, 3, FColor::Magenta, "Shoot Input");
	if (weaponState == EweaponState::Idle) { // if idle call to pull hammer
		pullHammerFromShoot = true;
		Hammer();
	}
	else if (weaponState == EweaponState::HammerPulled) {
		PullTrigger();
	}
}

void ADES311_ShooterCharacter::Hammer()
{
	GEngine->AddOnScreenDebugMessage(31, 3, FColor::Magenta, "Hammer Input");

	weaponState = EweaponState::PullingHammer;
	//GEngine->AddOnScreenDebugMessage(35, 3, FColor::Magenta, "Hammer Input");
	//GunMesh->PlayAnimation(hammerAnim, false);
}

void ADES311_ShooterCharacter::HammerPullAnimFinished()
{
	GEngine->AddOnScreenDebugMessage(32, 3, FColor::Magenta, "Hammer Anim Finished");

	if (pullHammerFromShoot) { // pull trigger if triggered by shoot input
		pullHammerFromShoot = false;
		PullTrigger();
	}
	else {
		weaponState = EweaponState::HammerPulled;
	}
}

void ADES311_ShooterCharacter::ShootAnimFinished()
{
	GEngine->AddOnScreenDebugMessage(33, 3, FColor::Magenta, "Shoot Anim Finished");
	weaponState = EweaponState::Idle;
}

/// <summary>
/// Fires a line trace from the camera and deals damage to hit actor
/// </summary>
void ADES311_ShooterCharacter::PullTrigger() 
{
	// Handle Animation and weapon state
	weaponState = EweaponState::Shooting;
	GunMesh->PlayAnimation(shootAnim, false);

	FVector cameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector cameraForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector traceEndLocation = cameraLocation + (cameraForwardVector * maxBulletRange);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	FHitResult Hit;

	GEngine->AddOnScreenDebugMessage(5, 5, FColor::Red, "PulledTrigger");


	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, cameraLocation, traceEndLocation, ECC_EngineTraceChannel2, traceParams);

	if (bSuccess) {
		// debug traces
		DrawDebugLine(GetWorld(), cameraLocation, traceEndLocation, FColor::Green, false, 2.0f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), Hit.Location, 10, 12, FColor::Red, false, 2.0f, 0, 2.0f);

		AActor* hitActor = Hit.GetActor(); // get hit actor pointer ref
		UPrimitiveComponent* hitComponent = Hit.GetComponent(); // get hit component pointer ref 

		GEngine->AddOnScreenDebugMessage(66, 5, FColor::Red, hitComponent->GetName());
		//GEngine->AddOnScreenDebugMessage(67, 5, FColor::Orange, Hit.Location.ToString());

		if (hitActor != nullptr) {
			//GEngine->AddOnScreenDebugMessage(68, 5, FColor::Red, "HitActor not null");
			FVector shotDirection = (traceEndLocation - cameraLocation).GetSafeNormal(); // get shot direction

			float damage = 100;
			FPointDamageEvent PointDamage(damage, Hit, shotDirection, nullptr);
			
			hitActor->TakeDamage(damage, PointDamage, GetController(), this);

			GEngine->AddOnScreenDebugMessage(69, 5, FColor::Cyan, "Take Damamge Called");
		}

		FString hitLocation = Hit.Location.ToString();
		//GEngine->AddOnScreenDebugMessage(70, 5, FColor::Cyan, hitLocation);
	}
}

//void ADES311_ShooterCharacter::SetHasRifle(bool bNewHasRifle)
//{
//	bHasRifle = bNewHasRifle;
//}

//bool ADES311_ShooterCharacter::GetHasRifle()
//{
//	return bHasRifle;
//}