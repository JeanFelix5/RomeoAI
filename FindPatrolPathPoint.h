// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "FindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJ_AUTRE_CPP_API UFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UFindPatrolPathPoint(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
};
