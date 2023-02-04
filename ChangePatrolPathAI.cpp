// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangePatrolPathAI.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AChangePatrolPathAI::AChangePatrolPathAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
}

// Called when the game starts or when spawned
void AChangePatrolPathAI::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AChangePatrolPathAI::onBeginOverlap);
}

void AChangePatrolPathAI::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	npc = Cast<ANPC>(OtherActor);
	if (npc) {

		if (Next_patrol_path) {
			npc->set_patrol_path(Next_patrol_path);
			
			controller = Cast<ANPC_AIController>(npc->GetController());
			if (controller) {
				//set the current index to 0 so the AI can start at the first waypoint in the next patrol path.
				//otherwise the AI would start the path at the index he was currently going in the previous path
				controller->get_blackboard()->SetValueAsInt(bb_keys::patrol_path_index, 0);
			}
		}
	}

}