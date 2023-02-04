#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "NPC.h"
#include "HoldHands.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class UEPROJ_AUTRE_CPP_API UHoldHands : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UHoldHands(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;



protected:
	//This value is the max distance between actors before the hand holding breaks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = "true"))
		float f_MaxDistanceToHoldHands = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = "true"))
		FVector v_OffsetTarget = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = "true"))
		float f_OffsetDistance = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = "true"))
		float f_OffsetLeftDistance = 74.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_WaitInPlace = false;



};
