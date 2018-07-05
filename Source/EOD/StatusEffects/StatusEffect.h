// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS(Abstract)
class EOD_API UStatusEffect : public UObject
{
	GENERATED_BODY()
	
public:

	UStatusEffect();

	/** 
	 * If the owner of status effect received any damage 
	 * @param HittingCharacter The enemy character that landed a hit on you
	 */
	// virtual void OnReceivingHit(ABaseCharacter* HittingCharacter) PURE_VIRTUAL(UStatusEffect::OnGettingHit, );

	/**
	 * If the owning character successfully hits the enemy
	 * @param HitCharacter The enemy character that got hit
	 */
	// virtual void OnSuccessfulHit(ABaseCharacter* HitCharacter) PURE_VIRTUAL(UStatusEffect::OnSuccessfulHit, );

	/** 
	 * If the owning character lands a critical hit on enemy
	 * @param HitCharacter The enemy character that got hit
	 */
	// virtual void OnCriticalHit(ABaseCharacter* HitCharacter) PURE_VIRTUAL(UStatusEffect::OnCriticalHit, );
	
	/** If the owning character fails to hit the enemy */
	// virtual void OnUnsuccessfulHit() PURE_VIRTUAL(UStatusEffect::OnUnsuccessfulHit, );
	
	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Initiator The actor that initiated the status effect. Can be nullptr. For elemental effects this would be a weapon.
	 */
	virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Initiator) PURE_VIRTUAL(UStatusEffect::OnInitialize, );

	/** Called to deinitialize this status effect on a character */
	virtual void OnDeinitialize() PURE_VIRTUAL(UStatusEffect::OnDeinitialize, );

	/** Called when the status effect is activated */
	virtual void OnActivation(TArray<ABaseCharacter*> RecipientCharacters) PURE_VIRTUAL(UStatusEffect::OnActivation, );

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation() PURE_VIRTUAL(UStatusEffect::OnDeactivation, );

	ABaseCharacter* GetOwningCharacter() const;

	void SetOwningCharacter(ABaseCharacter* NewCharacter);

protected:

	/** True if the status effect triggers on Owner receiving damage */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnReceivingHit : 1;

	/** True if the status effect triggers on Owner successfully landing an attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulHit : 1;

	/** True if the status effect triggers on Owner failing to land an attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnUnsuccessfulHit : 1;

	/** True if the status effect triggers on Owner successfully landing a critical attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnCriticalHit : 1;

	/** True if the status effect triggers on Owner successfully dodging an incoming enemy attack */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulDodge : 1;

	/** True if the status effect triggers on Owner successfully blocking an incoming enemy attack */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulBlock : 1;

	/** True if the status effect triggers on Owner regenerating full health */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnFullHealth : 1;

	/** True if the status effect triggers on Owner hitting 'low health' mark */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnLowHealth : 1;

	/** True if the status effect triggers on Owner entering combat */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnEnteringCombat : 1;

	/** True if the status effect triggers on Owner leaving combat */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnLeavingCombat : 1;

	//~ @note Redundant property but it could be useful in certain situations
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnInitialization : 1;

	// @todo add buffs/debuffs that activate on getting hit by another spell, buff, etc.

private:

	UPROPERTY(Transient)
	ABaseCharacter* OwningCharacter;
	
	// @todo Add flags to determine allies and enemies (for buff and debuff effects)
	// @note better to handle allies from ABaseCharacter class and add/use a function like TArray<ABaseCharacter*> GetAllies();
	// int AllyFlag;
	// @todo add boolean to check if this status effects deactives on death
	// @todo add boolean to determine whether the status effect is currently active or not
	
	
};
