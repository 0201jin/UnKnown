// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonActor.h"
#include "Engine.h"

// Sets default values
ADungeonActor::ADungeonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Modeling/TilingMat.TilingMat'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> WallMat(TEXT("Material'/Game/Modeling/WallMet.WallMet'"));
	
	Mater = Mat.Object;
	WallMater = WallMat.Object;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;

	Mesh->SetStaticMesh(Mesh1.Object);

	Mesh->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
}

void ADungeonActor::SetMaterial()
{
	Mesh->SetMaterial(0, Mater);
}

void ADungeonActor::SetMatInst(int size, UTexture * tex, UTexture * normal)
{
	UMaterialInstanceDynamic * Instance = UMaterialInstanceDynamic::Create(Mater, this);
	Instance->SetScalarParameterValue(TEXT("U"), size);
	Instance->SetScalarParameterValue(TEXT("V"), size);
	Instance->SetTextureParameterValue(TEXT("Base_Tex"), tex);
	Instance->SetTextureParameterValue(TEXT("Normal_Tex"), normal);
	Mesh->SetMaterial(0, Instance);
}

void ADungeonActor::SetWallMaetInst(UTexture * tex)
{
	UMaterialInstanceDynamic * Instance = UMaterialInstanceDynamic::Create(WallMater, this);
	Instance->SetTextureParameterValue(TEXT("Base_Tex"), tex);
	Mesh->SetMaterial(0, Instance);
}
