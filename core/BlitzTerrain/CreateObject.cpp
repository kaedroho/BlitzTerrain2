#ifndef COMPILE_GDK
	#include "CreateObject.h"
#else
	#include "..\BlitzTerrain\CreateObject.h"
#endif

#ifndef COMPILE_GDK

// Typedefs for the function pointers
typedef bool (__cdecl * CreateNewObject_t) (int, LPCSTR, int);
typedef sObject* (__cdecl *GetObjectPtr_t) (int);
typedef bool (__cdecl *SetupMeshFvfData_t) (sMesh*,DWORD,DWORD,DWORD);
typedef bool (__cdecl *SetNewObjectFinalProperties_t) (int,float);
typedef void (__cdecl *TextureObject_t)(int,int);
typedef bool (__cdecl *SetupStandardVertex_t)(DWORD,BYTE*,int,float,float,float,float,float,float,DWORD,float,float);


// Storage for the function pointers
CreateNewObject_t               CreateNewObject_f = 0;
GetObjectPtr_t                  GetObjectPtr_f = 0;
SetupMeshFvfData_t              SetupMeshFvfData_f = 0;
SetNewObjectFinalProperties_t   SetNewObjectFinalProperties_f = 0;
TextureObject_t                 TextureObject_f = 0;
SetupStandardVertex_t           SetupStandardVertex_f = 0;


// Inline functions for the function pointers
inline bool CreateNewObject(int ObjectId, LPCSTR Name, int LimbCount)
{
    return CreateNewObject_f(ObjectId, Name, LimbCount);
}

inline sObject* dbGetObject(int ObjectId)
{
    return GetObjectPtr_f(ObjectId);
}

inline bool SetupMeshFVFData(sMesh* Mesh, DWORD Fvf, DWORD VertexCount, DWORD IndexCount)
{
    return SetupMeshFvfData_f( Mesh, Fvf, VertexCount, IndexCount );
}

inline bool SetNewObjectFinalProperties(int ObjectId, float Radius)
{
    return SetNewObjectFinalProperties_f( ObjectId, Radius );
}

inline void dbTextureObject(int ObjectId, int ImageId)
{
    TextureObject_f( ObjectId, ImageId );
}

inline bool SetupStandardVertex(DWORD Fvf, BYTE* VertexPointer, int VertexOffset,
                                float x, float y, float z,
                                float nx, float ny, float nz, 
                                DWORD Diffuse,
                                float tu, float tv)
{
    return SetupStandardVertex_f(Fvf, VertexPointer, VertexOffset, x, y, z, nx, ny, nz, Diffuse, tu, tv);
}

void BT_Intern_Error(int);

// Initialisation of the function pointers is carried out when the core pointer is passed into the plug-in
void CO_ReceiveCoreDataPtr(LPVOID CorePtr)
{
	HMODULE mod=GetModuleHandle("DBProBasic3DDebug.dll");
    CreateNewObject_f  = (CreateNewObject_t)    GetProcAddress(mod, "?CreateNewObject@@YA_NHPADH@Z");
    GetObjectPtr_f     = (GetObjectPtr_t)       GetProcAddress(mod, "?GetObjectA@@YAPAUsObject@@H@Z");
    SetupMeshFvfData_f = (SetupMeshFvfData_t)   GetProcAddress(mod, "?SetupMeshFVFData@@YA_NPAUsMesh@@KKK@Z");
    SetNewObjectFinalProperties_f =
                (SetNewObjectFinalProperties_t) GetProcAddress(mod, "?SetNewObjectFinalProperties@@YA_NHM@Z");
    TextureObject_f    = (TextureObject_t)      GetProcAddress(mod, "?SetTexture@@YAXHH@Z");
    SetupStandardVertex_f = 
                (SetupStandardVertex_t)         GetProcAddress(mod, "?SetupStandardVertex@@YA_NKPAEHMMMMMMKMM@Z");
}

#else

bool CreateNewObject(int,LPSTR,int);
bool SetNewObjectFinalProperties(int ObjectId, float Radius);
bool SetupStandardVertex(DWORD Fvf, BYTE* VertexPointer, int VertexOffset,
                                float x, float y, float z,
                                float nx, float ny, float nz, 
                                DWORD Diffuse,
                                float tu, float tv);

void dbTextureObject(int,int);
sObject* dbGetObject(int);
bool SetupMeshFVFData(sMesh*,unsigned long,unsigned long,unsigned long);

#endif

// This is the function that creates everything.
sObject* BT_Intern_CreateBlankObject(DWORD ObjectId, DWORD FrameCount)
{
    CreateNewObject(ObjectId,"BlitzTerrain", FrameCount);
	sObject* Object = dbGetObject(ObjectId);
	return Object;
}

void BT_Intern_SetupMesh(sMesh* Mesh, DWORD VertexCount, DWORD IndexCount, DWORD Fvf)
{
	SetupMeshFVFData( Mesh, Fvf, VertexCount, IndexCount );
    Mesh->iPrimitiveType = D3DPT_TRIANGLELIST;
    Mesh->iDrawVertexCount = VertexCount;
    Mesh->iDrawPrimitives = (IndexCount ? IndexCount : VertexCount)  / 3;
	return;
}

void BT_Intern_FinnishObject(unsigned long ObjectID)
{
    SetNewObjectFinalProperties(ObjectID, 0.0f);
	dbTextureObject(ObjectID,0);
	return;
}