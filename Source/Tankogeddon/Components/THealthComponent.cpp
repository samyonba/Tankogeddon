
#include "THealthComponent.h"

UTHealthComponent::UTHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTHealthComponent::SetMaxHealth(float Value)
{
	MaxHealth = Value;
	CurrentHealth = Value;
}

void UTHealthComponent::TakeDamage(const FTDamageData& DamageData)
{
	const float TakenDamage =  DamageData.DamageValue;
	CurrentHealth = FMath::Max(0.f, CurrentHealth - TakenDamage);

	if(FMath::IsNearlyZero(CurrentHealth))
	{
		OnDieDelegate.Broadcast();
	}
}

float UTHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UTHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UTHealthComponent::AddHealth(float AdditionalHealth)
{
	CurrentHealth = FMath::Min(MaxHealth, (CurrentHealth + AdditionalHealth));
}
