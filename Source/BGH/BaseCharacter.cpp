// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "BaseCharacter.h"


ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer)
{
	Health = 100;
	MaxHealth = 100;
}

float ABaseCharacter::GetHealth() const
{
	return Health;
}

bool ABaseCharacter::IsAlive() const
{
	return Health > 0;
}

void ABaseCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Destroy();
}
