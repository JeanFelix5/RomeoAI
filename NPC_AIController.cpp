// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UEProj_autre_cppCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h" 
#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"


ANPC_AIController::ANPC_AIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/AI/NPC_BT.NPC_BT'"));
	if (obj.Succeeded())
	{
		behavior_tree = obj.Object;
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
	setup_perception_system();
}

void ANPC_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(behavior_tree);
	behavior_tree_component->StartTree(*behavior_tree);

	//The following lines change the movement mode of the NPC character
	npc = Cast<ANPC>(GetCharacter());

	if (npc)
	{
		npc->MovementModeChangedDelegate.AddUniqueDynamic(this, &ANPC_AIController::OnMovementModeChanged);
		npc->LandedDelegate.AddUniqueDynamic(this, &ANPC_AIController::OnLanded);
	}
	InitializeJulietteRef();
}

void ANPC_AIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard) {
		(blackboard->InitializeBlackboard(*behavior_tree->BlackboardAsset));
	}
		
}

UBlackboardComponent* ANPC_AIController::get_blackboard() const
{
	return blackboard;
}

FVector ANPC_AIController::GetNPCLocation()
{
	FVector npc_location;
	if (npc) {
		npc_location =  npc->GetActorLocation();
	}
	return npc_location;
}

void ANPC_AIController::on_updated(TArray<AActor*> const& updated_actors)
{
	
}

void ANPC_AIController::setup_perception_system()
{
	//create and initialise sight configuration object
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 700.0f; 
	sight_config->LoseSightRadius = sight_config->SightRadius + 25.0f;
	sight_config->PeripheralVisionAngleDegrees = 360.0f; //normally set at 90.0f
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	// add sight configuration component to perception component
	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}

void ANPC_AIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AUEProj_autre_cppCharacter>(actor)) {
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}

}

//CODE FOUND IN A OPEN SOURCE BLOG
//THE CODE IS FROM: Vikram Saran  5 November 2017
//FROM: https://www.vikram.codes/blog/ai/02-nav-modifiers-links
//I modified it a little bit for this project
void ANPC_AIController::OnMovementModeChanged(ACharacter* MovedCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	// If the new movement mode is Falling
	if (MovedCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		GetPathFollowingComponent()->Deactivate();
	}
}

//By deactivating and activating it the path following component won't interfere during the jump and won't make the NPC stop mid air.
void ANPC_AIController::OnLanded(const FHitResult& Hit)
{
	GetPathFollowingComponent()->Activate();
	npc->SetisJumping(false); 
}

void ANPC_AIController::OnBeginHoldingHands()
{
	if (!npc->GetIsHoldingHand() && !b_BurntOutHH) {

		GetWorldTimerManager().ClearTimer(localTimerHandle);
		npc->SetHoldingHand(true);

		get_blackboard()->SetValueAsBool(bb_keys::is_holding_hands, true);

		OnHandHoldHandler.Broadcast(true);
		
		//increment hand holding timer
		HandHoldingTimerRepeatable();
	}

}

void ANPC_AIController::StopHoldingHands()
{
	GetWorldTimerManager().ClearTimer(localTimerHandle);

	npc->SetHoldingHand(false);
	get_blackboard()->SetValueAsBool(bb_keys::is_holding_hands, false);
	npc->StopCrouching();	//romeo stop crouching when they stop holding hands
	npc->StopPushing(); //Romeo stop  pushing when not holding hands
	npc->GetCharacterMovement()->AirControl = 0.2;

	OnHandHoldHandler.Broadcast(false);
	//Stop the Rumble
	auto playerController = GetWorld()->GetFirstPlayerController();
	if (npc->rumble && b_IsRumbling) {
		playerController->ClientStopForceFeedback(npc->rumble, HHRumble);
		b_IsRumbling = false;
	}

	//Reset Romeo Speed
	npc->ResetMaxSpeed();

	//Cooldown timer
	HandHoldingCooldownTimerRepeatable();
}

