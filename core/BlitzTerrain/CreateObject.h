#ifndef _CREATEOBJECT_H
#define _CREATEOBJECT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <globstruct.h>
#include <DBOData.h>

#ifdef COMPILE_GDK
	#include <DarkGDK.h>
#endif


sObject* BT_Intern_CreateBlankObject(DWORD ObjectId, DWORD FrameCount);
void BT_Intern_SetupMesh(sMesh* Mesh, DWORD VertexCount, DWORD IndexCount, DWORD Fvf);
void BT_Intern_FinnishObject(unsigned long ObjectID);

void CO_ReceiveCoreDataPtr(LPVOID CorePtr);

#endif