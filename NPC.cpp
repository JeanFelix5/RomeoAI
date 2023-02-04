// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "UEProj_autre_cppCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ANPC::ANPC()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f); 

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = npc_WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 600.f;

	//To better follow Juliette when Jumping
	GetCharacterMovement()->AirControl = 1.0f;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	AUEProj_autre_cppCharacter* JulietteRef = Cast<AUEProj_autre_cppCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (JulietteRef != nullptr)
	{
		JulietteRef->OnHandHoldJump.AddUniqueDynamic(this, &ANPC::JumpingBroadcastHandler);
		JulietteRef->OnHandHoldCrouch.AddUniqueDynamic(this, &ANPC::CrouchingBroadcastHandler);
	}
}

void ANPC::JumpingBroadcastHandler(bool bIsStartingJump)
{
	if (!bisHoldingHand || getIsWinter() == true)	//If is during the winter season, can't jump while holding hands
		return;

	if (bIsStartingJump)
	{
		AActiveCharacter::StartJumping();
		this->GetCharacterMovement()->AirControl = 1.0;
	}	
	else AActiveCharacter::StopJumping();
	
}

void ANPC::CrouchingBroadcastHandler(bool bIsStarting)
{
	if (!bisHoldingHand || this->getIsWinter() == true)
		return;

	else if (bIsStarting)
		ANPC::StartCrouching();

	else ANPC::StopCrouching();
}

void ANPC::StartCrouching()
{
	AUEProj_autre_cppCharacter* JulietteRef = Cast<AUEProj_autre_cppCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector distanceToJuliet = JulietteRef->GetActorLocation() - this->GetActorLocation();
	if (distanceToJuliet.Size() <= 50)
		this->SetActorLocation(this->GetActorLocation() - (distanceToJuliet.Normalize()*20));

	GetCapsuleComponent()->SetCapsuleHalfHeight(42.0f);
	GetCharacterMovement()->MaxWalkSpeed = npc_CrouchSpeed;
	AActiveCharacter::StartCrouching();
	
}

void ANPC::StopCrouching()
{
	FHitResult hit;
	FCollisionQueryParams params;
	if (GetWorld()->LineTraceSingleByChannel(hit, this->GetActorLocation(), this->GetActorLocation() + (GetActorUpVector() * 150), ECC_Visibility, params))
		return;

	GetCapsuleComponent()->SetCapsuleHalfHeight(82.0f);
	GetCharacterMovement()->MaxWalkSpeed = npc_WalkSpeed;
	AActiveCharacter::StopCrouching();
	
}

APatrolPath* ANPC::get_patrol_path()
{
	return patrol_path;
}

void ANPC::set_patrol_path(APatrolPath* nextPatrolPath)
{
	patrol_path = nextPatrolPath;
}

void ANPC::SetMaxSpeed(float maxSpeed)
{
	this->GetCharacterMovement()->MaxWalkSpeed = maxSpeed;
}

