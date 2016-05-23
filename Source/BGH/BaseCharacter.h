// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class BGH_API ABaseCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
public:

	ABaseCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
		bool IsAlive() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
		float MaxHealth;

	/* Take damage & handle death */

	virtual void Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);	
};
