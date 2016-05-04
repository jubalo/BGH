// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "SpiderAIController.h"
#include "EnemySpider.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpiderAIController::ASpiderAIController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	HomeLocationKeyName = "HomeLocation";
	TargetToFollowKeyName = "TargetToFollow";
}

void ASpiderAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemySpider* SpiderBot = Cast<AEnemySpider>(InPawn);
	if (SpiderBot)
	{
		if (SpiderBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*SpiderBot->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*SpiderBot->BehaviorTree);

		BlackboardComp->SetValueAsVector(HomeLocationKeyName, FVector(600.0f, 300.0f, 0.0f));
	}
}


void ASpiderAIController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}

void ASpiderAIController::SetTargetToFollow(AHunter* Hunter)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetToFollowKeyName, Hunter);
	}
}



