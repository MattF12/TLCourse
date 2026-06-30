// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_HealSelf.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

EBTNodeResult::Type URogueBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);

	URogueActionSystemComponent* ActionComp = Pawn->GetComponentByClass<URogueActionSystemComponent>();
	if (ensure(ActionComp))
	{
		UE_LOG(LogTemp, Log, TEXT("NPC is Healing self. Current HP: %f"), ActionComp->GetHealth());
		
		ActionComp->ApplyHealthChange(HealAmount);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
