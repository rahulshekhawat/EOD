// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CombatLibrary.h"
#include "Player/BaseCharacter.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// only call from server
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	for (FRaidCapsule& Capsule : CollisionCapsules)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();

		FVector CorrectedBottom = Capsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = Capsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));

		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;
		FVector NormalizedHeightVector = HalfHeightVector.GetSafeNormal();
		FVector Center = CorrectedBottom + HalfHeightVector;

		FRotator CapsuleRotation = UKismetMathLibrary::MakeRotFromZ(HalfHeightVector);

		// Transform Center
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);
		// Transform rotation
		FRotator TransformedRotation = WorldTransform.TransformRotation(CapsuleRotation.Quaternion()).Rotator();
		
		FVector TransformedBottom;
		FVector TransformedTop;

		if (HalfHeightVector.Size() > Capsule.Radius)
		{
			TransformedBottom = TransformedCenter - NormalizedHeightVector * HalfHeightVector.Size();
			TransformedTop = TransformedCenter + NormalizedHeightVector * HalfHeightVector.Size();
		}
		else
		{
			TransformedBottom = TransformedCenter - NormalizedHeightVector * Capsule.Radius;
			TransformedTop = TransformedCenter + NormalizedHeightVector * Capsule.Radius;
		}

#if WITH_EDITOR // draw debug shapes only if inside editor

		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
		
		UKismetSystemLibrary::DrawDebugArrow(MeshComp, TransformedBottom, TransformedTop, 100.f, FLinearColor::Red, 5.f, 2.f);
		
#endif // WITH_EDITOR

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CALLED FROM SERVER"));

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
		TArray<FHitResult> CapsuleHitResults;
		
		FCollisionQueryParams Params;
		// Always ignore self
		Params.AddIgnoredActor(MeshComp->GetOwner());
		Params.bIgnoreTouches = true;
		Params.bIgnoreBlocks = false;
		Params.bReturnFaceIndex = false;
		Params.bReturnPhysicalMaterial = false;
		Params.MobilityType = EQueryMobilityType::Dynamic;
		Params.bTraceComplex = false;
		Params.TraceTag = FName("CollisionQueryForCombat");
		// @todo mask filter for teams.

		bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(CapsuleHitResults, TransformedCenter, TransformedCenter, TransformedRotation.Quaternion(), COLLISION_COMBAT, CollisionShape, Params);
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (BaseCharacter)
		{
			UCombatLibrary::HandleCombatCollision(BaseCharacter, CapsuleHitResults, bHit);
		}
		else
		{
			UCombatLibrary::HandleCombatCollision(MeshComp->GetOwner(), CapsuleHitResults, bHit);
		}
	}
}
