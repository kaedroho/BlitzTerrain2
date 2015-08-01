#include "../rttms.h"
#include "../dbp/DBOData.h"

#pragma comment(lib,"d3dx9.lib")

void* BT_RTTMS_LockSectorVertexData(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
void  BT_RTTMS_AddUpdateHandler(unsigned long TerrainID, BT_RTTMS_UpdateHandler_t UpdateHandler);
void* BT_GetTerrainInfo(unsigned long TerrainID);
void* BT_GetLODLevelInfo(unsigned long TerrainID, unsigned long LODLevelID);
void* BT_GetSectorInfo(unsigned long TerrainID, unsigned long LODLevelID, unsigned long SectorID);
unsigned long BT_TerrainExist(unsigned long TerrainID);

#ifdef COMPILE_GDK
float BT_GetGroundHeight(unsigned long TerrainID, float X, float Z);
#else
unsigned long BT_GetGroundHeight(unsigned long TerrainID, float X, float Z);
#endif

#ifndef COMPILE_GDK
extern t_dbGetImageWidth dbGetImageWidth;
extern t_dbGetImageHeight dbGetImageHeight;
#endif


EXPORT void BT_PaintTerrain(unsigned long TerrainID, float X, float Z, float Radius, unsigned long Colour)
{
//Check that the terrain exists
	if (BT_TerrainExist(TerrainID)) {
		//Get terrain info
		BT_TerrainInfo* TerrainInfo = (BT_TerrainInfo*)BT_GetTerrainInfo(TerrainID);

		//Check that the terrain is built
		if (TerrainInfo->Built == true) {
			//Check that the terrain has a texture
			if (TerrainInfo->Texture > 0) {
				//Get DBP Object
				sObject* Object = (sObject*)TerrainInfo->DBPObjectPtr;

				//Get texture
				LPDIRECT3DTEXTURE9 Texture = Object->pFrame->pMesh->pTextures[0].pTexturesRef;

				//Get rect
				long ImageWidth = dbGetImageWidth(TerrainInfo->Texture);
				long ImageHeight = dbGetImageHeight(TerrainInfo->Texture);
				long TexX = unsigned long((X * ImageWidth) / TerrainInfo->TerrainSize);
				long TexY = unsigned long((Z * ImageHeight) / TerrainInfo->TerrainSize);
				long TexWidth = unsigned long(((Radius * ImageWidth) / TerrainInfo->TerrainSize));
				long TexHeight = unsigned long(((Radius * ImageHeight) / TerrainInfo->TerrainSize));
				RECT Rect;
				Rect.top = TexY - TexHeight;
				Rect.bottom = TexY + TexHeight;
				Rect.left = TexX - TexWidth;
				Rect.right = TexX + TexWidth;

				//Clamp rect
				if (Rect.top < 0)
					Rect.top = 0;
				if (Rect.top >= ImageHeight)
					return;
				if (Rect.left < 0)
					Rect.left = 0;
				if (Rect.left >= ImageWidth)
					return;
				if (Rect.bottom <= 0)
					return;
				if (Rect.bottom > ImageHeight)
					Rect.bottom = ImageHeight;
				if (Rect.right <= 0)
					return;
				if (Rect.right > ImageWidth)
					Rect.right = ImageWidth;

				//Get point position on rect
				long PosX = TexX - (Rect.left + Rect.right) / 2;
				long PosY = TexY - (Rect.top + Rect.bottom) / 2;

				//Lock rect
				D3DLOCKED_RECT LockedRect;
				if (Texture->LockRect(0, &LockedRect, &Rect, 0) == D3D_OK) {
					//Draw circle
					for (long x = 0; x < TexWidth * 2; x++) {
						if (x + Rect.left < ImageWidth) {
							for (long y = 0; y < TexHeight * 2; y++) {
								if (y + Rect.top < ImageHeight) {
									float px = (float(x - PosX) / float(TexWidth)) - 1.0f;
									float py = (float(y - PosY) / float(TexHeight)) - 1.0f;
									if (px * px + py * py < 1.0f) {
										((unsigned long*)LockedRect.pBits)[x + y * ImageWidth] = Colour;
									}
								}
							}
						}
					}
					//Unlock the rect
					Texture->UnlockRect(0);
				}
			}
		}
	}
}

EXPORT void BT_PaintTerrain(unsigned long TerrainID, float X, float Z, float Radius, unsigned long MinColour, unsigned long MaxColour)
{
//Check that the terrain exists
	if (BT_TerrainExist(TerrainID)) {
		//Get terrain info
		BT_TerrainInfo* TerrainInfo = (BT_TerrainInfo*)BT_GetTerrainInfo(TerrainID);

		//Check that the terrain is built
		if (TerrainInfo->Built == true) {
			//Check that the terrain has a texture
			if (TerrainInfo->Texture > 0) {
				//Get DBP Object
				sObject* Object = (sObject*)TerrainInfo->DBPObjectPtr;

				//Get texture
				LPDIRECT3DTEXTURE9 Texture = Object->pFrame->pMesh->pTextures[0].pTexturesRef;

				//Get rect
				long ImageWidth = dbGetImageWidth(TerrainInfo->Texture);
				long ImageHeight = dbGetImageHeight(TerrainInfo->Texture);
				long TexX = unsigned long((X * ImageWidth) / TerrainInfo->TerrainSize);
				long TexY = unsigned long((Z * ImageHeight) / TerrainInfo->TerrainSize);
				long TexWidth = unsigned long(((Radius * ImageWidth) / TerrainInfo->TerrainSize));
				long TexHeight = unsigned long(((Radius * ImageHeight) / TerrainInfo->TerrainSize));
				RECT Rect;
				Rect.top = TexY - TexHeight;
				Rect.bottom = TexY + TexHeight;
				Rect.left = TexX - TexWidth;
				Rect.right = TexX + TexWidth;

				//Clamp rect
				if (Rect.top < 0)
					Rect.top = 0;
				if (Rect.top >= ImageHeight)
					return;
				if (Rect.left < 0)
					Rect.left = 0;
				if (Rect.left >= ImageWidth)
					return;
				if (Rect.bottom <= 0)
					return;
				if (Rect.bottom > ImageHeight)
					Rect.bottom = ImageHeight;
				if (Rect.right <= 0)
					return;
				if (Rect.right > ImageWidth)
					Rect.right = ImageWidth;

				//Get point position on rect
				long PosX = TexX - (Rect.left + Rect.right) / 2;
				long PosY = TexY - (Rect.top + Rect.bottom) / 2;

				//Lock rect
				D3DLOCKED_RECT LockedRect;
				if (Texture->LockRect(0, &LockedRect, &Rect, 0) == D3D_OK) {
					//Draw circle
					for (long x = 0; x < TexWidth * 2; x++) {
						if (x + Rect.left < ImageWidth) {
							for (long y = 0; y < TexHeight * 2; y++) {
								if (y + Rect.top < ImageHeight) {
									float px = (float(x - PosX) / float(TexWidth)) - 1.0f;
									float py = (float(y - PosY) / float(TexHeight)) - 1.0f;
									if (px * px + py * py < 1.0f) {
										float pos = (rand() % 256) / 255.0f;
										D3DXCOLOR _MinColour = D3DXCOLOR(MinColour);
										D3DXCOLOR _MaxColour = D3DXCOLOR(MaxColour);
										unsigned char R = unsigned char(float(_MinColour.r + (_MaxColour.r - _MinColour.r) * pos) * 255);
										unsigned char G = unsigned char(float(_MinColour.g + (_MaxColour.g - _MinColour.g) * pos) * 255);
										unsigned char B = unsigned char(float(_MinColour.b + (_MaxColour.b - _MinColour.b) * pos) * 255);
										((unsigned char*)LockedRect.pBits)[(x + y * ImageWidth) * 4] = B;
										((unsigned char*)LockedRect.pBits)[(x + y * ImageWidth) * 4 + 1] = G;
										((unsigned char*)LockedRect.pBits)[(x + y * ImageWidth) * 4 + 2] = R;
									}
								}
							}
						}
					}
					//Unlock the rect
					Texture->UnlockRect(0);
				}
			}
		}
	}
}

EXPORT void BT_SetPointColour(unsigned long TerrainID, float X, float Z, unsigned long Colour)
{
//Check that the terrain exists
	if (BT_TerrainExist(TerrainID)) {
		//Get terrain info
		BT_TerrainInfo* TerrainInfo = (BT_TerrainInfo*)BT_GetTerrainInfo(TerrainID);

		//Check that the terrain is built
		if (TerrainInfo->Built == true) {
			//Check that the terrain has a texture
			if (TerrainInfo->Texture > 0) {
				//Get DBP Object
				sObject* Object = (sObject*)TerrainInfo->DBPObjectPtr;

				//Get texture
				LPDIRECT3DTEXTURE9 Texture = Object->pFrame->pMesh->pTextures[0].pTexturesRef;

				//Get rect
				long ImageWidth = dbGetImageWidth(TerrainInfo->Texture);
				long ImageHeight = dbGetImageHeight(TerrainInfo->Texture);
				long TexX = unsigned long((X * ImageWidth) / TerrainInfo->TerrainSize);
				long TexY = unsigned long((Z * ImageHeight) / TerrainInfo->TerrainSize);
				RECT Rect;
				Rect.top = TexY;
				Rect.bottom = TexY;
				Rect.left = TexX;
				Rect.right = TexX;

				//Clamp rect
				if (Rect.top < 0)
					Rect.top = 0;
				if (Rect.top >= ImageHeight - 1)
					return;
				if (Rect.left < 0)
					Rect.left = 0;
				if (Rect.left >= ImageWidth - 1)
					return;
				if (Rect.bottom <= 0)
					return;
				if (Rect.bottom > ImageHeight - 1)
					Rect.bottom = ImageHeight - 1;
				if (Rect.right <= 0)
					return;
				if (Rect.right > ImageWidth - 1)
					Rect.right = ImageWidth - 1;

				//Lock rect
				D3DLOCKED_RECT LockedRect;
				if (Texture->LockRect(0, &LockedRect, &Rect, 0) == D3D_OK) {
					//Set colour
					*((D3DCOLOR*)LockedRect.pBits) = Colour;

					//Unlock the rect
					Texture->UnlockRect(0);
				}
			}
		}
	}
}


EXPORT void BT_UpdateTerrainTexture(unsigned long TerrainID)
{
//Check that the terrain exists
	if (BT_TerrainExist(TerrainID)) {
		//Get terrain info
		BT_TerrainInfo* TerrainInfo = (BT_TerrainInfo*)BT_GetTerrainInfo(TerrainID);

		//Check that the terrain is built
		if (TerrainInfo->Built == true) {
			//Check that the terrain has a texture
			if (TerrainInfo->Texture > 0) {
				//Get DBP Object
				sObject* Object = (sObject*)TerrainInfo->DBPObjectPtr;

				//Get texture
				LPDIRECT3DTEXTURE9 Texture = Object->pFrame->pMesh->pTextures[0].pTexturesRef;

				//Update the texture
				D3DXFilterTexture(Texture, NULL, 0, D3DX_DEFAULT);
			}
		}
	}
}
