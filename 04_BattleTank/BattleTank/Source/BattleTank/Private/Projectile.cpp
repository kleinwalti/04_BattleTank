// Fill out your copyright notice in the Description page of Project Settings.

#include "Particles/ParticleSystemComponent.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Adds a Projectile Movement Component to the Projectile-Class, so that the editor can not delete it accidently in blueprint
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->bAutoActivate = false;

	// Adds a Static Mesh 'CollisionMesh' as a component to the Projectile Class
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh"));
	// Forces the Collision Mesh to be the root component of the Projectile Class
	SetRootComponent(CollisionMesh);
	// Defaults the Details->Collision->'Simulation Generats Hit Events' Setting to true
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	// Defaults the Details->Rendering->Visible Setting to false (because usually we only see the smoke of the projectile, because it is so fast -.-)
	CollisionMesh->SetVisibility(false);

	// Adds a Particle System 'Launch Blast' as a component to the Projectile Class
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Launch Blast"));
	// Forces the LaunchBlast to be attached to the Collision Mesh
	LaunchBlast->AttachTo(CollisionMesh);


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovementComponent->Activate();
}