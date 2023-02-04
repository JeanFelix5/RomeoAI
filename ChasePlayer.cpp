// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get TargetLocation from blackboard via AI_controller
	ANPC_AIController* const controller = Cast<ANPC_AIController>(owner_comp.GetAIOwner());
	FVector const player_location = controller->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	if (controller->npc->getIsWinter() == true) {
		controller->npc->GetCharacterMovement()->MaxWalkSpeed = 250.0f;	//set the speed of Romeo to be constantly = 250.0f when he is in Winter mode
	}

	//move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, player_location);

	//finish with success
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
