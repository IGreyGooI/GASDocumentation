// Copyright 2023 Dan Kestranek.


#include "Characters/Abilities/HealAmountCalculation.h"

#include "Characters/Abilities/AttributeSets/GDAttributeSetBase.h"

struct HealStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	HealStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSetBase, MaxHealth, Target, false);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSetBase, Health, Target, false);
	}
};

static const HealStatics& GetHealStatics()
{
	static HealStatics DStatics;
	return DStatics;
}

UHealAmountCalculation::UHealAmountCalculation()
{
	RelevantAttributesToCapture.Add(GetHealStatics().HealthDef);
	RelevantAttributesToCapture.Add(GetHealStatics().MaxHealthDef);
}


void UHealAmountCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;

	float Health;
	ExecutionParams.
		AttemptCalculateCapturedAttributeMagnitude(GetHealStatics().HealthDef, EvaluationParameters, Health);
	float MaxHealth;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetHealStatics().MaxHealthDef, EvaluationParameters,
	                                                           MaxHealth);

	const auto HealthPercentage = FMath::Clamp(Health / MaxHealth, 0.0, 1.0);

	const auto HealthAmount = ((1 - HealthPercentage) * 0.40) * MaxHealth;
	
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(GetHealStatics().HealthProperty, EGameplayModOp::Additive, HealthAmount));
}
