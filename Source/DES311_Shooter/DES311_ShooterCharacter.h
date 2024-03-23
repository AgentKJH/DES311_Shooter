// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EnumsLibrary.h"
#include "DES311_ShooterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ADES311_ShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	//USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** Hammer Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HammerAction;

public:
	ADES311_ShooterCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	// ----- Shooting Functions -----
	UFUNCTION(BlueprintCallable)
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void Hammer();

	UFUNCTION()
	void HammerPullAnimFinished();

	UFUNCTION()
	void ShootAnimFinished();

	// ----- Shooting Variables -----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float maxBulletRange = 2500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float baseRevolverDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TEnumAsByte<EweaponState> weaponState;

	bool pullHammerFromShoot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UAnimationAsset* hammerAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UAnimationAsset* shootAnim;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* hitEffectSystem;*/

	/** Bool for AnimBP to switch to another animation set */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	//bool bHasRifle;

	/** Setter to set the bool */
	//UFUNCTION(BlueprintCallable, Category = Weapon)
	//void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	//UFUNCTION(BlueprintCallable, Category = Weapon)
	//bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	//USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:

};