void ANPC_AIController::OnBeginWaiting(float SecondsToWait)
{
	f_WaitingTime = SecondsToWait;
	//Start a timer
	GetWorldTimerManager().SetTimer(localTimerHandleWaiting, this, &ANPC_AIController::TimerRepeatableWaiting, f_timerRefreshDuration, true, 0.0f);
	get_blackboard()->SetValueAsFloat(bb_keys::is_Waiting, SecondsToWait);
	UE_LOG(LogTemp, Warning, TEXT("Waiting depart ")); 
	
}

void ANPC_AIController::OnEndWaiting(float SecondsToWait)
{
	GetWorldTimerManager().ClearTimer(localTimerHandleWaiting);
	f_TimeElapsed = 0.0f;
	get_blackboard()->SetValueAsFloat(bb_keys::is_Waiting, SecondsToWait);
	UE_LOG(LogTemp, Warning, TEXT("Waiting fin"));
}

//Simple rumble version tu use while rumble system is being made
void ANPC_AIController::SimpleRumble()
{
	//Start the Rumble
	auto playerController = GetWorld()->GetFirstPlayerController();
	if (npc->rumble && !b_IsRumbling) {
		playerController->ClientPlayForceFeedback(npc->rumble, false, HHRumble);
		b_IsRumbling = true;
	}
}

void ANPC_AIController::HandHoldingTimerRepeatable()
{
	FTimerDelegate TimerDelegate;

	//Instructions to execute after the timer
	TimerDelegate.BindLambda([&]
	{
			//check if you must start rumbling or stop holding hands
			if (npc)
			{
				if (npc->GetIsHoldingHand())
				{
					f_TimeElapsedHH += f_timerRefreshDuration;

					if (f_TimeElapsedHH >= npc->f_StartRumbleTime)
						SimpleRumble();

					if (f_TimeElapsedHH >= npc->f_HandHoldingDuration)
					{
						//if Romeo is at his limit, the timer is a bit incremented so cooldown is longer
						b_BurntOutHH = true;
						f_TimeElapsedHH = f_TimeElapsedHH * 1.1;
						StopHoldingHands();
						return;
					}
					else
						HandHoldingTimerRepeatable();
				}
			}
			else return;
	});
	/*
	if (GetWorld())
		GetWorld()->GetTimerManager().SetTimer(localTimerHandle, TimerDelegate, f_timerRefreshDuration, false);
	*/
}

void ANPC_AIController::HandHoldingCooldownTimerRepeatable()
{
	FTimerDelegate TimerDelegate;

	//Instructions to execute after the timer
	TimerDelegate.BindLambda([&]
		{
			//at the end of timer, decrement the Hand Holding Timer
			if (npc)
			{
				if (!npc->GetIsHoldingHand())
				{
					f_TimeElapsedHH -= f_timerRefreshDuration * 0.8;

					if (f_TimeElapsedHH >= npc->f_StartRumbleTime)
						SimpleRumble();

					if (f_TimeElapsedHH <= 0.0)
					{
						//clamp the time elapsed at 0, make Romeo able to be held again and stop the timer
						f_TimeElapsedHH = 0.0;
						b_BurntOutHH = false;
						return;
					}
					else
						HandHoldingCooldownTimerRepeatable();
				}
			}
			else return;
		});
	//set a timer

	/*
	if (GetWorld())
		GetWorld()->GetTimerManager().SetTimer(localTimerHandle, TimerDelegate, f_timerRefreshDuration, false);
	*/
}

void ANPC_AIController::TimerRepeatableWaiting()
{
	if (f_TimeElapsed >= f_WaitingTime) {
		OnEndWaiting(0.0f);
	}
	else {
		f_TimeElapsed += f_timerRefreshDuration;

	}
}

void ANPC_AIController::SetNPCPushStatus(bool status) {
	get_blackboard()->SetValueAsBool(bb_keys::is_pushing, status);

}
