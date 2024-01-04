// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "PaperFlipbook.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Containers/Map.h"
#include "PlayerPaperCharacter.generated.h"

UENUM(BlueprintType)
enum class EAnimationType : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	WALK UMETA(DisplayName = "WALK"),
	ATTACK UMETA(DisplayName = "ATTACK"),
};

UENUM(BlueprintType)
enum class EInputType : uint8
{
	JUMP_ACTION   UMETA(DisplayName = "JUMP_ACTION"),
	MOVE_ACTION   UMETA(DisplayName = "MOVE_ACTION"),
	ATTACK_ACTION UMETA(DisplayName = "ATTACK_ACTION"),
};


UCLASS()
class PLATAFORM_API APlayerPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	// Auxiliary functions
	void UpdateAnimations();
	void Flip(UPaperFlipbookComponent* FlipbookComponent);

	// Menbers
	EAnimationType CurrentAnimation;

protected:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TMap< EAnimationType, UPaperFlipbook*> AnimationsMap;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TMap< EInputType, UInputAction*> InputsMap;
	
	// Life cycle functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	APlayerPaperCharacter();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
