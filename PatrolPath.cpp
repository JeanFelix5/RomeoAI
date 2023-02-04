// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create our Components and setup default values
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->SetupAttachment(Root);

}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();

	CurrentIndex = 0;
	nb_points = this->patrol_points.Num();   
	CollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &APatrolPath::onBeginOverlap);
}

FVector APatrolPath::get_patrol_point(int const index) const
{
	return patrol_points[index];
}

int APatrolPath::num() const
{
	return patrol_points.Num();
}

bool APatrolPath::getIsLoopingPath()
{
	return bLoopingPath;
}

void APatrolPath::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	npc = Cast<AActiveCharacter>(OtherActor);

	if (npc) {
		
		controller = Cast<ANPC_AIController>(npc->GetController());
		if (controller) {
			//Spawn the sphere collision at each waypoint location
			nb_points = this->patrol_points.Num();
			CurrentIndex = controller->get_blackboard()->GetValueAsInt(bb_keys::patrol_path_index); //Make the current index equal to the current index in behavior tree (bb_key value)
			++CurrentIndex;

			if (CurrentIndex >= nb_points) {
				if (bLoopingPath == false) {
					return;
				}
				else {
					CurrentIndex = 0;

				}
			}
		
			CollisionSphere->SetWorldLocation((GetActorLocation()) + (patrol_points[CurrentIndex]));
		
			//update path index if is holding hand while entering the collision
			if (controller) {

				if (npc) { //verify again if the npc is not null or else it could crash
					bool holdingHands = npc->GetIsHoldingHand();

					if (holdingHands == true) {
						
						controller->get_blackboard()->SetValueAsInt(bb_keys::patrol_path_index, CurrentIndex);
						
					}
				}		
			}
		}
	}
}

void APatrolPath::ResetCollisionSphereLocation()
{
	CollisionSphere->SetWorldLocation((GetActorLocation()) + (patrol_points[0]));
}

void APatrolPath::ResetWaypointIndex()
{
	if (npc) {
		if (controller) {
			controller->get_blackboard()->SetValueAsInt(bb_keys::patrol_path_index, 0);
		}
	}
}




