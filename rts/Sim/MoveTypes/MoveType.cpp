/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include <cassert>


#include "MoveType.h"
#include "Map/Ground.h"
#include "Sim/Misc/LosHandler.h"
#include "Sim/Misc/ModInfo.h"
#include "Sim/Misc/QuadField.h"
#include "Sim/Misc/RadarHandler.h"
#include "Sim/Units/Unit.h"
#include "Sim/Units/UnitDef.h"
#include "System/myMath.h"

CR_BIND_DERIVED_INTERFACE(AMoveType, CObject);
CR_REG_METADATA(AMoveType, (
	CR_MEMBER(owner),
	CR_MEMBER(goalPos),
	CR_MEMBER(oldPos),
	CR_MEMBER(oldSlowUpdatePos),

	CR_MEMBER(maxSpeed),
	CR_MEMBER(maxSpeedDef),
	CR_MEMBER(maxWantedSpeed),
	CR_MEMBER(repairBelowHealth),

	CR_MEMBER(useHeading),
	CR_ENUM_MEMBER(progressState),
	CR_RESERVED(32)
));

AMoveType::AMoveType(CUnit* owner):
	owner(owner),

	goalPos(owner? owner->pos: ZeroVector),
	oldPos(owner? owner->pos: ZeroVector),
	oldSlowUpdatePos(oldPos),

	useHeading(true),

	progressState(Done),

	maxSpeed(owner->unitDef->speed / GAME_SPEED),
	maxSpeedDef(owner->unitDef->speed / GAME_SPEED),
	maxWantedSpeed(owner->unitDef->speed / GAME_SPEED),
#if STABLE_UPDATE && DEBUG_STABLE_UPDATE
	stableIsSkidding(false),
	stableIsFlying(false),
	stableGoalPos(owner? owner->pos: ZeroVector),
	stableProgressState(Done),
#endif
	repairBelowHealth(0.3f)
{
	StableInit(modInfo.asyncPathFinder);
}



void AMoveType::SlowUpdate()
{
	if (owner->pos != oldSlowUpdatePos) {
		oldSlowUpdatePos = owner->pos;

		const int newMapSquare = ground->GetSquare(owner->pos);
		const float losHeight = owner->losHeight;
		const float radarHeight = owner->radarHeight;
		const bool isAirMoveType = !owner->usingScriptMoveType && owner->unitDef->canfly;

		if (newMapSquare != owner->mapSquare) {
			owner->mapSquare = newMapSquare;

			if (isAirMoveType) {
				// temporarily set LOS- and radar-height to current altitude for aircraft
				owner->losHeight = (owner->pos.y - ground->GetApproximateHeight(owner->pos.x, owner->pos.z)) + 5.0f;
				owner->radarHeight = owner->losHeight;
			}

			owner->QueUpdateLOS();
			owner->QueUpdateRadar();

			if (isAirMoveType) {
				owner->losHeight = losHeight;
				owner->radarHeight = radarHeight;
			}
		}

		owner->QueUpdateQuad();
	}
}

void AMoveType::KeepPointingTo(CUnit* unit, float distance, bool aggressive)
{
	ASSERT_SINGLETHREADED_SIM();
	KeepPointingTo(float3(unit->pos), distance, aggressive);
}



bool AMoveType::WantsRepair() const { return (owner->health      < (repairBelowHealth * owner->maxHealth)); }
bool AMoveType::WantsRefuel() const { return (owner->currentFuel < (repairBelowHealth * owner->unitDef->maxFuel)); }

#if STABLE_UPDATE
void AMoveType::StableSlowUpdate() {
	stableIsSkidding = IsSkidding();
	stableIsFlying = IsFlying();
	stableGoalPos = goalPos;
	stableProgressState = progressState;
}

void AMoveType::StableUpdate(bool slow) {
	if (slow)
		StableSlowUpdate();
}

void AMoveType::StableInit(bool stable) {
	if (stable) {
		pStableIsSkidding = &AMoveType::IsSkiddingStable;
		pStableIsFlying = &AMoveType::IsFlyingStable;
		pStableGoalPos = &stableGoalPos;
		pStableProgressState = &stableProgressState;
	} else {
		pStableIsSkidding = &AMoveType::IsSkidding;
		pStableIsFlying = &AMoveType::IsFlying;
		pStableGoalPos = &goalPos;
		pStableProgressState = &progressState;
	}
}

bool AMoveType::IsSkiddingStable() const { return stableIsSkidding; }
bool AMoveType::IsFlyingStable() const { return stableIsFlying; }
#endif
