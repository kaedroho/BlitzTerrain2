#ifndef _CFDBP_H
#define _CFDBP_H

#include "dbp/DBOData.h"
#include "dbp/ccameradatac.h"

bool DBPRO_SetMeshRenderStates( sMesh* pMesh );
void DBPRO_ApplyEffect ( sMesh* pMesh, tagCameraData* m_Camera_Ptr );
#endif