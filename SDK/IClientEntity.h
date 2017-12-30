#pragma once

enum DrawModelFlags_t
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	// Not a studio flag, but used to flag model as a non-sorting brush model
	STUDIO_TRANSPARENCY = 0x80000000,
	// Not a studio flag, but used to flag model as using shadow depth material override
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000,
};

struct bf_read;
using CBaseHandle = unsigned long;

class ICollideable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

enum Collision_Group_t
{
	COLLISION_GROUP_NONE = 0,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
									  // TF2, this filters out other players and CBaseObjects
									  COLLISION_GROUP_NPC,			// Generic NPC group
									  COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
									  COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
									  COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
									  COLLISION_GROUP_PROJECTILE,		// Projectiles!
									  COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
									  COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
									  COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
									  COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

									  COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
									  COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other
									  COLLISION_GROUP_PZ_CLIP,



									  COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE, // Only collides with bullets

									  LAST_SHARED_COLLISION_GROUP
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable*			GetCollideable() = 0;
	virtual IClientNetworkable*		GetClientNetworkable() = 0;
	virtual IClientRenderable*		GetClientRenderable() = 0;
	virtual IClientEntity*			GetIClientEntity() = 0;
	virtual C_BaseEntity*			GetBaseEntity() = 0;
	virtual IClientThinkable*		GetClientThinkable() = 0;
	//virtual IClientModelRenderable*	GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*	GetClientAlphaProperty() = 0;
};

class IClientThinkable
{
public:
	virtual ~IClientThinkable() {};
};
class IClientRenderable
{
public:
	virtual ~IClientRenderable() {};
	int DrawModel( DrawModelFlags_t flags, uint8_t alpha );
};

class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;
	virtual void			NotifyShouldTransmit( int state ) = 0;
	virtual void			OnPreDataChanged( int updateType ) = 0;
	virtual void			OnDataChanged( int updateType ) = 0;
	virtual void			PreDataUpdate( int updateType ) = 0;
	virtual void			PostDataUpdate( int updateType ) = 0;
	virtual void			OnDataUnchangedInPVS() = 0;
	virtual bool			IsDormant() = 0;
	virtual int				EntIndex() const = 0;
	virtual void			ReceiveMessage( int classID, bf_read& msg ) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual ~IClientEntity() {};
};