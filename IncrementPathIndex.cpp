// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPathIndex.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"
#include "blackboard_keys.h"

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& object_initializer) {
    NodeName = TEXT("Increment Path Index");
}


EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    //get the AI controller
    ANPC_AIController* const controller = Cast<ANPC_AIController>(owner_comp.GetAIOwner());

    ANPC* const npc = Cast<ANPC>(controller->GetPawn());
    int const no_of_points = npc->get_patrol_path()->num();
    int const min_index = 0;
    int const max_index = no_of_points - 1;


    //get and set the blackboard index key
    int index = controller->get_blackboard()->GetValueAsInt(bb_keys::patrol_path_index);

    if (index >= max_index && direction == EDirectionType::Forward) {

        //check if is looping path
        bool bIsLooping = npc->get_patrol_path()->getIsLoopingPath();
        if (bIsLooping == false) {   //if bIsLooping == false, wait at the last index
            direction = EDirectionType::Wait; 
        }
        else { //if bIsLooping == true, keep looping the path
            direction = EDirectionType::Forward;
        }
    }
    else if (index == min_index && direction == EDirectionType::Wait) {
        direction = EDirectionType::Forward;
    }
    
    //Here if the current direction is set to forward the index will increment to the next waypoint. For the purpose of this project, I made the AI loop through the same path order for ever. 
    // But I could implement the wait behavior at the end of the path if the team wanted. By simply not incrementing the index if the direction is set to wait. 
    controller->get_blackboard()->SetValueAsInt(bb_keys::patrol_path_index, (direction == EDirectionType::Forward ? std::abs(++index) : std::abs(index)) % no_of_points); 

    //finish with success
    FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

