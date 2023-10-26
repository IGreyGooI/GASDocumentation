// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "HealAmountCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GASDOCUMENTATION_API UHealAmountCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UHealAmountCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
