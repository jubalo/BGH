// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "Hunter.generated.h"

UCLASS()
class BGH_API AHunter : public ABaseCharacter
{
	GENERATED_BODY()

	AHunter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UCapsuleComponent* MeleeCollisionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Hunter Sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hunter", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* HunterSprite;

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPawnNoiseEmitterComponent* NoiseMaker;

	float Orientation;

	bool bAttacking;

	bool bWantsToAttack;

	bool bLoadingBow;

	float MeleeStrikeCooldown;

	float LastMeleeAttackTime;

	FTimerHandle AttackTimerHandler;

	FTimerHandle BowTimerHandler;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* LeftSideRunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RightSideRunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* UpRunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DownRunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleDownAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleUpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleRightAnimation;

	// The animation to play when attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackDownAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackUpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackRightAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* BowLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* BowRightAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* BowUpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* BowDownAnimation;

	void HorizontalMove(float Value);

	void VerticalMove(float Value);

	void UpdateCharacter();

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void BeginSwordAttack();

	void QueueStopAttack();

	void StopSwordAttack();

	void BeginLoadingBow();

	void ShootArrow();

	void StopLoadingBow();

	UFUNCTION(Category = "Attacking")
		void PerformMeleeStrike(AActor* HitActor);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MeleeDamage;
};