#include "main.h"
#include "quadmapping.h"
#include "CreateObject.h"

extern s_BT_main BT_Main;

#ifndef COMPILE_GDK
	extern t_dbGetDirect3DDevice dbGetDirect3DDevice;
#endif

void BT_Intern_StartQuadMapGeneration(BT_Quadmap_Generator Generator)
{
	if(BT_Main.QuadmapInfo.Locked==false){
		BT_Main.QuadmapInfo.Locked=true;
		BT_Main.QuadmapInfo.TempVertices=(Generator.Size+1)*(Generator.Size+1)+1;
		BT_Main.QuadmapInfo.TempQuads=Generator.Size*Generator.Size;
		BT_Main.QuadmapInfo.TempVertexdata=(BT_Quadmap_Vertex*)malloc((BT_Main.QuadmapInfo.TempVertices+1)*sizeof(BT_Quadmap_Vertex));
		if(BT_Main.QuadmapInfo.TempVertexdata==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memset(BT_Main.QuadmapInfo.TempVertexdata,0,(BT_Main.QuadmapInfo.TempVertices+1)*sizeof(BT_Quadmap_Vertex));
		BT_Main.QuadmapInfo.TempQuaddata=(BT_Quadmap_Quad*)malloc(BT_Main.QuadmapInfo.TempQuads*sizeof(BT_Quadmap_Quad));
		if(BT_Main.QuadmapInfo.TempQuaddata==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memset(BT_Main.QuadmapInfo.TempQuaddata,0,BT_Main.QuadmapInfo.TempQuads*sizeof(BT_Quadmap_Quad));
		BT_Main.QuadmapInfo.TempVertexMap=(BT_Quadmap_Vertex***)malloc((Generator.Size+1)*sizeof(BT_Quadmap_Vertex**));
		if(BT_Main.QuadmapInfo.TempVertexMap==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memset(BT_Main.QuadmapInfo.TempVertexMap,0,(Generator.Size+1)*sizeof(BT_Quadmap_Vertex**));
		for(unsigned long i=0;i<unsigned(Generator.Size+1);i++){
			BT_Main.QuadmapInfo.TempVertexMap[i]=(BT_Quadmap_Vertex**)malloc((Generator.Size+1)*sizeof(BT_Quadmap_Vertex*));
			if(BT_Main.QuadmapInfo.TempVertexMap[i]==nullptr)
				BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
			memset(BT_Main.QuadmapInfo.TempVertexMap[i],0,(Generator.Size+1)*sizeof(BT_Quadmap_Vertex*));
		}
	}
}

void BT_Intern_EndQuadMapGeneration()
{
	if(BT_Main.QuadmapInfo.Locked){
		free(BT_Main.QuadmapInfo.TempVertexdata);
		free(BT_Main.QuadmapInfo.TempQuaddata);
		free(BT_Main.QuadmapInfo.TempVertexMap);
		BT_Main.QuadmapInfo.TempVertices=0;
		BT_Main.QuadmapInfo.TempQuads=0;
		BT_Main.QuadmapInfo.Locked=false;
	}
}


void BT_QuadMap::MakeBuffer(char** Buffer,unsigned long* BufferSize)
{
//Check if the quad map has been generated
	if(Generated==true){
	//Calculate sizes
		unsigned long HeaderSize=sizeof(BT_Quadmap_FileHeader101);
		unsigned long VertexBufferSize=(Vertices+1)*sizeof(BT_Quadmap_LiteVertex101);
		unsigned long QuadBufferSize=Quads*sizeof(BT_Quadmap_LiteQuad101);
		unsigned long QuadMapSize=QuadsAccross*QuadsAccross*sizeof(unsigned short);
		unsigned short SegmentCount=SegmentsPerSide*4*Sector->Terrain->LODLevels;
		unsigned short EdgePoints=SegmentCount*PointsPerSegment;
		unsigned long EdgeHeightsSize=EdgePoints*(sizeof(float)+sizeof(bool))+SegmentCount*sizeof(unsigned short);
		*BufferSize=HeaderSize+VertexBufferSize+QuadBufferSize+QuadMapSize+EdgeHeightsSize+sizeof(unsigned short);

	//Allocate buffer
		(*Buffer)=(char*)malloc(*BufferSize);
		if(*Buffer==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

	//Fill Header
		*(unsigned short*)*Buffer=sizeof(BT_Quadmap_FileHeader101);
		BT_Quadmap_FileHeader101* Header=(BT_Quadmap_FileHeader101*)&(*Buffer)[sizeof(unsigned short)];
		Header->VertexCount=Vertices;
		Header->QuadCount=Quads;
		Header->HighestPoint=HighestPoint;
		Header->LowestPoint=LowestPoint;

	//Fill vertex buffer
		unsigned long CurrentPos=HeaderSize+sizeof(unsigned short);
		for(unsigned long Vertexn=0;Vertexn<=Vertices;Vertexn++){
			BT_Quadmap_LiteVertex101* Temp;
			Temp=(BT_Quadmap_LiteVertex101*)&(*Buffer)[CurrentPos];
			Temp->Pos_y=Vertex[Vertexn].Pos_y;
			Temp->Vrow=Vertex[Vertexn].Vrow;
			Temp->Vcol=Vertex[Vertexn].Vcol;
			Temp->Nrm_x=Vertex[Vertexn].Nrm_x;
			Temp->Nrm_y=Vertex[Vertexn].Nrm_y;
			Temp->Nrm_z=Vertex[Vertexn].Nrm_z;
			CurrentPos+=sizeof(BT_Quadmap_LiteVertex101);
		}

	//Fill quad buffer
		for(unsigned long Quadn=0;Quadn<Quads;Quadn++){
			BT_Quadmap_LiteQuad101* Temp;
			Temp=(BT_Quadmap_LiteQuad101*)&(*Buffer)[CurrentPos];
			Temp->V1=unsigned short((unsigned long)Quad[Quadn].V1-(unsigned long)Vertex);
			Temp->V2=unsigned short((unsigned long)Quad[Quadn].V2-(unsigned long)Vertex);
			Temp->V3=unsigned short((unsigned long)Quad[Quadn].V3-(unsigned long)Vertex);
			Temp->V4=unsigned short((unsigned long)Quad[Quadn].V4-(unsigned long)Vertex);
			Temp->Span=(unsigned char)float(Quad[Quadn].Width/TileSize);
			CurrentPos+=sizeof(BT_Quadmap_LiteQuad101);
		}

	//Fill QuadMap
		for(unsigned long Quadn=0;Quadn<unsigned long(QuadsAccross*QuadsAccross);Quadn++){
			unsigned short* Temp=(unsigned short*)&(*Buffer)[CurrentPos];
			if(QuadMap[Quadn]==NULL){
				*Temp=NULL;
			}else{
				*Temp=unsigned short((QuadMap[Quadn]-Quad)+1);
			}
			CurrentPos+=sizeof(unsigned short);
		}

	//Fill segments
		for(unsigned short Segment=0;Segment<SegmentsPerSide*4;Segment++){
			unsigned short SegmentPointStart=SegmentVertexStart[Segment];
			unsigned short* Temp=(unsigned short*)&(*Buffer)[CurrentPos];
			*Temp=SegmentPointStart;
			CurrentPos+=sizeof(unsigned short);
			for(unsigned short Point=0;Point<PointsPerSegment;Point++){
				float* Temp=(float*)&(*Buffer)[CurrentPos];
				*Temp=SegmentPointHeight[Segment][Point];
				CurrentPos+=sizeof(float);
				bool* BoolTemp=(bool*)&(*Buffer)[CurrentPos];
				*BoolTemp=SegmentPointReduced[Segment][Point];
				CurrentPos+=sizeof(bool);
			}
		}
	}
}


void BT_QuadMap::GenerateFromBuffer100(char* Buffer,unsigned long BufferSize,s_BT_Sector* SectorPtr)
{
//Set generated to true
	Generated=true;

//Get header
	unsigned short HeaderSize=*(unsigned short*)Buffer;
	BT_Quadmap_FileHeader100* Header=(BT_Quadmap_FileHeader100*)&Buffer[sizeof(unsigned short)];
	Vertices=(unsigned short)Header->VertexCount;
	Quads=(unsigned short)Header->QuadCount;
	Optimised=Header->Optimised==1;
	Size=Header->Size;
	TileSize=Header->TileSize;
	QuadsAccross=Header->QuadsAccross;
	HighestPoint=Header->HighestPoint;
	LowestPoint=Header->LowestPoint;
	Sector=SectorPtr;

//Calculate sizes
	unsigned long VertexBufferSize=(Vertices+1)*sizeof(BT_Quadmap_LiteVertex100);
	unsigned long QuadBufferSize=Quads*sizeof(BT_Quadmap_LiteQuad100);
	unsigned long QuadMapSize=Quads*sizeof(unsigned short);
	unsigned long OriginalEdgeHeightsSize=QuadsAccross*4*sizeof(float);

//Set current pos
	unsigned long CurrentPosb=HeaderSize+sizeof(unsigned short);

//Allocate vertex buffer
	Vertex=(BT_Quadmap_Vertex*)malloc((Vertices+1)*sizeof(BT_Quadmap_Vertex));
	if(Vertex==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through vertices
	for(unsigned long Vertexn=0;Vertexn<=Vertices;Vertexn++){
		BT_Quadmap_LiteVertex100* Temp;
		Temp=(BT_Quadmap_LiteVertex100*)&Buffer[CurrentPosb];
		Vertex[Vertexn].Exclude=Temp->Exclude;
		Vertex[Vertexn].Pos_y=Temp->Pos_y;
		Vertex[Vertexn].Vrow=Temp->Vrow;
		Vertex[Vertexn].Vcol=Temp->Vcol;
		Vertex[Vertexn].Nrm_x=Temp->Nrm_x;
		Vertex[Vertexn].Nrm_y=Temp->Nrm_y;
		Vertex[Vertexn].Nrm_z=Temp->Nrm_z;
		Vertex[Vertexn].Index=Temp->Index;
		CurrentPosb+=sizeof(BT_Quadmap_LiteVertex100);
	}

//Allocate quad buffer
	Quad=(BT_Quadmap_Quad*)malloc(Quads*sizeof(BT_Quadmap_Quad));
	if(Quad==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through quads
	for(unsigned long Quadn=0;Quadn<Quads;Quadn++){
		BT_Quadmap_LiteQuad100* Temp;
		Temp=(BT_Quadmap_LiteQuad100*)&Buffer[CurrentPosb];
		Quad[Quadn].Exclude=Temp->Exclude;
		Quad[Quadn].V1=(BT_Quadmap_Vertex*)(Temp->V1+(unsigned long)Vertex);
		Quad[Quadn].V2=(BT_Quadmap_Vertex*)(Temp->V2+(unsigned long)Vertex);
		Quad[Quadn].V3=(BT_Quadmap_Vertex*)(Temp->V3+(unsigned long)Vertex);
		Quad[Quadn].V4=(BT_Quadmap_Vertex*)(Temp->V4+(unsigned long)Vertex);
		Quad[Quadn].Width=Quad[Quadn].Height=Temp->Span*TileSize;
		Quad[Quadn].Rotation=Temp->Rotation;
		CurrentPosb+=sizeof(BT_Quadmap_LiteQuad100);
	}

//Allocate QuadMap
	QuadMap=(BT_Quadmap_Quad**)malloc(QuadsAccross*QuadsAccross*sizeof(BT_Quadmap_Quad*));
	if(QuadMap==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through QuadMap
	for(unsigned long Quadn=0;Quadn<unsigned long(QuadsAccross*QuadsAccross);Quadn++){
		unsigned short* Temp=(unsigned short*)&Buffer[CurrentPosb];
		if(*Temp==NULL){
			QuadMap[Quadn]=NULL;
		}else{
			QuadMap[Quadn]=(BT_Quadmap_Quad*)Quad+*Temp-1;
		}
		CurrentPosb+=sizeof(unsigned short);
	}

//Calculate edge heights
	/*
	unsigned char CurrentLODLevel;
	unsigned long CurrentLODLevelTileSpan;
	for(CurrentLODLevel=1;CurrentLODLevel<(Sector->Terrain->LODLevels-Sector->LODLevel->ID);CurrentLODLevel++){
		CurrentLODLevelTileSpan=0x1<<CurrentLODLevel;
		for(unsigned long i=0;i<unsigned long(QuadsAccross*4);i++){
			unsigned long LeftVertex=unsigned long(floor(i/float(CurrentLODLevelTileSpan))*CurrentLODLevelTileSpan);
			unsigned long RightVertex=unsigned long(ceil(i/float(CurrentLODLevelTileSpan))*CurrentLODLevelTileSpan);
			if(RightVertex>=unsigned long(QuadsAccross*4))
				RightVertex=0;

			if(LeftVertex==RightVertex){
				EdgeHeights[i+QuadsAccross*4*CurrentLODLevel]=EdgeHeights[i];
			}else{
				float Position=float(float(i-LeftVertex)/float(RightVertex-LeftVertex));
				EdgeHeights[i+QuadsAccross*4*CurrentLODLevel]=EdgeHeights[LeftVertex]+(EdgeHeights[RightVertex]-EdgeHeights[LeftVertex])*Position;
			}
		}
	}
	*/
#pragma message("update this!")
}


void BT_QuadMap::GenerateFromBuffer101(char* Buffer,unsigned long BufferSize,s_BT_Sector* SectorPtr)
{
//Set generated to true
	Generated=true;

//Get header
	unsigned short HeaderSize=*(unsigned short*)Buffer;
	BT_Quadmap_FileHeader101* Header=(BT_Quadmap_FileHeader101*)&Buffer[sizeof(unsigned short)];
	Vertices=Header->VertexCount;
	Quads=Header->QuadCount;
	Optimised=SectorPtr->Terrain->MeshOptimisation;
	Size=SectorPtr->LODLevel->SectorSize;
	TileSize=SectorPtr->LODLevel->SectorSize/SectorPtr->LODLevel->SectorDetail;
	QuadsAccross=SectorPtr->LODLevel->SectorDetail;
	HighestPoint=Header->HighestPoint;
	LowestPoint=Header->LowestPoint;
	Sector=SectorPtr;

//Calculate sizes
	unsigned long VertexBufferSize=(Vertices+1)*sizeof(BT_Quadmap_LiteVertex101);
	unsigned long QuadBufferSize=Quads*sizeof(BT_Quadmap_LiteQuad101);
	unsigned long QuadMapSize=Quads*sizeof(unsigned short);
	unsigned long OriginalEdgeHeightsSize=QuadsAccross*4*sizeof(float);

//Set current pos
	unsigned long CurrentPosb=HeaderSize+sizeof(unsigned short);

//Allocate vertex buffer
	Vertex=(BT_Quadmap_Vertex*)malloc((Vertices+1)*sizeof(BT_Quadmap_Vertex));
	if(Vertex==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through vertices
	for(unsigned short Vertexn=0;Vertexn<=Vertices;Vertexn++){
		BT_Quadmap_LiteVertex101* Temp;
		Temp=(BT_Quadmap_LiteVertex101*)&Buffer[CurrentPosb];
		Vertex[Vertexn].Pos_y=Temp->Pos_y;
		Vertex[Vertexn].Vrow=Temp->Vrow;
		Vertex[Vertexn].Vcol=Temp->Vcol;
		Vertex[Vertexn].Nrm_x=Temp->Nrm_x;
		Vertex[Vertexn].Nrm_y=Temp->Nrm_y;
		Vertex[Vertexn].Nrm_z=Temp->Nrm_z;
		Vertex[Vertexn].Index=Vertexn;
		Vertex[Vertexn].Exclude=0;
		CurrentPosb+=sizeof(BT_Quadmap_LiteVertex101);
	}

//Allocate quad buffer
	Quad=(BT_Quadmap_Quad*)malloc(Quads*sizeof(BT_Quadmap_Quad));
	if(Quad==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through quads
	for(unsigned short Quadn=0;Quadn<Quads;Quadn++){
		BT_Quadmap_LiteQuad101* Temp;
		Temp=(BT_Quadmap_LiteQuad101*)&Buffer[CurrentPosb];
		Quad[Quadn].V1=(BT_Quadmap_Vertex*)(Temp->V1+(unsigned long)Vertex);
		Quad[Quadn].V2=(BT_Quadmap_Vertex*)(Temp->V2+(unsigned long)Vertex);
		Quad[Quadn].V3=(BT_Quadmap_Vertex*)(Temp->V3+(unsigned long)Vertex);
		Quad[Quadn].V4=(BT_Quadmap_Vertex*)(Temp->V4+(unsigned long)Vertex);
		Quad[Quadn].Width=Quad[Quadn].Height=Temp->Span*TileSize;
		Quad[Quadn].Exclude=0;

		if(SectorPtr->Terrain->QuadRotation){
		//Calculate Quad Rotation
			float V1h=Quad[Quadn].V1->Pos_y;
			float V2h=Quad[Quadn].V2->Pos_y;
			float V3h=Quad[Quadn].V3->Pos_y;
			float V4h=Quad[Quadn].V4->Pos_y;

		//Get height differences
			float HD1=fabs(V4h-V1h);
			float HD2=fabs(V3h-V2h);

		//Work out rotation
			Quad[Quadn].Rotation=HD1>HD2;
		}else{
			Quad[Quadn].Rotation=0;
		}

		CurrentPosb+=sizeof(BT_Quadmap_LiteQuad101);
	}

//Allocate QuadMap
	QuadMap=(BT_Quadmap_Quad**)malloc(QuadsAccross*QuadsAccross*sizeof(BT_Quadmap_Quad*));
	if(QuadMap==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Loop through QuadMap
	for(unsigned long Quadn=0;Quadn<unsigned long(QuadsAccross*QuadsAccross);Quadn++){
		unsigned short* Temp=(unsigned short*)&Buffer[CurrentPosb];
		if(*Temp==NULL){
			QuadMap[Quadn]=NULL;
		}else{
			QuadMap[Quadn]=(BT_Quadmap_Quad*)Quad+*Temp-1;
		}
		CurrentPosb+=sizeof(unsigned short);
	}

//Calculate segments per side
	unsigned char LODLevel=Sector->LODLevel->ID;
	TileSpan=unsigned char(pow(2.0f,float(LODLevel)));
	SegmentsPerSide=TileSpan;

//Calculate points per segment
	PointsPerSegment=unsigned char(SectorPtr->LODLevel->SectorDetail);
	ActualPointsPerSegment=unsigned char(PointsPerSegment/SegmentsPerSide);

//Allocate Edgeheights
	SegmentVertexStart=(unsigned short*)malloc(SegmentsPerSide*4*sizeof(unsigned short));
	if(SegmentVertexStart==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	SegmentPointHeight=(float**)malloc(SegmentsPerSide*4*sizeof(float*));
	if(SegmentPointHeight==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	SegmentPointReduced=(bool**)malloc(SegmentsPerSide*4*sizeof(bool*));
	if(SegmentPointReduced==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	for(unsigned short i=0;i<SegmentsPerSide*4;i++){
		SegmentPointHeight[i]=(float*)malloc((PointsPerSegment+1)*sizeof(float));
		if(SegmentPointHeight[i]==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		SegmentPointReduced[i]=(bool*)malloc((PointsPerSegment+1)*sizeof(bool));
		if(SegmentPointReduced[i]==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memset(SegmentPointHeight[i],0,(PointsPerSegment+1)*sizeof(float));
		memset(SegmentPointReduced[i],0,(PointsPerSegment+1)*sizeof(bool));
	}

//Insert segments into segment arrays
	unsigned char SectorSpan=Sector->Terrain->LODLevel[0].Split/Sector->LODLevel->Split;
	unsigned short SectorRow=Sector->Row*SectorSpan;
	unsigned short SectorCollumn=Sector->Column*SectorSpan;
	for(unsigned char Segment=0;Segment<SegmentsPerSide;Segment++){
		if(SectorRow>SectorSpan-1)
			Sector->Terrain->BottomSegPointHeight[LODLevel][SectorRow][SectorCollumn+Segment]=SegmentPointHeight[Segment]; //TOP

		if(SectorCollumn>SectorSpan-1)
			Sector->Terrain->RightSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn]=SegmentPointHeight[SegmentsPerSide*3+SegmentsPerSide-Segment-1]; //LEFT  [SegmentsPerSide*3+SegmentsPerSide-Segment-1]

		if(SectorRow<Sector->Terrain->LODLevel[0].Split-SectorSpan)
			Sector->Terrain->TopSegPointHeight[LODLevel][SectorRow+SectorSpan][SectorCollumn+Segment]=SegmentPointHeight[SegmentsPerSide*2+SegmentsPerSide-Segment-1]; //BOTTOM  [SegmentsPerSide*2+SegmentsPerSide-Segment-1]

		if(SectorCollumn<Sector->Terrain->LODLevel[0].Split-SectorSpan)
			Sector->Terrain->LeftSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn+SectorSpan]=SegmentPointHeight[SegmentsPerSide+Segment]; //RIGHT
	}

//Fill segments
	for(unsigned short Segment=0;Segment<SegmentsPerSide*4;Segment++){
		unsigned short* Temp=(unsigned short*)&Buffer[CurrentPosb];
		SegmentVertexStart[Segment]=*Temp;
		CurrentPosb+=sizeof(unsigned short);
		for(unsigned short Point=0;Point<PointsPerSegment;Point++){
			float* Temp=(float*)&Buffer[CurrentPosb];
			SegmentPointHeight[Segment][Point]=*Temp;
			CurrentPosb+=sizeof(float);
			bool* BoolTemp=(bool*)&Buffer[CurrentPosb];
			SegmentPointReduced[Segment][Point]=*BoolTemp;
			CurrentPosb+=sizeof(bool);
		}
	}
}


void BT_QuadMap::Generate(BT_Quadmap_Generator Generator)
{
//Variables
	unsigned short Vertexn;
	unsigned short Quadn;
	unsigned char Vrow;
	unsigned char Vcol;
	unsigned short vertexnum;
	float SectorSize;
	unsigned short IncVertices;
	unsigned short IncQuads;
	unsigned short CurrVertex;
	unsigned short CurrQuad;
	BT_Quadmap_Vertex* TempVertex;
	BT_Quadmap_Quad* TempQuad;
	unsigned long InnerVertices;

//Calculate some Values
	Vertices=(Generator.Size+1)*(Generator.Size+1);
	Quads=Generator.Size*Generator.Size;
	SectorSize=Generator.Size*Generator.TileSize;
	Optimised=Generator.Optimise;
	QuadsAccross=Generator.Size;
	Size=SectorSize;
	InnerVertices=(QuadsAccross)*(QuadsAccross);
	Sector=Generator.Sector;
	TileSize=Generator.TileSize;

//Allocate Vertices and Quads
	TempVertex=BT_Main.QuadmapInfo.TempVertexdata;
	TempQuad=BT_Main.QuadmapInfo.TempQuaddata;
	BT_Quadmap_Vertex*** VertexMap=BT_Main.QuadmapInfo.TempVertexMap;

//Allocate Quadmap
	QuadMap=(BT_Quadmap_Quad**)malloc(Quads*sizeof(BT_Quadmap_Quad*));
	if(QuadMap==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

//Calculate segments per side
	unsigned char LODLevel=Sector->LODLevel->ID;
	TileSpan=unsigned char(pow(2.0f,float(LODLevel)));
	SegmentsPerSide=TileSpan;

//Calculate points per segment
	PointsPerSegment=unsigned char(Generator.Size);
	ActualPointsPerSegment=unsigned char(Generator.Size/SegmentsPerSide);

//Allocate Edgeheights
	SegmentVertexStart=(unsigned short*)malloc(SegmentsPerSide*4*sizeof(unsigned short));
	if(SegmentVertexStart==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	SegmentPointHeight=(float**)malloc(SegmentsPerSide*4*sizeof(float*));
	if(SegmentPointHeight==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	SegmentPointReduced=(bool**)malloc(SegmentsPerSide*4*sizeof(bool*));
	if(SegmentPointReduced==nullptr)
		BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	for(unsigned short i=0;i<SegmentsPerSide*4;i++){
		SegmentPointHeight[i]=(float*)malloc((PointsPerSegment+1)*sizeof(float));
		if(SegmentPointHeight[i]==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		SegmentPointReduced[i]=(bool*)malloc((PointsPerSegment+1)*sizeof(bool));
		if(SegmentPointReduced[i]==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memset(SegmentPointHeight[i],0,(PointsPerSegment+1)*sizeof(float));
		memset(SegmentPointReduced[i],0,(PointsPerSegment+1)*sizeof(bool));
	}

//Insert segments into segment arrays
	unsigned char SectorSpan=Sector->Terrain->LODLevel[0].Split/Sector->LODLevel->Split;
	unsigned short SectorRow=Sector->Row*SectorSpan;
	unsigned short SectorCollumn=Sector->Column*SectorSpan;
	for(unsigned char Segment=0;Segment<SegmentsPerSide;Segment++){
		if(SectorRow>SectorSpan-1)
			Sector->Terrain->BottomSegPointHeight[LODLevel][SectorRow][SectorCollumn+Segment]=SegmentPointHeight[Segment]; //TOP

		if(SectorCollumn>SectorSpan-1)
			Sector->Terrain->RightSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn]=SegmentPointHeight[SegmentsPerSide*3+SegmentsPerSide-Segment-1]; //LEFT  [SegmentsPerSide*3+SegmentsPerSide-Segment-1]

		if(SectorRow<Sector->Terrain->LODLevel[0].Split-SectorSpan)
			Sector->Terrain->TopSegPointHeight[LODLevel][SectorRow+SectorSpan][SectorCollumn+Segment]=SegmentPointHeight[SegmentsPerSide*2+SegmentsPerSide-Segment-1]; //BOTTOM  [SegmentsPerSide*2+SegmentsPerSide-Segment-1]

		if(SectorCollumn<Sector->Terrain->LODLevel[0].Split-SectorSpan)
			Sector->Terrain->LeftSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn+SectorSpan]=SegmentPointHeight[SegmentsPerSide+Segment]; //RIGHT
	}

//Loop through vertices
	Vertexn=0;

//Top
	for(unsigned char i=0;i<Generator.Size;i++){
	//Set row and collumn
		Vrow=0;
		Vcol=i;

		//Set vertexmap
			VertexMap[Vrow][Vcol]=&TempVertex[Vertexn];

		//Multiply row and collumn by tilespan
			Vrow=Vrow*Generator.TileSpan;
			Vcol=Vcol*Generator.TileSpan;

		//Calculate position
			TempVertex[Vertexn].Vrow=unsigned char(Vrow);
			TempVertex[Vertexn].Vcol=unsigned char(Vcol);
			TempVertex[Vertexn].Pos_y=Generator.heights[Vcol+Vrow*(Generator.Size+1)];

			unsigned short SegmentPoint=Vertexn*TileSpan;
			unsigned short Segment=0;
			while(!(SegmentPoint<PointsPerSegment)){
				SegmentPoint-=PointsPerSegment;
				Segment++;
			}
			SegmentPointHeight[Segment][SegmentPoint]=TempVertex[Vertexn].Pos_y;

			if(TempVertex[Vertexn].Pos_y>HighestPoint){
				HighestPoint=TempVertex[Vertexn].Pos_y;
				HighestVertex=Vertexn;
			}

			if(TempVertex[Vertexn].Pos_y<LowestPoint){
				LowestPoint=TempVertex[Vertexn].Pos_y;
				LowestVertex=Vertexn;
			}

		//Index
			TempVertex[Vertexn].Index=(unsigned short)Vertexn;

		Vertexn++;
	}

//Right
	for(unsigned char i=0;i<Generator.Size;i++){
	//Set row and collumn
		Vrow=i;
		Vcol=Generator.Size;

		//Set vertexmap
			VertexMap[Vrow][Vcol]=&TempVertex[Vertexn];

		//Multiply row and collumn by tilespan
			Vrow=Vrow*Generator.TileSpan;
			Vcol=Vcol*Generator.TileSpan;

		//Calculate position
			TempVertex[Vertexn].Vrow=unsigned char(Vrow);
			TempVertex[Vertexn].Vcol=unsigned char(Vcol);
			TempVertex[Vertexn].Pos_y=Generator.heights[Vcol+Vrow*(Generator.Size+1)];

			unsigned short SegmentPoint=Vertexn*TileSpan;
			unsigned short Segment=0;
			while(!(SegmentPoint<PointsPerSegment)){
				SegmentPoint-=PointsPerSegment;
				Segment++;
			}
			SegmentPointHeight[Segment][SegmentPoint]=TempVertex[Vertexn].Pos_y;

			if(TempVertex[Vertexn].Pos_y>HighestPoint){
				HighestPoint=TempVertex[Vertexn].Pos_y;
				HighestVertex=Vertexn;
			}

			if(TempVertex[Vertexn].Pos_y<LowestPoint){
				LowestPoint=TempVertex[Vertexn].Pos_y;
				LowestVertex=Vertexn;
			}

		//Index
			TempVertex[Vertexn].Index=(unsigned short)Vertexn;

		Vertexn++;
	}

//Bottom
	for(unsigned char i=0;i<Generator.Size;i++)
	{
	//Set row and collumn
		Vrow=Generator.Size;
		Vcol=Generator.Size-i;

		//Set vertexmap
			VertexMap[Vrow][Vcol]=&TempVertex[Vertexn];

		//Multiply row and collumn by tilespan
			Vrow=Vrow*Generator.TileSpan;
			Vcol=Vcol*Generator.TileSpan;

		//Calculate position
			TempVertex[Vertexn].Vrow=unsigned char(Vrow);
			TempVertex[Vertexn].Vcol=unsigned char(Vcol);
			TempVertex[Vertexn].Pos_y=Generator.heights[Vcol+Vrow*(Generator.Size+1)];

			unsigned short SegmentPoint=Vertexn*TileSpan;
			unsigned short Segment=0;
			while(!(SegmentPoint<PointsPerSegment)){
				SegmentPoint-=PointsPerSegment;
				Segment++;
			}
			SegmentPointHeight[Segment][SegmentPoint]=TempVertex[Vertexn].Pos_y;

			if(TempVertex[Vertexn].Pos_y>HighestPoint){
				HighestPoint=TempVertex[Vertexn].Pos_y;
				HighestVertex=Vertexn;
			}

			if(TempVertex[Vertexn].Pos_y<LowestPoint){
				LowestPoint=TempVertex[Vertexn].Pos_y;
				LowestVertex=Vertexn;
			}

		//Index
			TempVertex[Vertexn].Index=(unsigned short)Vertexn;

		Vertexn++;
	}

//Left
	for(unsigned char i=0;i<Generator.Size;i++){
	//Set row and collumn
		Vrow=Generator.Size-i;
		Vcol=0;

		//Set vertexmap
			VertexMap[Vrow][Vcol]=&TempVertex[Vertexn];

		//Multiply row and collumn by tilespan
			Vrow=Vrow*Generator.TileSpan;
			Vcol=Vcol*Generator.TileSpan;

		//Calculate position
			TempVertex[Vertexn].Vrow=unsigned char(Vrow);
			TempVertex[Vertexn].Vcol=unsigned char(Vcol);
			TempVertex[Vertexn].Pos_y=Generator.heights[Vcol+Vrow*(Generator.Size+1)];

			unsigned short SegmentPoint=Vertexn*TileSpan;
			unsigned short Segment=0;
			while(!(SegmentPoint<PointsPerSegment)){
				SegmentPoint-=PointsPerSegment;
				Segment++;
			}
			SegmentPointHeight[Segment][SegmentPoint]=TempVertex[Vertexn].Pos_y;

			if(TempVertex[Vertexn].Pos_y>HighestPoint){
				HighestPoint=TempVertex[Vertexn].Pos_y;
				HighestVertex=Vertexn;
			}

			if(TempVertex[Vertexn].Pos_y<LowestPoint){
				LowestPoint=TempVertex[Vertexn].Pos_y;
				LowestVertex=Vertexn;
			}

		//Index
			TempVertex[Vertexn].Index=(unsigned short)Vertexn;

		Vertexn++;
	}

//Loop through segments
	for(unsigned short Segment=0;Segment<SegmentsPerSide*4;Segment++){
	//Calculate reduced points
		for(unsigned short SegmentPoint=0;SegmentPoint<PointsPerSegment;SegmentPoint++)
			if(SegmentPoint%TileSpan!=0)
				SegmentPointReduced[Segment][SegmentPoint]=true;

	//Calculate segment vertex starts
		SegmentVertexStart[Segment]=ActualPointsPerSegment*Segment;

	//Join segment corners
		if(Segment!=SegmentsPerSide*4-1)
			SegmentPointHeight[Segment][PointsPerSegment]=SegmentPointHeight[Segment+1][0];
	}

//Join last corner with first corner
	SegmentPointHeight[SegmentsPerSide*4-1][PointsPerSegment]=SegmentPointHeight[0][0];

//Inside
	for(unsigned char i=0;i<unsigned(Generator.Size-1);i++){
		for(unsigned char ii=0;ii<unsigned(Generator.Size-1);ii++){
		//Set row and collumn
			Vrow=i+1;
			Vcol=ii+1;

			//Set vertexmap
				VertexMap[Vrow][Vcol]=&TempVertex[Vertexn];

			//Multiply row and collumn by tilespan
				Vrow=Vrow*Generator.TileSpan;
				Vcol=Vcol*Generator.TileSpan;

			//Calculate position
				TempVertex[Vertexn].Vrow=unsigned char(Vrow);
				TempVertex[Vertexn].Vcol=unsigned char(Vcol);
				TempVertex[Vertexn].Pos_y=Generator.heights[Vcol+Vrow*(Generator.Size+1)];

				if(TempVertex[Vertexn].Pos_y>HighestPoint){
					HighestPoint=TempVertex[Vertexn].Pos_y;
					HighestVertex=Vertexn;
				}

				if(TempVertex[Vertexn].Pos_y<LowestPoint){
					LowestPoint=TempVertex[Vertexn].Pos_y;
					LowestVertex=Vertexn;
				}

			//Index
				TempVertex[Vertexn].Index=(unsigned short)Vertexn;

			Vertexn++;
		}
	}

//Loop through Quads
	for(Quadn=0;Quadn<Quads;Quadn++){
	//Get row and collumn
		Vrow=(unsigned char)floor((float)(Quadn/Generator.Size));
		Vcol=Quadn-Vrow*Generator.Size;

	//Calculate vertices
		vertexnum=(Generator.Size+1)*Vrow+Vcol;
		TempQuad[Quadn].V1=VertexMap[Vrow][Vcol];
		TempQuad[Quadn].V2=VertexMap[Vrow][Vcol+1];
		TempQuad[Quadn].V3=VertexMap[Vrow+1][Vcol];
		TempQuad[Quadn].V4=VertexMap[Vrow+1][Vcol+1];

	//Sizes
		TempQuad[Quadn].Width=Generator.TileSize;
		TempQuad[Quadn].Height=Generator.TileSize;

	//Exclusion mapping
		if(Generator.exclusion!=NULL){
			TempQuad[Quadn].Exclude=Generator.exclusion[Vcol+Vrow*(Generator.Size+1)] &&
			Generator.exclusion[(Vcol+1)+Vrow*(Generator.Size+1)] &&
			Generator.exclusion[Vcol+(Vrow+1)*(Generator.Size+1)] &&
			Generator.exclusion[(Vcol+1)+(Vrow+1)*(Generator.Size+1)];
		}else{
			TempQuad[Quadn].Exclude=false;
		}

		/*
	//Exclude if remove far X/Z is set
		if(Vrow==Generator.Size-1 && Generator.RemoveFarX==true)
			TempQuad[Quadn].Exclude=true;
		if(Vcol==Generator.Size-1 && Generator.RemoveFarZ==true)
			TempQuad[Quadn].Exclude=true;
		*/

	//Set Quadmap
		if(Generator.Optimise==false){
			QuadMap[Quadn]=&TempQuad[Quadn];
		}else{
			QuadMap[Quadn]=NULL;
		}
	}

//Calculate normals
	for(unsigned char i=0;i<unsigned(Generator.Size-1);i++){
		for(unsigned char ii=0;ii<unsigned(Generator.Size-1);ii++){
		//Set row and collumn
			Vrow=i+1;
			Vcol=ii+1;

		//Find vertices
			BT_Quadmap_Vertex* ThisVertex=VertexMap[Vrow][Vcol];
			BT_Quadmap_Vertex* LeftVertex=VertexMap[Vrow][Vcol-1];
			BT_Quadmap_Vertex* RightVertex=VertexMap[Vrow][Vcol+1];
			BT_Quadmap_Vertex* TopVertex=VertexMap[Vrow-1][Vcol];
			BT_Quadmap_Vertex* BottomVertex=VertexMap[Vrow+1][Vcol];

		//Work out normal
			float Dx=(BottomVertex->Pos_y-TopVertex->Pos_y)/TileSize;
			float Dz=(RightVertex->Pos_y-LeftVertex->Pos_y)/TileSize;
			Dx=Dx/Sector->Terrain->Scale*C_BT_INTERNALSCALE;
			Dz=Dz/Sector->Terrain->Scale*C_BT_INTERNALSCALE;
			float Dy=float(1.0/sqrt(1.0+Dx*Dx+Dz*Dz));
			ThisVertex->Nrm_x=signed char((-Dz*Dy)*127.0);
			ThisVertex->Nrm_y=signed char((Dy)*127.0);
			ThisVertex->Nrm_z=signed char((-Dx*Dy)*127.0);
		}
	}



//Initialise include variables
	IncVertices=Vertices;
	IncQuads=Quads;


//Quad reduction
	if(Generator.QuadReduction==true){
		unsigned char Level=1;
		unsigned short QuadsLeft;
		do{
			QuadsLeft=0;
			unsigned short TwoPowerLevel=unsigned short(pow(float(2),float(Level))); //2^Level
			unsigned short TwoPowerLevelSquared=TwoPowerLevel*TwoPowerLevel;		 //(2^Level)^2
			for(Quadn=0;Quadn<Quads/TwoPowerLevelSquared;Quadn++){
			//Calculate row and collumn
				unsigned char RealQuadCol=(Quadn%(QuadsAccross/TwoPowerLevel))*TwoPowerLevel;
				unsigned char RealQuadRow=((Quadn*TwoPowerLevel-RealQuadCol)/(QuadsAccross/TwoPowerLevel));

			//Calculate quad ids
				unsigned short TL=RealQuadRow*QuadsAccross+RealQuadCol;
				unsigned short TR=TL+TwoPowerLevel/2;
				unsigned short BL=(RealQuadRow+TwoPowerLevel/2)*QuadsAccross+RealQuadCol;
				unsigned short BR=BL+TwoPowerLevel/2;

			//Calculate
				if(TempQuad[TL].Width==TileSize*TwoPowerLevel/2){
					if(ReduceQuad(TL,TR,BL,BR,TempQuad,Generator.ReductionThreshold)){
					//Exclude other quads
						TempQuad[TR].Exclude=true;
						TempQuad[BL].Exclude=true;
						TempQuad[BR].Exclude=true;

					//Level out vertices
						for(Vertexn=0;Vertexn<=TwoPowerLevel/2+1;Vertexn++){
						//Position
							float Position=float(Vertexn)/float(TwoPowerLevel);

						//Vertices
							BT_Quadmap_Vertex* TopVertex=VertexMap[TempQuad[TL].V1->Vrow][TempQuad[TL].V1->Vcol+Vertexn];
							BT_Quadmap_Vertex* RightVertex=VertexMap[TempQuad[TR].V2->Vrow+Vertexn][TempQuad[TR].V2->Vcol];
							BT_Quadmap_Vertex* BottomVertex=VertexMap[TempQuad[BL].V3->Vrow][TempQuad[BL].V3->Vcol+Vertexn];
							BT_Quadmap_Vertex* LeftVertex=VertexMap[TempQuad[TL].V1->Vrow+Vertexn][TempQuad[TL].V1->Vcol];

						//Level out vertex positions
							TopVertex->Pos_y=TempQuad[TL].V1->Pos_y+(TempQuad[TR].V2->Pos_y-TempQuad[TL].V1->Pos_y)*Position;
							RightVertex->Pos_y=TempQuad[TR].V2->Pos_y+(TempQuad[BR].V4->Pos_y-TempQuad[TR].V2->Pos_y)*Position;
							BottomVertex->Pos_y=TempQuad[BL].V3->Pos_y+(TempQuad[BR].V4->Pos_y-TempQuad[BL].V3->Pos_y)*Position;
							LeftVertex->Pos_y=TempQuad[TL].V1->Pos_y+(TempQuad[BL].V3->Pos_y-TempQuad[TL].V1->Pos_y)*Position;

						//Set edgepoint reduced value
							if(Vertexn>0 && Vertexn<TwoPowerLevel/2+1){
							//Top
								if(TopVertex->Index<QuadsAccross*4)
									SegmentPointReduced[0][TopVertex->Vcol]=true;
							//Right
								if(RightVertex->Index<QuadsAccross*4)
									SegmentPointReduced[SegmentsPerSide][RightVertex->Vrow]=true;
							//Bottom
								if(BottomVertex->Index<QuadsAccross*4)
									SegmentPointReduced[SegmentsPerSide*2][BottomVertex->Vcol]=true;
							//Left
								if(LeftVertex->Index<QuadsAccross*4)
									SegmentPointReduced[SegmentsPerSide*3][LeftVertex->Vrow]=true;
							}

						//Level out normals
							TopVertex->Nrm_x=signed char(TempQuad[TL].V1->Nrm_x+(TempQuad[TR].V2->Nrm_x-TempQuad[TL].V1->Nrm_x)*Position);
							TopVertex->Nrm_y=signed char(TempQuad[TL].V1->Nrm_y+(TempQuad[TR].V2->Nrm_y-TempQuad[TL].V1->Nrm_y)*Position);
							TopVertex->Nrm_z=signed char(TempQuad[TL].V1->Nrm_z+(TempQuad[TR].V2->Nrm_z-TempQuad[TL].V1->Nrm_z)*Position);
							RightVertex->Nrm_x=signed char(TempQuad[TR].V2->Nrm_x+(TempQuad[BR].V4->Nrm_x-TempQuad[TR].V2->Nrm_x)*Position);
							RightVertex->Nrm_y=signed char(TempQuad[TR].V2->Nrm_y+(TempQuad[BR].V4->Nrm_y-TempQuad[TR].V2->Nrm_y)*Position);
							RightVertex->Nrm_z=signed char(TempQuad[TR].V2->Nrm_z+(TempQuad[BR].V4->Nrm_z-TempQuad[TR].V2->Nrm_z)*Position);
							BottomVertex->Nrm_x=signed char(TempQuad[BL].V3->Nrm_x+(TempQuad[BR].V4->Nrm_x-TempQuad[BL].V3->Nrm_x)*Position);
							BottomVertex->Nrm_y=signed char(TempQuad[BL].V3->Nrm_y+(TempQuad[BR].V4->Nrm_y-TempQuad[BL].V3->Nrm_y)*Position);
							BottomVertex->Nrm_z=signed char(TempQuad[BL].V3->Nrm_z+(TempQuad[BR].V4->Nrm_z-TempQuad[BL].V3->Nrm_z)*Position);
							LeftVertex->Nrm_x=signed char(TempQuad[TL].V1->Nrm_x+(TempQuad[BL].V3->Nrm_x-TempQuad[TL].V1->Nrm_x)*Position);
							LeftVertex->Nrm_y=signed char(TempQuad[TL].V1->Nrm_y+(TempQuad[BL].V3->Nrm_y-TempQuad[TL].V1->Nrm_y)*Position);
							LeftVertex->Nrm_z=signed char(TempQuad[TL].V1->Nrm_z+(TempQuad[BL].V3->Nrm_z-TempQuad[TL].V1->Nrm_z)*Position);
						}

					//Merge quads
						TempQuad[TL].V2=TempQuad[TR].V2;
						TempQuad[TL].V3=TempQuad[BL].V3;
						TempQuad[TL].V4=TempQuad[BR].V4;
						TempQuad[TL].Width*=2;
						TempQuad[TL].Height*=2;

					//Increase Quadsleft
						QuadsLeft++;
					}
				}
			}
			Level++;
		}while(QuadsLeft>3);
	}

//Fill the rest of the segment points
	for(unsigned char Segment=0;Segment<SegmentsPerSide*4;Segment++)
		FillSegmentPointsRec(Segment,PointsPerSegment,0);

//Extra variables for quad rotation
	float V1h;
	float V2h;
	float V3h;
	float V4h;
	float HD1;
	float HD2;

//Quad rotation
	if(Generator.QuadRotation==true){
		for(Quadn=0;Quadn<Quads;Quadn++){
			if(TempQuad[Quadn].Exclude==false){
			//Get the heights
				V1h=TempQuad[Quadn].V1->Pos_y;
				V2h=TempQuad[Quadn].V2->Pos_y;
				V3h=TempQuad[Quadn].V3->Pos_y;
				V4h=TempQuad[Quadn].V4->Pos_y;

			//Get height differences
				HD1=fabs(V4h-V1h);
				HD2=fabs(V3h-V2h);

			//Work out rotation
				TempQuad[Quadn].Rotation=HD1>HD2;
			}
		}
	}

//Optimise arrays
	if(Generator.Optimise==true){
	//Make new arrays
		Vertex=(BT_Quadmap_Vertex*)calloc(IncVertices+1,sizeof(BT_Quadmap_Vertex));
		if(Vertex==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		Quad=(BT_Quadmap_Quad*)calloc(IncQuads,sizeof(BT_Quadmap_Quad));
		if(Quad==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
	/*
	//Exclude every single vertex
		for(Vertexn=0;Vertexn<=Vertices;Vertexn++)
			TempVertex[Vertexn].Exclude=true;

	//Include vertices which are being used
		for(Quadn=0;Quadn<Quads;Quadn++)
		{
			if(TempQuad[Quadn].Exclude==false)
			{
				TempQuad[Quadn].V1->Exclude=false;
				TempQuad[Quadn].V2->Exclude=false;
				TempQuad[Quadn].V3->Exclude=false;
				TempQuad[Quadn].V4->Exclude=false;
			}
		}
	*/
#pragma message("FINNISH THIS!!")

	//Copy vertices
		CurrVertex=0;
		for(Vertexn=0;Vertexn<=Vertices;Vertexn++){
			if(TempVertex[Vertexn].Exclude==false){
			//Position
				Vertex[CurrVertex].Pos_y=TempVertex[Vertexn].Pos_y;
				Vertex[CurrVertex].Vrow=TempVertex[Vertexn].Vrow;
				Vertex[CurrVertex].Vcol=TempVertex[Vertexn].Vcol;

			//Normals
				Vertex[CurrVertex].Nrm_x=TempVertex[Vertexn].Nrm_x;
				Vertex[CurrVertex].Nrm_y=TempVertex[Vertexn].Nrm_y;
				Vertex[CurrVertex].Nrm_z=TempVertex[Vertexn].Nrm_z;

			//Index
				Vertex[CurrVertex].Index=(unsigned short)CurrVertex;

			//Set newptr in old array so Quads can find this vertex again
				TempVertex[Vertexn].NewPtr=&Vertex[CurrVertex];

			//Check if this is the highest or lowest vertex
				if(Vertexn==HighestVertex)
					HighestVertex=CurrVertex;
				if(Vertexn==LowestVertex)
					LowestVertex=CurrVertex;

			//Increase current vertex
				CurrVertex++;
			}else{
				if(Vertexn<QuadsAccross*4)
					SegmentPointReduced[unsigned char(floor(float(Vertexn)/float(QuadsAccross)))][Vertexn-unsigned char(floor(float(Vertexn)/float(QuadsAccross)))*QuadsAccross]=true;

				IncVertices--;
			}
		}

	//Calculate new segment vertex starts
		for(unsigned short Segment=0;Segment<SegmentsPerSide*4;Segment++)
			SegmentVertexStart[Segment]=TempVertex[SegmentVertexStart[Segment]].NewPtr->Index;

	//Extra variables for copy quads function
		unsigned long QuadSize;
		unsigned long Qx;
		unsigned long Qy;
		unsigned long Quadnum;

	//Copy quads
		CurrQuad=0;
		for(Quadn=0;Quadn<Quads;Quadn++){
			if(TempQuad[Quadn].Exclude==false){
			//Update quad list
				Quad[CurrQuad].V1=TempQuad[Quadn].V1->NewPtr;
				Quad[CurrQuad].V2=TempQuad[Quadn].V2->NewPtr;
				Quad[CurrQuad].V3=TempQuad[Quadn].V3->NewPtr;
				Quad[CurrQuad].V4=TempQuad[Quadn].V4->NewPtr;

			//Copy size
				Quad[CurrQuad].Width=TempQuad[Quadn].Width;
				Quad[CurrQuad].Height=TempQuad[Quadn].Height;

			//Copy rotation
				Quad[CurrQuad].Rotation=TempQuad[Quadn].Rotation;

			//Set Quadmap
				//Get quadsize
				QuadSize=unsigned long(Quad[CurrQuad].Width/Generator.TileSize);

				//Set all the quads to this one
				for(Qy=0;Qy<QuadSize;Qy++){
					for(Qx=0;Qx<QuadSize;Qx++){
						Quadnum=Quadn+Qy+Qx*Generator.Size;
						QuadMap[Quadnum]=&Quad[CurrQuad];
					}
				}
				CurrQuad++;
			}else{
			//Decrease quadcount
				IncQuads--;
			}
		}

	//Set some numbers
		Vertices=IncVertices;
		Quads=IncQuads;

	}else{
	//Copy temp data
		Vertex=(BT_Quadmap_Vertex*)malloc((Vertices+1)*sizeof(BT_Quadmap_Vertex));
		if(Vertex==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		Quad=(BT_Quadmap_Quad*)malloc(Quads*sizeof(BT_Quadmap_Quad));
		if(Quad==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
		memcpy(Vertex,TempVertex,Vertices*sizeof(BT_Quadmap_Vertex));
		memcpy(Quad,TempQuad,Quads*sizeof(BT_Quadmap_Quad));
	}

//Say that the quadmap is generated
	Generated=true;

}

void BT_QuadMap::PostProcess()
{
	for(unsigned short i=0;i<QuadsAccross*4;i++){
		float Px=Sector->Pos_x+float(Vertex[i].Vcol*TileSize-Size/2.0);
		float Pz=Sector->Pos_z+float(Vertex[i].Vrow*TileSize-Size/2.0);
		Vector3 Normal=BT_Intern_GetPointNormal(Sector->Terrain,Px,Pz);
		Vertex[i].Nrm_x=unsigned char(Normal.x*127.0);
		Vertex[i].Nrm_y=unsigned char(Normal.y*127.0);
		Vertex[i].Nrm_z=unsigned char(Normal.z*127.0);
	}
}


s_BT_DrawBuffer* BT_QuadMap::GeneratePlain()
{
//Variables
	s_BT_DrawBuffer* DrawBuffer=NULL;

//Check that the quadmap is generated
	if(Generated==true){
	//Make draw buffer
		DrawBuffer=(s_BT_DrawBuffer*)malloc(sizeof(s_BT_DrawBuffer));
		if(DrawBuffer==nullptr)
			BT_Intern_Error(C_BT_ERROR_MEMORYERROR);

		IDirect3DDevice9* D3DDevice=dbGetDirect3DDevice(); //Must be called before doing anything with d3d

	//Generate buffers
		if(FAILED(D3DDevice->CreateVertexBuffer(Mesh_Vertices*sizeof(BT_Meshdata_Vertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&DrawBuffer->VertexBuffer,NULL)))
			BT_Intern_Error(C_BT_ERROR_CANNOTCREATEVB);

		if(FAILED(D3DDevice->CreateIndexBuffer(Mesh_Indices*sizeof(unsigned short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&DrawBuffer->IndexBuffer,NULL)))
			BT_Intern_Error(C_BT_ERROR_CANNOTCREATEIB);

		if(FAILED(D3DDevice->CreateIndexBuffer((QuadsAccross+1)*4*sizeof(unsigned short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&DrawBuffer->EdgeLineIndexBuffer,NULL)))
			BT_Intern_Error(C_BT_ERROR_CANNOTCREATEIB);

	//Set buffer parameters
		DrawBuffer->FVF_Size=sizeof(BT_Meshdata_Vertex);
		DrawBuffer->Vertices=Mesh_Vertices;
		DrawBuffer->Indices=Mesh_Indices;
		SetToMesh(DrawBuffer);

		return DrawBuffer;
	}
	return NULL;
}

void BT_QuadMap::GenerateDBPMesh(sMesh* Mesh)
{
//Variables
	bool bDeleteMeshData=false;

//Check that the quadmap is generated
	if(Generated==true){
	//Generate mesh data if its not already generated
		if(MeshMade==false){
			bDeleteMeshData=true;
			GenerateMeshData();
		}

	//Create mesh
		BT_Intern_SetupMesh(Mesh,Mesh_Vertices,Mesh_Indices,D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2);

	//Fill mesh vertexdata
		if(Mesh->pVertexData==NULL){
			Mesh->pVertexData=(BYTE*)malloc(Mesh_Vertices*sizeof(BT_Meshdata_Vertex));
			if(Mesh->pVertexData==nullptr)
				BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
			Mesh->pIndices=(WORD*)malloc(Mesh_Indices*sizeof(unsigned short));
			if(Mesh->pIndices==nullptr)
				BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
			Mesh->dwVertexCount=Mesh_Vertices;
			Mesh->dwIndexCount=Mesh_Indices;
		}

		memcpy(Mesh->pVertexData,Mesh_Vertex,Mesh_Vertices*sizeof(BT_Meshdata_Vertex));
		memcpy(Mesh->pIndices,Mesh_Index,Mesh_Indices*sizeof(unsigned short));
		Mesh->bVBRefreshRequired=true;

	//Delete mesh data if created
		if(bDeleteMeshData)
			DeleteMeshData();
	}
}

void BT_QuadMap::UpdateDBPMesh(sMesh* Mesh)
{
//Variables
	bool bDeleteMeshData=false;

//Check that the quadmap is generated
	if(Generated==true){
	//Generate mesh data if its not already generated
		if(MeshMade==false){
			bDeleteMeshData=true;
			GenerateMeshData();
		}

		memcpy(Mesh->pVertexData,Mesh_Vertex,Mesh_Vertices*sizeof(BT_Meshdata_Vertex));
		memcpy(Mesh->pIndices,Mesh_Index,Mesh_Indices*sizeof(unsigned short));
		Mesh->bVBRefreshRequired=true;

	//Delete mesh data if created
		if(bDeleteMeshData)
			DeleteMeshData();
	}
}

void BT_QuadMap::SetToMesh(s_BT_DrawBuffer* DrawBuffer)
{
//Check that the quadmap is generated
	if(Generated==true){
		bool DeleteData=false;
		if(MeshMade==false){
			DeleteData=true;
			GenerateMeshData();
		}
		BT_Intern_RefreshVB(DrawBuffer,0,Mesh_Vertices,Mesh_Vertex);
		BT_Intern_RefreshIB(DrawBuffer,0,Mesh_Indices,Mesh_Index);


	//Update Edges
		unsigned short* LockedIndex;

		//Lock indexdata
		DrawBuffer->EdgeLineIndexBuffer->Lock(0,(QuadsAccross+1)*4*sizeof(unsigned short),(void**)&LockedIndex,NULL);

		//Fill indexdata
		DrawBuffer->EdgeIndexCount=0;

		//Fill top
		int i;
		for(i=0;i<QuadsAccross;i++){
			unsigned short Vrow=0;
			unsigned short Vcol=i;
			BT_Quadmap_Quad* Quad=QuadMap[unsigned long(QuadsAccross*Vrow+Vcol)];
			if(Quad!=NULL){
				if(Quad->Exclude==false){
					LockedIndex[DrawBuffer->EdgeIndexCount]=Quad->V1->Index;
					DrawBuffer->EdgeIndexCount++;
					LockedIndex[DrawBuffer->EdgeIndexCount]=Quad->V2->Index;
					DrawBuffer->EdgeIndexCount++;
				}
			}
		}
		//Fill right
		for(i=0;i<QuadsAccross;i++){
			unsigned short Vrow=i;
			unsigned short Vcol=QuadsAccross-1;
			BT_Quadmap_Quad* Quad=QuadMap[unsigned long(QuadsAccross*Vrow+Vcol)];
			if(Quad!=NULL){
				if(Quad->Exclude==false){
					LockedIndex[DrawBuffer->EdgeIndexCount]=Quad->V2->Index;
					DrawBuffer->EdgeIndexCount++;
					LockedIndex[DrawBuffer->EdgeIndexCount]=Quad->V4->Index;
					DrawBuffer->EdgeIndexCount++;
				}
			}
		}

		//Unlock indexdata
		DrawBuffer->EdgeLineIndexBuffer->Unlock();




		DrawBuffer->Primitives=(unsigned short)Mesh_Indices/3;
		DrawBuffer->Vertices=Mesh_Vertices;
		if(DeleteData==true)
			DeleteMeshData();
	}
}

void BT_QuadMap::UpdateMesh(s_BT_DrawBuffer* DrawBuffer,bool ClearUpdateInfo)
{
//Check that the quadmap is generated
	if(Generated==true){
	//Update vertices
		if(UpdateVertexBuffer==true){
			//BT_Intern_RefreshVB(DrawBuffer,UpdateFirstVertex,UpdateLastVertex,Mesh_Vertex);
			BT_Intern_RefreshVB(DrawBuffer,0,UpdateLastVertex+1,Mesh_Vertex);
			DrawBuffer->Vertices=Mesh_Vertices;
		}

	//Update indices
		if(UpdateIndexBuffer==true){
			BT_Intern_RefreshIB(DrawBuffer,UpdateFirstIndex,UpdateLastIndex,Mesh_Index);
			DrawBuffer->Primitives=(unsigned short)Mesh_Indices/3;
		}
	}

//Clear update info
	if(ClearUpdateInfo==true){
		UpdateVertexBuffer=false;
		UpdateFirstVertex=0;
		UpdateLastVertex=0;

		UpdateIndexBuffer=false;
		UpdateFirstIndex=0;
		UpdateLastIndex=0;
	}
}


void BT_QuadMap::GenerateMeshData()
{
//Variables
	unsigned long CurrentVertex;
	unsigned long CurrentIndex;
	unsigned long CurrentQuad;

//Check that the quadmap is generated
	if(Generated==true){
	//Create mesh if not made
		if(MeshMade==false){
			Mesh_Vertex=(BT_Meshdata_Vertex*)malloc((Vertices+1)*sizeof(BT_Meshdata_Vertex));
			if(Mesh_Vertex==nullptr)
				BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
			Mesh_Index=(unsigned short*)malloc(Quads*6*sizeof(unsigned short));
			if(Mesh_Index==nullptr)
				BT_Intern_Error(C_BT_ERROR_MEMORYERROR);
			MeshMade=true;
			UpdateVertices=true;
			UpdateIndices=true;
		}

	//Update Vertices
		if(UpdateVertices==true){
			float Scale=Sector->Terrain->Scale/C_BT_INTERNALSCALE;
			float TerrainSize=Sector->Terrain->TerrainSize*Scale;
			float SectorSize=Sector->LODLevel->SectorSize*Scale;
			for(CurrentVertex=0;CurrentVertex<=Vertices;CurrentVertex++)
			{
			//Position
				Mesh_Vertex[CurrentVertex].Pos_x=float((Vertex[CurrentVertex].Vcol*TileSize-Size/2.0)/C_BT_INTERNALSCALE*Sector->Terrain->Scale);
				Mesh_Vertex[CurrentVertex].Pos_y=Vertex[CurrentVertex].Pos_y;
				Mesh_Vertex[CurrentVertex].Pos_z=float((Vertex[CurrentVertex].Vrow*TileSize-Size/2.0)/C_BT_INTERNALSCALE*Sector->Terrain->Scale);

			//Normals
				Mesh_Vertex[CurrentVertex].Nrm_x=float(Vertex[CurrentVertex].Nrm_x/127.0);
				Mesh_Vertex[CurrentVertex].Nrm_y=float(Vertex[CurrentVertex].Nrm_y/127.0);
				Mesh_Vertex[CurrentVertex].Nrm_z=float(Vertex[CurrentVertex].Nrm_z/127.0);

			//UVs
				if(Sector->Terrain->ATMode){
					Mesh_Vertex[CurrentVertex].U0=(Sector->Pos_x*Scale+Mesh_Vertex[CurrentVertex].Pos_x)/TerrainSize;
					Mesh_Vertex[CurrentVertex].V0=1.0f-(Sector->Pos_z*Scale+Mesh_Vertex[CurrentVertex].Pos_z)/TerrainSize;
				}else{
					Mesh_Vertex[CurrentVertex].U0=(Sector->Pos_x*Scale+Mesh_Vertex[CurrentVertex].Pos_x)/TerrainSize;
					Mesh_Vertex[CurrentVertex].V0=(Sector->Pos_z*Scale+Mesh_Vertex[CurrentVertex].Pos_z)/TerrainSize;
				}
				Mesh_Vertex[CurrentVertex].U1=Mesh_Vertex[CurrentVertex].U0*Sector->Terrain->LODLevel[0].Split*Sector->Terrain->Tile;
				Mesh_Vertex[CurrentVertex].V1=Mesh_Vertex[CurrentVertex].V0*Sector->Terrain->LODLevel[0].Split*Sector->Terrain->Tile;
			}
			Mesh_Vertices=Vertices;
			UpdateVertices=false;
		}

	//Update indices
		if(UpdateIndices==true){
			CurrentIndex=0;
			for(CurrentQuad=0;CurrentQuad<Quads;CurrentQuad++){
				if(Quad[CurrentQuad].Exclude==false){
					if(Quad[CurrentQuad].Rotation){
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V1->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V3->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V2->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V4->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V2->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V3->Index;
					}else{
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V1->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V3->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V4->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V1->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V4->Index;
						Mesh_Index[CurrentIndex++]=Quad[CurrentQuad].V2->Index;
					}
				}
			}
			Mesh_Indices=CurrentIndex;

		//Edges
			UpdateIndices=false;
		}
	}
}

void BT_QuadMap::DeleteMeshData()
{
//Check that the quadmap is generated
	if(Generated==true){
		if(MeshMade==true){
			MeshMade=false;
			free(Mesh_Vertex);
			free(Mesh_Index);
			Mesh_Vertices=0;
			Mesh_Indices=0;
		}
	}
}



bool BT_QuadMap::GetPointExcluded(float x,float z)
{
//Variables
	float U;
	float V;
	unsigned long Row;
	unsigned long Collumn;
	BT_Quadmap_Quad* cQuad;

//Check that the quadmap is generated
	if(Generated==true){
	//Find position on quadmap
		U=float((x+Size*0.5)/Size);
		V=float((z+Size*0.5)/Size);

	//Find row and collumn
		Row=unsigned long(floor(V*QuadsAccross));
		Collumn=unsigned long(floor(U*QuadsAccross));

	//Work out the quad number
		cQuad=QuadMap[unsigned long(QuadsAccross*Row+Collumn)];

	//Check that the quad exists
		if(cQuad==NULL)
			return true;

	//Return false
		return false;
	}
	return true;
}



float BT_QuadMap::GetPointHeight(float x,float z,bool Round)
{
//Variables
	float U;
	float V;
	unsigned short Row;
	unsigned short Collumn;
	float Px;
	float Pz;
	BT_Quadmap_Quad* cQuad;
	bool Tile;
	float Dx;
	float Dy;
	float Dz;

//Check that the quadmap is generated
	if(Generated==true){
	//Find position on quadmap
		U=float((x+Size*0.5)/Size);
		V=float((z+Size*0.5)/Size);

	//Find row and collumn
		Row=unsigned short(floor(V*QuadsAccross));
		Collumn=unsigned short(floor(U*QuadsAccross));

		if(V==1.00f)
			Row--;
		if(U==1.00f)
			Collumn--;

	//Work out the quad number
		cQuad=QuadMap[unsigned long(QuadsAccross*Row+Collumn)];

	//Check that the quad exists
		if(cQuad==NULL)
			return 0.0;

	//Scales
		float xScale=cQuad->Width/TileSize;
		float zScale=cQuad->Height/TileSize;

	//Find the position on the quad
		Px=((U*QuadsAccross)-cQuad->V1->Vcol)/xScale;
		Pz=((V*QuadsAccross)-cQuad->V1->Vrow)/zScale;

	//Work out height
		if(Round){
			return cQuad->V1->Pos_y;
		}else{
			Tile=0;
			if(cQuad->Rotation){
				Tile=!(Px<Pz);
				if(Tile==true){
					Dx=(cQuad->V4->Pos_y-cQuad->V3->Pos_y);
					Dz=(cQuad->V2->Pos_y-cQuad->V4->Pos_y);
				}else{
					Dx=(cQuad->V2->Pos_y-cQuad->V1->Pos_y);
					Dz=(cQuad->V1->Pos_y-cQuad->V3->Pos_y);
				}
				Pz=1-Pz;
				Dy=cQuad->V3->Pos_y;
			}else{
				Tile=Px>Pz;
				if(Tile==true){
					Dx=(cQuad->V2->Pos_y-cQuad->V1->Pos_y);
					Dz=(cQuad->V4->Pos_y-cQuad->V2->Pos_y);
				}else{
					Dx=(cQuad->V4->Pos_y-cQuad->V3->Pos_y);
					Dz=(cQuad->V3->Pos_y-cQuad->V1->Pos_y);
				}
				Dy=cQuad->V1->Pos_y;
			}

		//Return height
			return Dx*Px+Dz*Pz+Dy;
		}
	}
	return 0.0;
}

void BT_QuadMap::FillSegmentPointsRec(unsigned char Segment,unsigned char CurrentSpan,unsigned short CurrentLeftPoint)
{
	if(SegmentPointReduced[Segment][CurrentLeftPoint+CurrentSpan/2]==true)
		SegmentPointHeight[Segment][CurrentLeftPoint+CurrentSpan/2]=(SegmentPointHeight[Segment][CurrentLeftPoint]+SegmentPointHeight[Segment][CurrentLeftPoint+CurrentSpan])/2.0f;
		
	if(CurrentSpan>2){
		FillSegmentPointsRec(Segment,CurrentSpan/2,CurrentLeftPoint);
		FillSegmentPointsRec(Segment,CurrentSpan/2,CurrentLeftPoint+CurrentSpan/2);
	}
}

void BT_QuadMap::SetSegmentLOD(unsigned char Side,unsigned char Segment,unsigned long LODLevel)
{
//Check that the quadmap is generated
	if(Generated==true){
	//Get segment position
		unsigned char SectorSpan=Sector->Terrain->LODLevel[0].Split/Sector->LODLevel->Split;
		unsigned short SectorRow=Sector->Row*SectorSpan;
		unsigned short SectorCollumn=Sector->Column*SectorSpan;

	//Check that this edge is on the terrain
		if((Side==2 && !(SectorRow+SectorSpan<Sector->Terrain->LODLevel->Split))||(Side==1 && !(SectorCollumn+SectorSpan<Sector->Terrain->LODLevel->Split)))
			return;

	//Find segment
		bool Flip=0;
		float* SegPointHeight=0;
		if(Side==0 || Side==2){
			if(Side==0){ // 0 Top
				SegPointHeight=Sector->Terrain->TopSegPointHeight[LODLevel][SectorRow][SectorCollumn+Segment];
			}else	{	// 2 Bottom
				SegPointHeight=Sector->Terrain->BottomSegPointHeight[LODLevel][SectorRow+SectorSpan][SectorCollumn+Segment];
				Flip=1;
			}
		}else{
			if(Side==1){ // 1 Right
				SegPointHeight=Sector->Terrain->RightSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn+SectorSpan];
			}else{		// 3 Left
				SegPointHeight=Sector->Terrain->LeftSegPointHeight[LODLevel][SectorRow+Segment][SectorCollumn];
				Flip=1;
			}
		}

	//Copy segment data into edges
		unsigned short Vertexn=0;
		if(SegPointHeight!=0){
			if(Flip)
				Vertexn=SegmentVertexStart[(Side+1)*SegmentsPerSide-Segment-1];
			else
				Vertexn=SegmentVertexStart[Side*SegmentsPerSide+Segment];
			for(unsigned char Point=0;Point<PointsPerSegment;Point+=TileSpan){
				if(Vertexn!=0 && Vertexn!=QuadsAccross && Vertexn!=QuadsAccross*2 && Vertexn!=QuadsAccross*3)
					Mesh_Vertex[Vertexn].Pos_y=SegPointHeight[PointsPerSegment-Point];
				Vertexn++;
			}

		//Update vertex buffer
			if(UpdateVertexBuffer==true){
				if(UpdateFirstVertex>SegmentVertexStart[Segment])
					UpdateFirstVertex=SegmentVertexStart[Segment];
				if(UpdateLastVertex<Vertexn)
					UpdateLastVertex=Vertexn;
			}else{
				UpdateFirstVertex=SegmentVertexStart[Segment];
				UpdateLastVertex=Vertexn;
				UpdateVertexBuffer=true;
			}
		}
	}
}

void BT_QuadMap::FillMeshData(BT_RTTMS_STRUCT* Meshdata)
{
//Check that the quadmap is generated
	if(Generated==true){
		((BT_RTTMS_STRUCTINTERNALS*)Meshdata->Internals)->DeleteMeshData=false;
		Meshdata->IndexCount=0;
		Meshdata->Indices=NULL;
		Meshdata->VertexCount=0;
		Meshdata->Vertices=NULL;
		if(MeshMade==false){
			((BT_RTTMS_STRUCTINTERNALS*)Meshdata->Internals)->DeleteMeshData=true;
			GenerateMeshData();
		}
		Meshdata->IndexCount=Mesh_Indices;
		Meshdata->VertexCount=Mesh_Vertices;
		Meshdata->Vertices=(BT_RTTMS_VERTEX*)Mesh_Vertex;
		for(unsigned char Segment=0;Segment<SegmentsPerSide;Segment++){
			SetSegmentLOD(0,Segment,Sector->LODLevel->ID);SetSegmentLOD(1,Segment,Sector->LODLevel->ID);
			SetSegmentLOD(2,Segment,Sector->LODLevel->ID);SetSegmentLOD(3,Segment,Sector->LODLevel->ID);
		}
		Meshdata->Indices=Mesh_Index;
		Meshdata->ChangedAVertex=false;
		Meshdata->FirstUpdatedVertex=0;
		Meshdata->LastUpdatedVertex=0;
	}
}

void BT_QuadMap::ChangeMeshData(unsigned short VertexStart,unsigned short VertexEnd,BT_RTTMS_VERTEX* Vertices)
{
//Check that the quadmap is generated
	if(Generated==true){
	//Update vertex buffer
		if(UpdateVertexBuffer==true){
			if(UpdateFirstVertex>VertexStart)
				UpdateFirstVertex=VertexStart;
			if(UpdateLastVertex<VertexEnd)
				UpdateLastVertex=VertexEnd;
		}else{
			UpdateFirstVertex=VertexStart;
			UpdateLastVertex=VertexEnd;
			UpdateVertexBuffer=true;
		}

	//Copy data back into the quadmap structures
		for(unsigned short Vertexn=VertexStart;Vertexn<VertexEnd+1;Vertexn++)
			Vertex[Vertexn].Pos_y=Vertices[Vertexn].Pos_y;

	//Copy edgedata back into segments
		unsigned short Vertexn=0;
		for(unsigned char Segment=0;Segment<4*SegmentsPerSide;Segment++){
			for(unsigned char Point=0;Point<=PointsPerSegment;Point+=TileSpan){
				SegmentPointHeight[Segment][Point]=Vertices[Vertexn].Pos_y;
				Vertexn++;
			}
			Vertexn--;
			FillSegmentPointsRec(Segment,PointsPerSegment,0);
		}

	//Recalculate highest point
		bool Calculated=false;
		/*
		if(HighestVertex>=VertexStart && HighestVertex<=VertexEnd){
			if(Vertices[HighestVertex].Pos_y<HighestPoint){
				for(unsigned short Vertexn=0;Vertexn<Mesh_Vertices;Vertexn++){
					if(Vertexn>VertexStart && Vertexn<VertexEnd){
						if(Vertices[Vertexn].Pos_y>HighestPoint){
							HighestPoint=Vertices[Vertexn].Pos_y;
							HighestVertex=Vertexn;
						}
					}else{
						if(Mesh_Vertex[Vertexn].Pos_y>HighestPoint){
							HighestPoint=Mesh_Vertex[Vertexn].Pos_y;
							HighestVertex=Vertexn;
						}
					}
				}
				Calculated=true;
			}
		}
		*/
		HighestPoint=LowestPoint=Vertices[0].Pos_y;
		if(Calculated==false){
			//for(unsigned short Vertexn=VertexStart;Vertexn<=VertexEnd;Vertexn++)
			for(unsigned short Vertexn=1;Vertexn<=Mesh_Vertices;Vertexn++){
				if(Vertices[Vertexn].Pos_y>HighestPoint){
					HighestPoint=Vertices[Vertexn].Pos_y;
					HighestVertex=Vertexn;
				}
			}
		}

	//Recalculate lowest point
		Calculated=false;
		/*
		if(HighestVertex>=VertexStart && HighestVertex<=VertexEnd){
			if(Vertices[HighestVertex].Pos_y<HighestPoint){
				for(unsigned short Vertexn=0;Vertexn<Mesh_Vertices;Vertexn++){
					if(Vertexn>VertexStart && Vertexn<VertexEnd){
						if(Vertices[Vertexn].Pos_y<LowestPoint){
							LowestPoint=Vertices[Vertexn].Pos_y;
							LowestVertex=Vertexn;
						}
					}else{
						if(Mesh_Vertex[Vertexn].Pos_y<LowestPoint){
							LowestPoint=Mesh_Vertex[Vertexn].Pos_y;
							LowestVertex=Vertexn;
						}
					}
				}
				Calculated=true;
			}
		}
		*/
		if(Calculated==false){
			//for(unsigned short Vertexn=VertexStart;Vertexn<=VertexEnd;Vertexn++)
			for(unsigned short Vertexn=1;Vertexn<=Mesh_Vertices;Vertexn++){
				if(Vertices[Vertexn].Pos_y<LowestPoint){
					LowestPoint=Vertices[Vertexn].Pos_y;
					LowestVertex=Vertexn;
				}
			}
		}
	}
}

void BT_QuadMap::DeleteInternalData()
{
//Check that the quadmap is generated
	if(Generated==true){
	//Delete everything
		if(Vertex!=NULL)
			free(Vertex);
		if(Quad!=NULL)
			free(Quad);
		if(QuadMap!=NULL)
			free(QuadMap);
		if(Mesh_Vertex!=NULL)
			free(Mesh_Vertex);
		if(Mesh_Index!=NULL)
			free(Mesh_Index);
		for(int i=0;i<SegmentsPerSide*4;i++){
			free(SegmentPointHeight[i]);
			free(SegmentPointReduced[i]);
		}
		free(SegmentPointHeight);
		free(SegmentPointReduced);
		free(SegmentVertexStart);
	}
}

bool BT_QuadMap::ReduceQuad(unsigned short QuadTL,unsigned short QuadTR,unsigned short QuadBL,unsigned short QuadBR,BT_Quadmap_Quad* Quads,float Threshold)
{
//Check that none of them are excluded
	if(Quads[QuadTL].Exclude==true || Quads[QuadTR].Exclude==true || Quads[QuadBL].Exclude==true || Quads[QuadBR].Exclude==true)
		return false;

#pragma message("TEMPORARY CODE")
//Check that this isnt on an edge
	if(Quads[QuadTL].V1->Vrow==0 || Quads[QuadTL].V1->Vcol==0 || Quads[QuadBR].V4->Vrow==QuadsAccross || Quads[QuadBR].V4->Vcol==QuadsAccross)
		return false;

//Check that they are all the same size
	if((Quads[QuadTL].Width==Quads[QuadTR].Width) && (Quads[QuadBL].Width==Quads[QuadBR].Width) && (Quads[QuadTL].Width==Quads[QuadBL].Width)){
	//Get heights of each point
		float p11,p12,p13,p21,p22,p23,p31,p32,p33;
		p11=Quads[QuadTL].V1->Pos_y/(Sector->Terrain->YScale);
		p12=Quads[QuadTL].V2->Pos_y/(Sector->Terrain->YScale);
		p13=Quads[QuadTR].V2->Pos_y/(Sector->Terrain->YScale);
		p21=Quads[QuadTL].V3->Pos_y/(Sector->Terrain->YScale);
		p22=Quads[QuadTL].V4->Pos_y/(Sector->Terrain->YScale);
		p23=Quads[QuadTR].V3->Pos_y/(Sector->Terrain->YScale);
		p31=Quads[QuadBL].V3->Pos_y/(Sector->Terrain->YScale);
		p32=Quads[QuadBL].V4->Pos_y/(Sector->Terrain->YScale);
		p33=Quads[QuadBR].V4->Pos_y/(Sector->Terrain->YScale);

	//Work out detail
		//Accross
		float detaila1=fabs(p12-(p11+p13)/2.0f);
		float detaila2=fabs(p22-(p21+p23)/2.0f);
		float detaila3=fabs(p32-(p31+p33)/2.0f);
		//Down
		float detaild1=fabs(p21-(p11+p31)/2.0f);
		float detaild2=fabs(p22-(p12+p32)/2.0f);
		float detaild3=fabs(p23-(p13+p33)/2.0f);
		//Final value
		float detailfinal=detaila1+detaila2+detaila3+detaild1+detaild2+detaild3;

	//Check if the tile should be reduced
		if(detailfinal>Threshold)
			return false;

	}else{
		return false;
	}
	return true;
}


unsigned short BT_QuadMap::FindVertex(unsigned short Vcol,unsigned short Vrow)
{
//Check that the quadmap is generated
	if(Generated==true){
	//Find the vertex
		BT_Quadmap_Vertex* Vertex;
		if(Vrow==QuadsAccross){
			if(Vcol==QuadsAccross){
				Vertex=QuadMap[unsigned long(QuadsAccross*(Vrow-1)+(Vcol-1))]->V4;
			}else{
				Vertex=QuadMap[unsigned long(QuadsAccross*(Vrow-1)+Vcol)]->V3;
			}
		}else{
			if(Vcol==QuadsAccross){
				Vertex=QuadMap[unsigned long(QuadsAccross*Vrow+(Vcol-1))]->V2;
			}else{
				Vertex=QuadMap[unsigned long(QuadsAccross*Vrow+Vcol)]->V1;
			}
		}
	
	//Check that the vertex is in the same position
		if(Vertex->Vrow==Vrow && Vertex->Vcol==Vcol){
		//Return the index
			return Vertex->Index+1;
		}
	}

//Return nothing
	return 0;
}