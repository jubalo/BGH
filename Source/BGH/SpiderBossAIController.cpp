// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "SpiderBossAIController.h"
#include "SpiderBoss.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpiderBossAIController::ASpiderBossAIController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	NextLocationKeyName = "NextLocation";
	TargetToFollowKeyName = "TargetToFollow";
}

void ASpiderBossAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ASpiderBoss* SpiderBot = Cast<ASpiderBoss>(InPawn);
	if (SpiderBot)
	{
		if (SpiderBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*SpiderBot->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*SpiderBot->BehaviorTree);
	}
}


void ASpiderBossAIController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}

void ASpiderBossAIController::SetTargetToFollow(APawn* Pawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetToFollowKeyName, Pawn);
	}
}

void ASpiderBossAIController::SetNextWaypoint(FVector NextWaypoint)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(NextLocationKeyName, NextWaypoint);
	}
}


