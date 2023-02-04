// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitAI.h"

// Sets default values
AWaitAI::AWaitAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
}

// Called when the game starts or when spawned
void AWaitAI::BeginPlay()
{
	Super::BeginPlay(); 
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaitAI::onBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AWaitAI::onEndOverlap);
}

void AWaitAI::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	npc = Cast<ANPC>(OtherActor);
	if (npc) {
		controller = Cast<ANPC_AIController>(npc->GetController());
		controller->OnBeginWaiting(f_Time);
	}
	
	juliette = Cast<AUEProj_autre_cppCharacter>(OtherActor);
	if (juliette) {
		if (controller) {	
			controller->OnEndWaiting(0.0f);
		}
		this->Destroy();
	}
}

void AWaitAI::onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	npc = Cast<ANPC>(OtherActor);
	if (npc) {
		this->Destroy();
	}
}
