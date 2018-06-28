// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "PrimaryWeapon.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API APrimaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APrimaryWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEquip(ABaseWeapon* LastWeapon);

	virtual void OnUnEquip();
	
	
	
};
