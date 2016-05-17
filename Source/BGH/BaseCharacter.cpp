// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "BaseCharacter.h"


ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer)
{
	Health = 100;
}

float ABaseCharacter::GetHealth() const
{
	return Health;
}

bool ABaseCharacter::IsAlive() const
{
	return Health > 0;
}

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;

		if (Health <= 0)
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}

	return ActualDamage;
}

void ABaseCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Destroy();
}
