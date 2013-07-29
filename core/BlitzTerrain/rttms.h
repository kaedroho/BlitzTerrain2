#ifndef _RTTMS_H
#define _RTTMS_H

#include "main.h"

struct BT_RTTMS_VERTEX
{
//Position
	float Pos_x,Pos_y,Pos_z;

//Normals
	float Nrm_x,Nrm_y,Nrm_z;

//UVs
	float U0,V0,U1,V1;
};

struct BT_RTTMS_STRUCT
{
//Counts
	unsigned short VertexCount;
	unsigned long IndexCount;

//Data
	BT_RTTMS_VERTEX* Vertices;
	unsigned short* Indices;

//Updated vertices
	bool ChangedAVertex;
	unsigned short FirstUpdatedVertex;
	unsigned short LastUpdatedVertex;

//Internals
	void* Internals;
};

struct BT_RTTMS_STRUCTINTERNALS
{
//Terrain, LODLevel and Sector
	unsigned long TerrainID;
	unsigned long LODLevelID;
	unsigned long SectorID;

//Sector pointer
	s_BT_Sector* SectorPtr;

//Delete meshdata
	bool DeleteMeshData;
};

typedef void(*BT_RTTMS_UpdateHandler_t)(unsigned long TerrainID,unsigned long LODLevelID,unsigned long SectorID,unsigned short StartVertex,unsigned short EndVertex,void* Vertices);

EXPORT void BT_RTTMS_AddUpdateHandler(unsigned long TerrainID,BT_RTTMS_UpdateHandler_t UpdateHandler);
static void BT_RTTMS_CallUpdateHandlers(unsigned long TerrainID,unsigned long LODLevelID,unsigned long SectorID,unsigned short StartVertex,unsigned short EndVertex,BT_RTTMS_VERTEX* Vertices);
void BT_RTTMS_DeleteUpdateHandlers(unsigned long TerrainID);
EXPORT void* BT_RTTMS_LockSectorVertexData(unsigned long TerrainID,unsigned long LODLevelID,unsigned long SectorID);
EXPORT void BT_RTTMS_UnlockSectorVertexData(void* StructPtr);
EXPORT unsigned short BT_RTTMS_FindVertex(void* StructPtr,unsigned short Vrow,unsigned short Vcol);
struct s_BT_terrain;
void BT_RTTMS_UnlockTerrain(s_BT_terrain* TerrainPtr);

#endif