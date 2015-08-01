#ifndef _MAIN_H
#define _MAIN_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


//RTTMS STRUCTURES
struct BT_RTTMS_STRUCT {
	unsigned short VertexCount;
	float* Vertices;
	bool ChangedAVertex;
	unsigned short FirstUpdatedVertex;
	unsigned short LastUpdatedVertex;
	void* Internals;
};



//BLITZTERRAIN STRUCTURES
struct BT_TerrainInfo {
	bool Built;
	bool Generated;
	unsigned long Texture;
	unsigned long Detailmap;
	bool Smoothing;
	bool QuadReduction;
	bool QuadRotation;
	unsigned char LODLevels;
	float Scale;
	float YScale;
	float Tile;
	bool MeshOptimisation;
	unsigned short Sectors;
	unsigned short Heightmapsize;
	float TerrainSize;
	bool MultiDetailmapping;
	unsigned long DetailMapCount;
	bool Exclusion;
	unsigned char ExclusionThreshold;
	unsigned char ATMode;
	unsigned char DetailBlendMode;
	void* InternalData;
	//Added in 2.2
	unsigned long SmoothAmount;
	void* DBPObjectPtr;
};
struct BT_LODLevelInfo {
	float SectorSize;
	unsigned char SectorDetail;
	unsigned short Sectors;
	float Distance;
	unsigned short Split;
	unsigned char TileSpan;
};
struct BT_SectorInfo {
	bool Excluded;
	unsigned short Row;
	unsigned short Column;
	float Pos_x;
	float Pos_y;
	float Pos_z;
	void *WorldMatrix;
};



//UPDATE HANDLER TYPE
typedef void (*BT_RTTMS_UpdateHandler_t)(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID, unsigned short StartVertex, unsigned short EndVertex, float* Vertices);


//FUNCTIONS
#ifdef COMPILE_GDK
	#include <DarkGDK.h>
	#include <BlitzTerrain.h>
	#define EXPORT

void* BT_RTTMS_LockSectorVertexData(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
void BT_RTTMS_AddUpdateHandler(unsigned long TerrainID, BT_RTTMS_UpdateHandler_t UpdateHandler);
void* BT_GetTerrainInfo(unsigned long TerrainID);
void* BT_GetLODLevelInfo(unsigned long TerrainID, unsigned long LODLevelID);
void* BT_GetSectorInfo(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
#else
	#define EXPORT __declspec(dllexport)

//From DBPro
typedef long (*t_dbGetImageWidth)(long);
typedef long (*t_dbGetImageHeight)(long);
#endif


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
