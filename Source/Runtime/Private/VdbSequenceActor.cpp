// Copyright 2022 Eidos-Montreal / Eidos-Sherbrooke

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "VdbSequenceActor.h"

#include "VdbSequenceComponent.h"
#include "VdbVolumeSequence.h"

#define LOCTEXT_NAMESPACE "VdbActor"

AVdbSequenceActor::AVdbSequenceActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VdbSequenceComponent = CreateDefaultSubobject<UVdbSequenceComponent>(TEXT("VdbSequenceComponent"));
	RootComponent = VdbSequenceComponent;

	// Force a 90deg rotation to fit with Unreal coordinate system (left handed, z-up)
	FTransform Transform(FRotator(0.0f, 0.0f, -90.0f));
	VdbSequenceComponent->SetWorldTransform(Transform);
}

#if WITH_EDITOR
bool AVdbSequenceActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);

	if (VdbSequenceComponent->VdbSequence)
	{
		Objects.Add(VdbSequenceComponent->VdbSequence.Get());
	}
	return true;
}
#endif

#undef LOCTEXT_NAMESPACE