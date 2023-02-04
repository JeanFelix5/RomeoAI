// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldHands.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"



UHoldHands::UHoldHands(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Hold Hands");
}

EBTNodeResult::Type UHoldHands::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) {

	//get TargetLocation from blackboard via AI_controller
	ANPC_AIController* const controller = Cast<ANPC_AIController>(owner_comp.GetAIOwner());
	FVector const player_location = controller->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	if (!ensure(controller->JulietteRef)) controller->InitializeJulietteRef();

	//Check if target in radius
	FVector const npc_location = controller->GetNPCLocation();
	float f_distance = FVector::Dist(npc_location, player_location);

	FVector vec_To_Player = player_location-npc_location;
	vec_To_Player = vec_To_Player / vec_To_Player.Size();

	if (f_distance > f_MaxDistanceToHoldHands && b_WaitInPlace) {
		FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else {
		//If Waiting, stop waiting
		if (controller->get_blackboard()->GetValueAsFloat(bb_keys::is_Waiting) > 0.0) {
			controller->OnEndWaiting(0.0f);
		}

		if (f_distance > f_OffsetDistance) {

			if (controller->npc->getIsWinter() == true) {
				controller->npc->SetMaxSpeed(475.0f * 1.2);	//Increase speed for winter AI here because he has a different speed (250.0f)
			}
			else if (f_distance >= f_OffsetDistance*1.5) {
				controller->npc->SetMaxSpeed(controller->npc->npc_WalkSpeed * 1.2); //Increase speed by 10% every time we do not reach target;
			}
		}
		
		else if(f_distance <= f_OffsetDistance){

			if (controller->npc->getIsWinter() == true) {
				controller->npc->SetMaxSpeed(600.0f * 0.75); //Slow down speed for winter AI here because he has a different speed (250.0f)
			}
			else {
				controller->npc->SetMaxSpeed(controller->JulietteRef->GetCharacterMovement()->MaxWalkSpeed); //better because doesn't cause a bug if juliette stand still
			}
		}

		
		FVector player_leftVector = -controller->JulietteRef->GetActorRightVector();
		FVector player_FrontVector = controller->JulietteRef->GetActorForwardVector();
		
		const FVector target = player_location + f_OffsetLeftDistance*player_leftVector + f_OffsetLeftDistance*player_FrontVector; //Sets Romeo position next to Juliette
		//move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, target);

		//finish with success
		FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}


}
