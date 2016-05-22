// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "EnemySpider.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class BGH_API AEnemySpider : public ABaseCharacter
{
	GENERATED_BODY()

	AEnemySpider(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	bool bSensedTarget;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UCapsuleComponent* MeleeCollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SenseTimeOut;

	float LastHeardTime;

	float LastMeleeAttackTime;

	FVector HomeLocation;

	// Booleans
	bool bIsAttacking;

	bool bChasing;

protected:

	// The animations to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkUpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkDownAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkRightAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation;

	UFUNCTION()
		void OnHearNoise(APawn* Pawn, const FVector& Location, float Volume);

	UFUNCTION(Category = "Attacking")
		void PerformMeleeStrike(AActor* HitActor);

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MeleeDamage;

	float MeleeStrikeCooldown;

	UFUNCTION()
		void OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void OnRetriggerMeleeStrike();

	void UpdateCharacter();

	void UpdateAnimation();

	FTimerHandle TimerHandle_MeleeAttack;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
};
