// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Hunter.h"
#include "SpiderBossAIController.generated.h"

/**
*
*/
UCLASS()
class BGH_API ASpiderBossAIController : public AAIController
{
	GENERATED_BODY()

	ASpiderBossAIController(const class FObjectInitializer& ObjectInitializer);

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetToFollowKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName NextLocationKeyName;

public:

	void SetTargetToFollow(APawn* Pawn);

	void SetNextWaypoint(FVector NextWaypoint);

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
