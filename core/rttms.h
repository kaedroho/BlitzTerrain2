#ifndef _RTTMS_H
#define _RTTMS_H

#include "main.h"

struct BT_RTTMS_STRUCT {
	// Counts
	unsigned short VertexCount;

	// Data
	float* Vertices;

	// Updated vertices
	bool ChangedAVertex;
	unsigned short FirstUpdatedVertex;
	unsigned short LastUpdatedVertex;

	// Internals
	void* Internals;
};

struct BT_RTTMS_STRUCTINTERNALS {
	// Terrain, LODLevel and Sector
	unsigned long TerrainID;
	unsigned long LODLevelID;
	unsigned long SectorID;

	// Sector pointer
	s_BT_Sector* SectorPtr;

	// Delete meshdata
	bool DeleteMeshData;
};

typedef void (*BT_RTTMS_UpdateHandler_t)(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID, unsigned short StartVertex, unsigned short EndVertex, float* Vertices);

EXPORT void BT_RTTMS_AddUpdateHandler(unsigned long TerrainID, BT_RTTMS_UpdateHandler_t UpdateHandler);
static void BT_RTTMS_CallUpdateHandlers(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID, unsigned short StartVertex, unsigned short EndVertex, float* Vertices);
void BT_RTTMS_DeleteUpdateHandlers(unsigned long TerrainID);
EXPORT void* BT_RTTMS_LockSectorVertexData(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
EXPORT void BT_RTTMS_UnlockSectorVertexData(void* StructPtr);
EXPORT unsigned short BT_RTTMS_FindVertex(void* StructPtr, unsigned short Vrow, unsigned short Vcol);
struct s_BT_terrain;
void BT_RTTMS_UnlockTerrain(s_BT_terrain* TerrainPtr);


// Defined in rttms/rttmsmain.cpp
EXPORT void BT_LockVertexdataForSector(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
EXPORT unsigned long BT_LockedASector();
EXPORT unsigned long BT_GetLockedTerrain();
EXPORT unsigned long BT_GetLockedSector();
EXPORT void BT_UnlockVertexData();
EXPORT unsigned short BT_GetVertexCount();
EXPORT unsigned long BT_GetIndexCount();
EXPORT unsigned short BT_GetIndex(unsigned long IndexID);
#ifdef COMPILE_GDK
float BT_GetVertexPositionX(unsigned short VertexID);
float BT_GetVertexPositionX(unsigned short Vrow, unsigned short Vcol);
float BT_GetVertexPositionY(unsigned short VertexID);
float BT_GetVertexPositionY(unsigned short Vrow, unsigned short Vcol);
float BT_GetVertexPositionZ(unsigned short VertexID);
float BT_GetVertexPositionZ(unsigned short Vrow, unsigned short Vcol);
#else
EXPORT unsigned long BT_GetVertexPositionX(unsigned short VertexID);
EXPORT unsigned long BT_GetVertexPositionX(unsigned short Vrow, unsigned short Vcol);
EXPORT unsigned long BT_GetVertexPositionY(unsigned short VertexID);
EXPORT unsigned long BT_GetVertexPositionY(unsigned short Vrow, unsigned short Vcol);
EXPORT unsigned long BT_GetVertexPositionZ(unsigned short VertexID);
EXPORT unsigned long BT_GetVertexPositionZ(unsigned short Vrow, unsigned short Vcol);
#endif
EXPORT void BT_SetVertexHeight(unsigned short VertexID, float Height);
EXPORT void BT_SetVertexHeight(unsigned short Vrow, unsigned short Vcol, float Height);

#endif
