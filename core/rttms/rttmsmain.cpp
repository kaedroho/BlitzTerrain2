#include "../rttms.h"

void* BT_RTTMS_LockSectorVertexData(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
void BT_RTTMS_AddUpdateHandler(unsigned long TerrainID, BT_RTTMS_UpdateHandler_t UpdateHandler);
void* BT_GetTerrainInfo(unsigned long TerrainID);
void* BT_GetLODLevelInfo(unsigned long TerrainID, unsigned long LODLevelID);
void* BT_GetSectorInfo(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
unsigned long BT_TerrainExist(unsigned long TerrainID);
unsigned short BT_RTTMS_FindVertex(void* Struct, unsigned short row, unsigned short col);

static bool gBT_LockedASector = false;
static BT_RTTMS_STRUCT* gBT_SectorRTTMS = NULL;
static BT_TerrainInfo* gBT_LockedTerrainInfo = NULL;
static BT_LODLevelInfo* gBT_LockedLODLevelInfo = NULL;
static BT_SectorInfo* gBT_LockedSectorInfo = NULL;
static unsigned long gBT_LockedTerrainID = 0;
static unsigned long gBT_LockedLODLevelID = 0;
static unsigned long gBT_LockedSectorID = 0;

EXPORT void BT_LockVertexdataForSector(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID)
{
	// Check that vertexdata isnt already locked
	if (gBT_LockedASector == false) {
		// Lock the vertexdata (this command is very small, all it does is set a locked flag and returns the RTTMS structure)
		gBT_SectorRTTMS = (BT_RTTMS_STRUCT*)BT_RTTMS_LockSectorVertexData(TerrainID, LODLevelID, SectorID);
		if (gBT_SectorRTTMS == NULL)
			return;

		// Set globals
		gBT_LockedASector = true;
		gBT_LockedTerrainID = TerrainID;
		gBT_LockedLODLevelID = LODLevelID;
		gBT_LockedSectorID = SectorID;

		// Get info
		gBT_LockedTerrainInfo = (BT_TerrainInfo*)BT_GetTerrainInfo(TerrainID);
		gBT_LockedLODLevelInfo = (BT_LODLevelInfo*)BT_GetLODLevelInfo(TerrainID, LODLevelID);
		gBT_LockedSectorInfo = (BT_SectorInfo*)BT_GetSectorInfo(TerrainID, LODLevelID, SectorID);
	}
}


EXPORT unsigned long BT_LockedASector()
{
	return gBT_LockedASector;
}


EXPORT unsigned long BT_GetLockedTerrain()
{
	return gBT_LockedTerrainID;
}


EXPORT unsigned long BT_GetLockedLODLevel()
{
	return gBT_LockedLODLevelID;
}


EXPORT unsigned long BT_GetLockedSector()
{
	return gBT_LockedSectorID;
}


EXPORT void BT_UnlockVertexData()
{
	// Check that vertexdata is locked
	if (gBT_LockedASector == true) {
		// Zero everything
		gBT_LockedASector = false;
		gBT_SectorRTTMS = NULL;
		gBT_LockedTerrainID = 0;
		gBT_LockedLODLevelID = 0;
		gBT_LockedSectorID = 0;
	}
}


EXPORT unsigned short BT_GetVertexCount()
{
	if (gBT_LockedASector == true) {
		return gBT_SectorRTTMS->VertexCount;
	} else {
		return 0;
	}
}


#ifdef COMPILE_GDK


float BT_GetVertexPositionY(unsigned short VertexID)
{
	if (gBT_LockedASector == true) {
		if (gBT_SectorRTTMS->VertexCount > VertexID)
			return gBT_SectorRTTMS->Vertices[VertexID];
	} else {
		return 0.0f;
	}
	return 0.0f;
}

float BT_GetVertexPositionY(unsigned short Vrow, unsigned short Vcol)
{
	if (gBT_LockedASector == true) {
		unsigned short VertexID = BT_RTTMS_FindVertex((void*)gBT_SectorRTTMS, Vrow, Vcol);
		if (VertexID != NULL)
			return BT_GetVertexPositionY(VertexID - 1);
	} else {
		return 0.0f;
	}
	return 0.0f;
}


#else


EXPORT unsigned long BT_GetVertexPositionY(unsigned short VertexID)
{
	if (gBT_LockedASector == true) {
		if (gBT_SectorRTTMS->VertexCount > VertexID)
			return *(unsigned long*)&gBT_SectorRTTMS->Vertices[VertexID];
	} else {
		return 0;
	}
	return 0;
}

EXPORT unsigned long BT_GetVertexPositionY(unsigned short Vrow, unsigned short Vcol)
{
	if (gBT_LockedASector == true) {
		unsigned short VertexID = BT_RTTMS_FindVertex((void*)gBT_SectorRTTMS, Vrow, Vcol);
		if (VertexID != NULL)
			return BT_GetVertexPositionY(VertexID - 1);
	} else {
		return 0;
	}
	return 0;
}
#endif


EXPORT void BT_SetVertexHeight(unsigned short VertexID, float Height)
{
	if (gBT_LockedASector == true) {
		if (gBT_SectorRTTMS->VertexCount + 1 > VertexID) {
			// Set position
			gBT_SectorRTTMS->Vertices[VertexID] = Height;

			// Set updated vertices
			if (gBT_SectorRTTMS->ChangedAVertex == false) {
				gBT_SectorRTTMS->FirstUpdatedVertex = gBT_SectorRTTMS->LastUpdatedVertex = VertexID;
				gBT_SectorRTTMS->ChangedAVertex = true;
			} else {
				if (VertexID < gBT_SectorRTTMS->FirstUpdatedVertex)
					gBT_SectorRTTMS->FirstUpdatedVertex = VertexID;
				if (VertexID > gBT_SectorRTTMS->LastUpdatedVertex)
					gBT_SectorRTTMS->LastUpdatedVertex = VertexID;
			}
		}
	}
}

EXPORT void BT_SetVertexHeight(unsigned short Vrow, unsigned short Vcol, float Height)
{
	if (gBT_LockedASector == true) {
		unsigned short VertexID = BT_RTTMS_FindVertex((void*)gBT_SectorRTTMS, Vrow, Vcol);
		if (VertexID != NULL)
			BT_SetVertexHeight(VertexID - 1, Height);
	}
}
