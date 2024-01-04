// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "PlataformInputModifier.generated.h"

/**
 * 
 */
UCLASS()
class PLATAFORM_API UPlataformInputModifier : public UInputModifier
{
	GENERATED_BODY()
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
