# Blitzwerks Terrain Documentation

## Creation commands

### BT MAKETERRAIN

This command makes a terrain and returns its ID. After this command is called, you can
set its settings and build it. The ID returned by this function must be used in all functions
which are to do with this terrain. In DarkGDK, this doesn't need to be called when using the
terrain class as it is called automatically when the object is created.

#### Parameters:

No Parameters

#### Returns:

TerrainID (integer) - Use this ID in all commands you want to use on this terrain.

#### Syntax:

DarkBasic

```BASIC
TerrainID = BT MakeTerrain()
```

DarkGDK

```C++
int TerrainID = BT_MakeTerrain();
```

DarkGDK (OOP)

```C++
BT_Terrain Terrain;  // (Make terrain automatically called)
```

### BT SETTERRAINHEIGHTMAP

This command takes a TerrainID and an ImageID and sets the image as the terrains
heightmap. A heightmap must be set before BuildTerrain is called or BuildTerrain will fail.
Full version users can set colour heightmaps using this command for increased accuracy.
Blitzwerks Terrain will automatically detect when a colour heightmap is used. The
heightmaps image must exist until BuildTerrain is called.

#### Parameters:

- TerrainID (integer) – The ID of the terrain
- Heightmap (integer) – The image ID of the heightmap

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainHeightMap TerrainID, Heightmap
```

DarkGDK

```C++
BT_SetTerrainHeightMap(TerrainID, Heightmap);
```

DarkGDK (OOP)

```C++
Terrain->SetHeightmap(Heightmap);
```

### BT SETTERRAINTEXTURE

This command takes a TerrainID and an ImageID and sets the image as the terrains
texture. This can be called at any time on the terrain and Blitzwerks Terrain will change the
texture whenever it is called. Textures are applied to texture stage 0.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Texture (integer) – The image ID of the texture

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainTexture TerrainID, Texture
```

DarkGDK

```C++
BT_SetTerrainTexture(TerrainID, Texture);
```

DarkGDK (OOP)

```C++
Terrain->SetTexture(Texture);
```

### BT SETTERRAINDETAIL

This command takes a TerrainID and an ImageID and sets the image as the terrains
detailmap. Detailmaps are textures that get applied over the terrains base texture to
increase the detail. This can be called at any time on the terrain and Blitzwerks Terrain will
change the detailmap whenever it is called. Detailmaps are applied to texture stage 1.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Detail (integer) – The image ID of the Detailmap

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainDetail TerrainID, Detail
```

DarkGDK

```C++
BT_SetTerrainDetail (TerrainID, Detail);
```

DarkGDK (OOP)

```C++
Terrain->SetDetail (Detail);
```

### BT SETTERRAINDETAILTILE

This command sets the tiling of the detailmap. If tile is set to 1 (default), the detailmap will
be stretched across each sector. If the tile is set to 2, It will squash the detailmap so it
creates 2x2 tiles on each sector. If the tile is set to 0.5, each tile of the detailmap will take
up 4 sectors.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Tile (float) – The amount the detailmap should be tiled

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainDetailTile TerrainID, Tile
```

DarkGDK

```C++
BT_SetTerrainDetailTile(TerrainID, Tile);
```

DarkGDK (OOP)

```C++
Terrain->SetDetailTile(Tile);
```

### BT SETTERRAINDETAILBLENDMODE

This command sets the way Blitzwerks Terrain blends the detailmap with the base texture.
The mode is a number from the list of D3D Blend modes. You can view a list of blend
modes in section 5.1.

#### Parameters:

TerrainID (integer) – The ID of the terrain
BlendMode (integer) – The blend mode (look at section 5.1 for possible values)

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainDetailBlendMode TerrainID, BlendMode
```

DarkGDK

```C++
BT_SetTerrainDetailBlendMode(TerrainID, BlendMode);
```

DarkGDK (OOP)

```C++
Terrain->SetDetailBlendMode(BlendMode);
```

### BT SETTERRAINSPLIT

This command sets the amount of sectors Blitzwerks Terrain makes while building a
terrain. Splitting is needed to split up the terrain into smaller chunks called sectors. These
can be drawn separately allowing Blitzwerks Terrain to switch/hide parts of the terrain
quickly so it can do LOD and culling. This is also needed to allow large terrains to run on
old computers. If you set the split value to 2, this will split the terrain into a 2x2 grid of
sectors. The split value must be a positive power of 2 number.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Split (integer) – The amount of rows and columns to split the sectors into.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainSplit TerrainID, Split
```

DarkGDK

```C++
BT_SetTerrainSplit(TerrainID, Split);
```

DarkGDK (OOP)

```C++
Terrain->SetSplit(Split);
```

### BT SETTERRAINSCALE

This command sets the scale of the terrain on the X and Z axis. A scale of 0.5 will halve
the size of the terrain, 2.0 will Double the size of the terrain and 1.0 will not effect the
terrain. The final size of the terrain will be ``HeightmapSize(in pixels)*Scale``.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Scale (float) – The scale value

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainScale TerrainID, Scale
```

DarkGDK

```C++
BT_SetTerrainScale(TerrainID, Scale);
```

DarkGDK (OOP)

```C++
Terrain->SetScale(Scale);
```

### BT SETTERRAINYSCALE

This command sets the scale of the terrain on the Y axis. A scale of 0.5 will halve the
height of the terrain, 2.0 will Double the height of the terrain and 1.0 will not effect the
terrain. The final height of the terrain will be YScale*255.

#### Parameters:

TerrainID (integer) – The ID of the terrain
YScale (float) – The scale value

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainYScale TerrainID, YScale
```

DarkGDK

```C++
BT_SetTerrainYScale(TerrainID, YScale);
```

DarkGDK (OOP)

```C++
Terrain->SetYScale(YScale);
```

### BT SETTERRAINLOD

This command sets the number of LODLevels the terrain has. LOD changes the detail of
the terrain automatically in realtime so that far away areas of terrain have low detail and
near areas have high detail. This allows you to focus all the detail in the area the camera is
in which improves render speed and graphics quality.
You can use BT SetTerrainLODDistance (2.1.11) to control the distance which LOD Details
change.

#### Parameters:

TerrainID (integer) – The ID of the terrain
LODLevels (integer) – The number of LODLevels to create

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainLOD TerrainID, LODLevels
```

DarkGDK

```C++
BT_SetTerrainLOD(TerrainID, LODLevels);
```

DarkGDK (OOP)

```C++
Terrain->SetLOD(LODLevels);
```

### BT SETTERRAINLODDISTANCE

This command sets the distance a LOD Level appears. For example, if you have 3 LOD
levels, you will need to set LOD Distance 1 and LOD Distance 2. The highest LOD level
appears between 0 and LOD Distance 1, The middle LOD level appears between LOD
Distance 1 and LOD Distance 2 and the lowest LOD level appears between LOD Distance
2 and the cameras range. You do not need to call this function for LOD level 0.

#### Parameters:

TerrainID (integer) – The ID of the terrain
LODLevel (integer) – The LOD level to set the distance of
Value (float) – The Distance the LODLevel appears

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainLODDistance TerrainID, LODLevel, Value
```

DarkGDK

```C++
BT_SetTerrainLODDistance(TerrainID, LODLevel, Value);
```

DarkGDK (OOP)

```C++
Terrain->SetLODDistance(LODLevel, Value);
```

### BT SETTERRAINENVIRONMENT

This command sets the terrains environment map. Environment maps are maps with
coloured areas. Each coloured area is an environment for example, Grass, sand, rock, etc.
You can use the BT AddTerrainEnvironment(2.1.13) command to register different
environments and the BT GetPointEnvironment(2.5.06) command to get the environment
at a point.

#### Parameters:

TerrainID (integer) – The ID of the terrain
ImageID (integer) – The image ID of the Environment map

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainEnvironment TerrainID, ImageID
```

DarkGDK

```C++
BT_SetTerrainEnvironment(TerrainID, ImageID);
```

DarkGDK (OOP)

```C++
Terrain->SetEnvironment(ImageID);
```

### BT ADDTERRAINENVIRONMENT

This command registers an environment on a terrain and returns its ID. The colour
parameter should be set to the colour of the areas on the environment map which
represents this environment. This command returns an ID to the environment which should
be checked against the value returned by BT GetPointEnvironment(2.5.06) command to
find if this environment is at a point.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Colour (dword) – The colour of the environment on the environment map (use the rgb
command to get this)

#### Returns:

EnvironmentID(integer) – The ID for this environment

#### Syntax:

DarkBASIC

```BASIC
EnvironmentID = BT AddTerrainEnvironment(TerrainID, Colour)
```

DarkGDK

```C++
int EnvironmentID = BT_AddTerrainEnvironment(TerrainID, Colour);
```

DarkGDK (OOP)

```C++
int EnvironmentID = Terrain->AddEnvironment(Colour);
```

### BT SETTERRAINQUADREDUCTION

This command enables quad reduction. Quad reduction reduces the amount of quads in
each sector. It looks for areas which are flat and merges the quads together. This reduces
the amount of polygons in view on a terrain.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Enabled (boolean) – Set this to 1 to enable QuadReduction

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainQuadReduction TerrainID, Enabled
```

DarkGDK

```C++
BT_SetTerrainQuadReduction(TerrainID, Enabled);
```

DarkGDK (OOP)

```C++
Terrain->SetQuadReduction(Enabled);
```

### BT SETTERRAINQUADROTATION

This command enables quad rotation. Quad rotation rotates the triangles in each quad in a
way to prevent the triangles from sticking out causing jagged edges all over your terrain. It
makes the terrain look a lot smoother.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Enabled (boolean) – Set this to 1 to enable QuadRotation

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainQuadRotation TerrainID, Enabled
```

DarkGDK

```C++
BT_SetTerrainQuadRotation(TerrainID, Enabled);
```

DarkGDK (OOP)

```C++
Terrain->SetQuadRotation(Enabled);
```

### BT SETTERRAINSMOOTHING

This command enables smoothing on the terrain. When enabled, smoothing averages
each point with the points around it to smooth out sharp edges on terrains. It also helps
reduce stepping caused by inaccurate heightmaps.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Enabled (boolean) – Set this to 1 to enable QuadRotation

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainQuadRotation TerrainID, Enabled
```

DarkGDK

```C++
BT_SetTerrainQuadRotation(TerrainID, Enabled);
```

DarkGDK (OOP)

```C++
Terrain->SetQuadRotation(Enabled);
```

### BT SETTERRAINEXCLUSION

This command sets the terrains exclusion map. Exclusion maps are greyscale maps which
culls quads at points where the brightness is less than the exclusion threshold. You can set
the exclusion threshold with the BT SetTerrainExclusionThreshold(2.1.18) command.
Exclusion is useful for culling quads which are under buildings or underwater.

#### Parameters:

TerrainID (integer) – The ID of the terrain
ImageID (integer) – The image ID of the Exclusion map

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainExclusion TerrainID, ImageID
```

DarkGDK

```C++
BT_SetTerrainExclusion(TerrainID, ImageID);
```

DarkGDK (OOP)

```C++
Terrain->SetExclusion(ImageID);
```

### BT SETTERRAINEXCLUSIONTHRESHOLD

This command sets the brightness level a point on the exclusion map needs to be less
than in order to be culled. This is useful for setting the height of the water on your map and
using the heightmap as the exclusion map.

#### Parameters:

TerrainID (integer) – The ID of the terrain
Threshold (integer) – The threshold. This must be between 0 and 255

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetTerrainExclusionThreshold TerrainID, Threshold
```

DarkGDK

```C++
BT_SetTerrainExclusionThreshold(TerrainID, Threshold);
```

DarkGDK (OOP)

```C++
Terrain->SetExclusionThreshold(Threshold);
```

#### Building commands

### BT SETBUILDSTEP

This command sets the number of sectors that get built in each call of
BT ContinueBuild(2.2.04).

#### Parameters:

BuildStep(integer) – The number of sectors to build.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetBuildStep BuildStep
```

DarkGDK

```C++
BT_SetBuildStep(BuildStep);
```

DarkGDK (OOP)

```C++
Blitzwerks Terrain->SetBuildStep(BuildStep);
```

### BT BUILDTERRAIN

This command builds a terrain. The ObjectID must not be 0 and must be unused. This is
the ID Blitzwerks Terrain will create its object into. You can use DarkBASIC functions on this ID
to allow you to use all the object functions on the terrain. If build fully is set to 1, The terrain
will be built inside this call and the terrain will be ready for use without a call to
ContinueBuild(2.2.04).

#### Parameters:

TerrainID(integer) – The ID of the terrain
ObjectID(integer) – An unused object id for the terrains object
BuildFully(boolean) – optional – Set this to 1 to make the terrain build completely inside
this call.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT BuildTerrain TerrainID, ObjectID
```
BT BuildTerrain TerrainID, ObjectID, BuildFully

DarkGDK

```C++
BT_BuildTerrain(TerrainID, ObjectID);
```
BT_BuildTerrain(TerrainID, ObjectID, BuildFully);

DarkGDK (OOP)

```C++
Terrain->Build(ObjectID);
```
Terrain->Build(ObjectID, BuildFully);

### BT LOADTERRAIN

This command loads a terrain from a file. This will do everything between
BT MakeTerrain(2.1.01) and BT BuildTerrain(2.2.02). You will have to apply your texture
and detailmap again as these are not saved inside the terrain file.

#### Parameters:

Filename(string) – The filename of the terrain
ObjectID(integer) – An unused object id for the terrains object
BuildFully(boolean) – optional – Set this to 1 to make the terrain build completely inside
this call.

#### Returns:

TerrainID(integer) - Use this ID in all commands you want to use on this terrain.

#### Syntax:

DarkBASIC

```BASIC
BT LoadTerrain FileName, ObjectID
```
BT LoadTerrain FileName, ObjectID, BuildFully

DarkGDK

```C++
BT_LoadTerrain(FileName, ObjectID);
```
BT_LoadTerrain(FileName, ObjectID, BuildFully);

DarkGDK (OOP)

```C++
Blitzwerks Terrain->LoadTerrain(FileName, ObjectID);
```
Blitzwerks Terrain->LoadTerrain(FileName, BuildFully);

### BT CONTINUEBUILD

This command generates sectors for the terrain currently being built. You can change the
number of sectors it builds each call by using the BT SetBuildStep(2.2.01) command. This
command is useful for creating loading bars for your program. It returns the current
percentage of how much terrain has been built. If it returns -1 then the terrain is built and
can be used.

#### Parameters:

No Parameters

#### Returns:

Progress(integer) – The percentage of terrain built, or -1 if the terrain is built.

#### Syntax:

DarkBASIC

```BASIC
Progress = BT ContinueBuild()
```

DarkGDK

```C++
int Progress = BT_ContinueBuild();
```

DarkGDK (OOP)

```C++
int Progress = Terrain->ContinueBuild();
```

#### Updating commands

### BT SETCURRENTCAMERA

This command sets the camera which should be used for culling terrains and updating
LOD. This is useful when multiple cameras are used, Blitzwerks Terrain can update and
render terrains individually for each camera instead of displaying everything that every
camera can see to every camera. This greatly improves speed. It also allows for Blitzwerks
Terrain to update LOD individually for cameras which are in different locations.

#### Parameters:

CameraID(integer) – The DarkBASIC camera to make Blitzwerks Terrain use.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetCurrentCamera CameraID
```

DarkGDK

```C++
BT_SetCurrentCamera(CameraID);
```

DarkGDK (OOP)

```C++
Blitzwerks Terrain->SetCurrentCamera(CameraID);
```

### BT UPDATETERRAINCULL

This command updates the culling for a terrain. Culling checks which parts of the terrain
are in the cameras view. The parts which are not get removed temporarily to improve
render speed. The cull area is taken from the current camera (set through BT
SetCurrentCamera(2.3.01)).

#### Parameters:

TerrainID(integer) – The ID of the terrain you want to update the cull for

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UpdateTerrainCull TerrainID
```

DarkGDK

```C++
BT_UpdateTerrainCull(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->UpdateCull();
```

### BT UPDATETERRAINLOD

This command updates the LOD for a terrain. This doesn't need to be called when there is
only 1 LOD level. LOD reduces the detail in far away areas improving render speed which
allows you to increase the detail/size of your terrains with very little impact on frame rate.
The LOD position is taken from the current cameras position (set through BT
SetCurrentCamera(2.3.01)).

#### Parameters:

TerrainID(integer) – The ID of the terrain you want to update the LOD for

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UpdateTerrainLOD TerrainID
```

DarkGDK

```C++
BT_UpdateTerrainLOD(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->UpdateLOD();
```

### BT UPDATETERRAIN

This function is BT UpdateTerrainCull(2.3.02) and BT UpdateTerrainLOD(2.3.03) put
together. It does not need to be called if you use these 2 functions instead.

#### Parameters:

TerrainID(integer) – The ID of the terrain you want to update

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UpdateTerrain TerrainID
```

DarkGDK

```C++
BT_UpdateTerrain(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->Update();
```

### BT RENDERTERRAIN

This command draws the terrain to the current camera. This must be called every loop
before sync is called.

#### Parameters:

TerrainID(integer) – The ID of the terrain you want to render

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT RenderTerrain TerrainID
```

DarkGDK

```C++
BT_RenderTerrain(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->Render();
```

#### Sculpting commands

### BT LOCKVERTEXDATAFORSECTOR

This command locks the vertexdata of a sector. It allows you to use the other vertexdata
commands on this sector. You can only lock one sector at a time and sectors are
automatically unlocked in the sync command.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT LockVertexDataForSector TerrainID, LODLevel, Sector
```

DarkGDK

```C++
BT_LockVertexDataForSector(TerrainID, LODLevel, Sector);
```

DarkGDK (OOP)

N/A

### BT LOCKEDASECTOR

This command returns 1 if a sector is locked and 0 if there are no locked sectors.

#### Parameters:

No parameters

#### Returns:

Sector locked(boolean) - 1 if there is a locked sector, otherwise 0

#### Syntax:

DarkBASIC

```BASIC
SectorLocked = BT LockedASector()
```

DarkGDK

```C++
int SectorLocked = BT_LockedASector();
```

DarkGDK (OOP)

N/A

### BT GETLOCKEDTERRAIN

If a sector has been locked, this returns the ID of the terrain which the locked sector
belongs to

#### Parameters:

No parameters

#### Returns:

TerrainID(integer) - The ID of the terrain the locked sector is in

#### Syntax:

DarkBASIC

```BASIC
TerrainID = BT GetLockedTerrain()
```

DarkGDK

```C++
int TerrainID = BT_GetLockedTerrain();
```

DarkGDK (OOP)

N/A

### BT GETLOCKEDLODLEVEL

If a sector has been locked, this returns the ID of the LOD level on the terrain which the
locked sector belongs to

#### Parameters:

No parameters

#### Returns:

LODLevel(integer) - The ID of the LOD level the locked sector is in

#### Syntax:

DarkBASIC

```BASIC
LODLevel = BT GetLockedLODLevel()
```

DarkGDK

```C++
int LODLevel = BT_GetLockedLODLevel();
```

DarkGDK (OOP)

N/A

### BT GETLOCKEDSECTOR

If a sector has been locked, this returns the ID of the locked sector on the terrain which the
locked sector belongs to.

#### Parameters:

No parameters

#### Returns:

SectorID(integer) - The ID of the locked sector

#### Syntax:

DarkBASIC

```BASIC
SectorID = BT GetLockedSector()
```

DarkGDK

```C++
int SectorID = BT_GetLockedSector();
```

DarkGDK (OOP)

N/A

### BT UNLOCKVERTEXDATA

This command unlocks the locked sector. All edited vertices will automatically change on
the terrain.

#### Parameters:

No parameters
Returns Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UnlockVertexdata()
```

DarkGDK

```C++
BT_UnlockVertexdata();
```

DarkGDK (OOP)

N/A

### BT GETVERTEXCOUNT

This command returns the number of vertices in the current locked sector.

#### Parameters:

No parameters

#### Returns:

VertexCount(integer) - The number of vertices in the locked sector

#### Syntax:

DarkBASIC

```BASIC
VertexCount = BT GetVertexCount()
```

DarkGDK

```C++
int VertexCount = BT_GetVertexCount();
```

DarkGDK (OOP)

N/A

### BT GETINDEXCOUNT

This command returns the number of indices in the current locked sector.

#### Parameters:

No parameters

#### Returns:

IndexCount(integer) - The number of indices in the locked sector

#### Syntax:

DarkBASIC

```BASIC
IndexCount = BT GetIndexCount()
```

DarkGDK

```C++
int IndexCount = BT_GetIndexCount();
```

DarkGDK (OOP)

N/A

### BT GETINDEX

This command returns the value of the specified index on the current locked sector. There
are 3 indices per triangle. Each index is a vertex number. For example, the 3 vertices for
the first triangle in the sector can be found by doing, BT GetIndex(0), BT GetIndex(1) and
BT GetIndex(2). You can use the numbers returned by these in the vertex commands to
change each vertex on this triangle.

#### Parameters:

IndexID(integer) – The ID of the index

#### Returns:

VertexID(integer) – The vertex at this index

#### Syntax:

DarkBASIC

```BASIC
VertexID = BT GetIndex(IndexID)
```

DarkGDK

```C++
int VertexID = BT_GetIndex(IndexID);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONX

This command returns the x coordinate of the specified vertex on the current locked
sector.

#### Parameters:

VertexID(integer) – The ID of the vertex

#### Returns:

XCoordinate(float) – The vertex X position

#### Syntax:

DarkBASIC

```BASIC
XCoordinate = BT GetVertexPositionX(VertexID)
```

DarkGDK

```C++
float XCoordinate = BT_GetVertexPositionX(VertexID);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONY

This command returns the y coordinate of the specified vertex on the current locked
sector.

#### Parameters:

VertexID(integer) – The ID of the vertex

#### Returns:

YCoordinate(float) – The vertex Y position

#### Syntax:

DarkBASIC

```BASIC
YCoordinate = BT GetVertexPositionY(VertexID)
```

DarkGDK

```C++
float YCoordinate = BT_GetVertexPositionY(VertexID);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONZ

This command returns the z coordinate of the specified vertex on the current locked
sector.

#### Parameters:

VertexID(integer) – The ID of the vertex

#### Returns:

ZCoordinate(float) – The vertex Z position

#### Syntax:

DarkBASIC

```BASIC
ZCoordinate = BT GetVertexPositionZ(VertexID)
```

DarkGDK

```C++
float ZCoordinate = BT_GetVertexPositionZ(VertexID);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONX

This command does the same as BT GetVertexPositionX(2.4.10) but this addresses each
vertex by its row and column.

#### Parameters:

Row(integer) – The row of the vertex
Column(integer) – The column of the vertex

#### Returns:

XCoordinate(float) – The vertex X position

#### Syntax:

DarkBASIC

```BASIC
XCoordinate = BT GetVertexPositionX(Row, Column)
```

DarkGDK

```C++
float XCoordinate = BT_GetVertexPositionX(Row, Column);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONY

This command does the same as BT GetVertexPositionY(2.4.11) but this addresses each
vertex by its row and column.

#### Parameters:

Row(integer) – The row of the vertex
Column(integer) – The column of the vertex

#### Returns:

YCoordinate(float) – The vertex Y position

#### Syntax:

DarkBASIC

```BASIC
YCoordinate = BT GetVertexPositionY(Row, Column)
```

DarkGDK

```C++
float YCoordinate = BT_GetVertexPositionY(Row, Column);
```

DarkGDK (OOP)

N/A

### BT GETVERTEXPOSITIONZ

This command does the same as BT GetVertexPositionZ(2.4.12) but this addresses each
vertex by its row and column.

#### Parameters:

Row(integer) – The row of the vertex
Column(integer) – The column of the vertex

#### Returns:

ZCoordinate(float) – The vertex Z position

#### Syntax:

DarkBASIC

```BASIC
ZCoordinate = BT GetVertexPositionZ(Row, Column)
```

DarkGDK

```C++
float ZCoordinate = BT_GetVertexPositionZ(Row, Column);
```

DarkGDK (OOP)

N/A

### BT SETVERTEXHEIGHT

This command sets the height of a vertex (Y coordinate). You can use this to modify the
sector in realtime.

#### Parameters:

VertexID(integer) – The ID of the vertex
Height(float) – The new height of the vertex

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetVertexHeight VertexID, Height
```

DarkGDK

```C++
BT_SetVertexHeight(VertexID, Height);
```

DarkGDK (OOP)

N/A

### BT SETVERTEXHEIGHT

This command does the same as BT SetVertexHeight(2.4.16) but this addresses each
vertex by its row and column.

#### Parameters:

Row(integer) – The row of the vertex
Column(integer) – The column of the vertex
Height(float) – The new height of the vertex

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetVertexHeight Row, Column, Height
```

DarkGDK

```C++
BT_SetVertexHeight(Row, Column, Height);
```

DarkGDK (OOP)

N/A

### BT SETPOINTHEIGHT

This command sets the height of a point on the terrain. You do not need to lock any
sectors to use this function. It will set the height on multiple sectors if the point is on the
edge of a sector, and it will also set the height through all LOD levels. This is the
recommended method of setting the height of points but is a bit slower than doing it using
the above functions.

#### Parameters:

TerrainID(integer) – The terrain which the vertex is on
Row(integer) – The row of the vertex
Column(integer) – The column of the vertex
Height(float) – The new height of the vertex

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetPointHeight TerrainID, Row, Column, Height
```

DarkGDK

```C++
BT_SetPointHeight(TerrainID, Row, Column, Height);
```

DarkGDK (OOP)

N/A

### BT RAISETERRAIN

This command raises a circular piece of terrain at the point specified. It will raise the
middle point the most and the points near the radius the least.
This command uses the following function on each point in the radius:
``NewHeight = CurrentHeight + ((cos(float((DistanceFromCentre * 1.57079633) / (Radius / 2.0f))) + 1.0f) / 2.0f) * Amount``

You can turn this function into a lower terrain function by using a negative amount.

#### Parameters:

TerrainID(integer) – The terrain which the vertex is on
X(float) – The X position of the middle of the brush
Z(float) – The Z position of the middle of the brush
Radius(float) – The radius of the brush
Amount(float) – The amount the middle point should move

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT RaiseTerrain TerrainID, X, Z, Radius, Amount
```

DarkGDK

```C++
BT_RaiseTerrain(TerrainID, X, Z, Radius, Amount);
```

DarkGDK (OOP)

N/A

### BT FLATTENTERRAIN

This function flattens a circular area of terrain. It sets the height of all the vertices in the
radius to the height of the middle point.

#### Parameters:

TerrainID(integer) – The terrain which the vertex is on
X(float) – The X position of the middle of the brush
Z(float) – The Z position of the middle of the brush
Radius(float) – The radius of the brush

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT FlattenTerrain TerrainID, X, Z, Radius
```

DarkGDK

```C++
BT_FlattenTerrain(TerrainID, X, Z, Radius);
```

DarkGDK (OOP)

N/A

### BT PAINTTERRAIN

This function sets the colour of an area of terrain. You can either set the whole are to a
fixed colour or to random colours in a range.

#### Parameters:

TerrainID(integer) – The terrain which to paint
X(float) – The X position of the middle of the brush
Z(float) – The Z position of the middle of the brush
Radius(float) – The radius of the brush
Colour(dword) – The colour to set to this area
MinColour(dword) – The minimum colour of the range to set to this area
MaxColour(dword) – The maximum colour of the range to set to this area

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT PaintTerrain TerrainID, X, Z, Radius, Colour
```
BT PaintTerrain TerrainID, X, Z, Radius, MinColour, MaxColour

DarkGDK

```C++
BT_PaintTerrain(TerrainID, X, Z, Radius, Colour);
```
BT_PaintTerrain(TerrainID, X, Z, Radius, MinColour, MaxColour);

DarkGDK (OOP)

N/A

### BT SETPOINTCOLOUR

This function sets the colour of a point on a terrain.

#### Parameters:

TerrainID(integer) – The terrain which the point is on
X(float) – The X position of the middle of the brush
Z(float) – The Z position of the middle of the brush
Colour(dword) – The colour to set this point to

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetPointColour TerrainID, X, Z, Colour
```

DarkGDK

```C++
BT_SetPointColour(TerrainID, X, Z, Colour);
```

DarkGDK (OOP)

N/A

### BT UPDATETERRAINTEXTURE

This function applies the changes to the texture to all mip levels of the texture. You only
need to call this once between the last texture modification and the sync command. You do
not need to call this when there are no other mip levels.

#### Parameters:

TerrainID(integer) – The terrain with the texture that needs updating

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UpdateTerrainTexture TerrainID
```

DarkGDK

```C++
BT_UpdateTerrainTexture(TerrainID);
```

DarkGDK (OOP)

N/A

2.5 Getting commands
### BT GETGROUNDHEIGHT

This function gets the height of a specified point. You can use this for positioning objects
on a terrain.

#### Parameters:

TerrainID(integer) – The terrain which the point is on
X(float) – The points X coordinate
Z(float) – The points Z coordinate

#### Returns:

Height(float) – The Height of the point

#### Syntax:

DarkBASIC

```BASIC
Height = BT GetGroundHeight(TerrainID, X, Z)
```

DarkGDK

```C++
float Height = BT_GetGroundHeight(TerrainID, X, Z);
```

DarkGDK (OOP)

```C++
float Height = Terrain->GetGroundHeight(X, Z);
```

### BT GETPOINTEXCLUDED

If an exclusion map has been applied. This function checks if a point is excluded. This can
be used to make sure objects aren't positioned in areas which don't exist.

#### Parameters:

TerrainID(integer) – The terrain which the point is on
X(float) – The points X coordinate
Z(float) – The points Z coordinate

#### Returns:

Excluded(boolean) – returns 1 if the point is excluded, otherwise returns 0

#### Syntax:

DarkBASIC

```BASIC
Excluded =BT GetPointExcluded(TerrainID, X, Z)
```

DarkGDK

```C++
bool Excluded = BT_GetPointExcluded(TerrainID, X, Z);
```

DarkGDK (OOP)

```C++
bool Excluded = Terrain->GetPointExcluded(X, Z);
```

### BT GETSECTORCOUNT

This function gets the number of sectors on the specified LOD level of the specified terrain.
This would usually be: (Split/2^LODLevel)^2

#### Parameters:

TerrainID(integer) – The terrain ID
LODLevel(integer) – The LOD level to get the sector count of. Starting from 0 (highest)

#### Returns:

SectorCount(integer) – The number of sectors on this LOD level

#### Syntax:

DarkBASIC

```BASIC
SectorCount =BT GetSectorCount(TerrainID, LODLevel)
```

DarkGDK

```C++
int SectorCount = BT_GetSectorCount(TerrainID, LODLevel);
```

DarkGDK (OOP)

```C++
int SectorCount = Terrain->GetSectorCount(LODLevel);
```

### BT TERRAINEXIST

This function finds if the specified terrain has been made with BT MakeTerrain(2.1.01) but
not deleted with BT DeleteTerrain(2.6.01).

#### Parameters:

TerrainID(integer) – The terrain ID

#### Returns:

Exists(boolean) – returns1 if the terrain exists, otherwise returns 0

#### Syntax:

DarkBASIC

```BASIC
Exists =BT TerrainExist(TerrainID)
```

DarkGDK

```C++
bool Exists = BT_TerrainExist(TerrainID);
```

DarkGDK (OOP)

N/A

### BT GETTERRAINOBJECTID

This function returns the ID of the terrains object which was set in BT BuildTerrain(2.2.02).

#### Parameters:

TerrainID(integer) – The terrain ID

#### Returns:

ObjectID(integer) – The DarkBASIC object for the terrain

#### Syntax:

DarkBASIC

```BASIC
ObjectID = BT GetTerrainObjectID(TerrainID)
```

DarkGDK

```C++
int ObjectID = BT_GetTerrainObjectID(TerrainID);
```

DarkGDK (OOP)

```C++
int ObjectID = Terrain->GetObjectID();
```

### BT GETPOINTENVIRONMENT

If an environment map has been applied, this function will return the ID of the environment
at a point. If the Environment at this point has not been added with
BT AddTerrainEnvironment(2.1.13), this will return 0.

#### Parameters:

TerrainID(integer) – The terrain which the point is on
X(float) – The points X coordinate
Z(float) – The points Z coordinate

#### Returns:

Environment(integer) – The environment ID at this point

#### Syntax:

DarkBASIC

```BASIC
Environment = BT GetPointEnvironment(TerrainID, X, Z)
```

DarkGDK

```C++
int Environment = BT_GetPointEnvironment(TerrainID, X, Z);
```

DarkGDK (OOP)

N/A

### BT GETTERRAINSIZE

This function gets the total size of a terrain in DarkBASIC units. You can use this to make sure
objects don't go off the edge of the terrain

#### Parameters:

TerrainID(integer) – The terrain to get the size of

#### Returns:

Size(float) – The total size of the terrain in DarkBASIC units

#### Syntax:

DarkBASIC

```BASIC
Size = BT GetTerrainSize(TerrainID)
```

DarkGDK

```C++
float Size = BT_GetTerrainSize(TerrainID);
```

DarkGDK (OOP)

```C++
float Size = Terrain->GetSize();
```

### BT GETSECTORPOSITIONX

This gets the X position in DarkBASIC units of the specified sector.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector

#### Returns:

X Position(float) – The X position in DarkBASIC units of the sector.

#### Syntax:

DarkBASIC

```BASIC
XPosition = BT GetSectorPositionX(TerrainID, LODLevel, Sector)
```

DarkGDK

```C++
float XPosition = BT_GetSectorPositionX(TerrainID, LODLevel, Sector)
```

DarkGDK (OOP)

```C++
float XPosition = Terrain->GetSectorPositionX(LODLevel, Sector)
```

### BT GETSECTORPOSITIONY

This gets the Y position in DarkBASIC units of the specified sector.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector

#### Returns:

Y Position(float) – The Y position in DarkBASIC units of the sector.

#### Syntax:

DarkBASIC

```BASIC
YPosition = BT GetSectorPositionY(TerrainID, LODLevel, Sector)
```

DarkGDK

```C++
float YPosition = BT_GetSectorPositionY(TerrainID, LODLevel, Sector)
```

DarkGDK (OOP)

```C++
float Yposition = Terrain->GetSectorPositionY(LODLevel, Sector)
```

### BT GETSECTORPOSITIONZ

This gets the Z position in DarkBASIC units of the specified sector.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector

#### Returns:

Z Position(float) – The Z position in DarkBASIC units of the sector.

#### Syntax:

DarkBASIC

```BASIC
ZPosition = BT GetSectorPositionZ(TerrainID, LODLevel, Sector)
```

DarkGDK

```C++
float ZPosition = BT_GetSectorPositionZ(TerrainID, LODLevel, Sector)
```

DarkGDK (OOP)

```C++
float ZPosition = Terrain->GetSectorPositionZ(LODLevel, Sector)
```

### BT GETSECTOREXCLUDED

This command checks if the specified sector is excluded. Sectors can be excluded if every
single quad on the sector is excluded with an exclusion map. You can use this function to
make sure the sector exists before trying to use one of the other sector commands on it.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector

#### Returns:

Excluded(boolean) – 1 if the sector is excluded, 0 if its not excluded.

#### Syntax:

DarkBASIC

```BASIC
Excluded = BT GetSectorExcluded(TerrainID, LODLevel, Sector)
```

DarkGDK

```C++
bool Excluded = BT_GetSectorExcluded(TerrainID, LODLevel, Sector)
```

DarkGDK (OOP)

```C++
bool Excluded =Terrain->GetSectorExcluded(LODLevel, Sector)
```

2.6 Miscellaneous Terrain commands
### BT DELETETERRAIN

This command deletes a terrain. The terrain must be past the BT MakeTerrain(2.1.01)
stage in order to be deleted. When terrains are deleted their ID will be re used if a new
terrain is created afterwards.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT DeleteTerrain(TerrainID)
```

DarkGDK

```C++
BT_DeleteTerrain(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->Delete(); or delete Terrain;
```

### BT MAKESECTOROBJECT

This command creates a DarkBASIC object from the vertex data of the specified sector. These
objects should only be used for getting vertex data from a terrain and setting up collision.
They should not be used for visuals.

#### Parameters:

TerrainID(integer) – The ID of the terrain the sector is on
LODLevel(integer) – The LODLevel of the terrain the sector is on
Sector(integer) – The ID of the sector
ObjectID(integer) – The DarkBASIC object ID to create the sector object in.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT MakeSectorObject(TerrainID, LODLevel, Sector, ObjectID)
```

DarkGDK

```C++
BT_MakeSectorObject(TerrainID, LODLevel, Sector, ObjectID);
```

DarkGDK (OOP)

```C++
Terrain->MakeSectorObject(LODLevel, Sector, ObjectID);
```

### BT SAVETERRAIN

This command saves a terrain into Blitzwerks Terrains native format. The file extension
can be anything you want. The files produced can only be loaded with BT LoadTerrain. If
you have the full version of Blitzwerks Terrain and save a terrain which uses full version
features. It cannot be loaded into the free version of Blitzwerks Terrain.

#### Parameters:

TerrainID(integer) – The ID of the terrain to save
FileName(string) – The filename to save the terrain as

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SaveTerrain(TerrainID, FileName)
```

DarkGDK

```C++
BT_SaveTerrain(TerrainID, FileName);
```

DarkGDK (OOP)

```C++
Terrain->Save(FileName);
```

### BT MAKETERRAINOBJECT

This command creates a DarkBASIC object from the vertex data of the specified terrain. These
objects should only be used for getting vertex data from a terrain and setting up collision.
They should not be used for visuals.

#### Parameters:

TerrainID(integer) – The ID of the terrain
LODLevel(integer) – The LODLevel to use for the object
ObjectID(integer) – The DarkBASIC object ID to create the terrain object in.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT MakeTerrainObject(TerrainID, LODLevel, ObjectID)
```

DarkGDK

```C++
BT_MakeTerrainObject(TerrainID, LODLevel, ObjectID);
```

DarkGDK (OOP)

```C++
Terrain->MakeObject(LODLevel, ObjectID);
```

### BT UPDATETERRAINOBJECTS

This command repositions the DarkBASIC objects of a terrain (created with
BT MakeSectorObject and BT MakeTerrainObject) to the same position as the terrain. This
makes repositioning terrains with collision objects a lot easier.

#### Parameters:

TerrainID(integer) – The ID of the terrain

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT UpdateTerrainObjects TerrainID
```

DarkGDK

```C++
BT_UpdateTerrainObjects(TerrainID);
```

DarkGDK (OOP)

```C++
Terrain->UpdateObjects();
```

2.7 Miscellaneous commands
### BT SETATMODE

This command makes Blitzwerks Terrain emulate Advanced Terrains method of loading
heightmaps and applying textures. This should be used if you are using a heightmap
designed to work with Advanced Terrain. Advanced Terrain rotates and mirrors the terrain
but Blitzwerks Terrain doesn't. This command solves this issue.

#### Parameters:

Enabled(boolean) – Set this to 1 to enable AT mode. Set this to 0 to disable it.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT SetATMode(Enabled)
```

DarkGDK

```C++
BT_SetATMode(Enabled);
```

DarkGDK (OOP)

```C++
Blitzwerks Terrain->SetATMode(Enabled);
```

### BT GETVERSION

This command returns a string with the version of Blitzwerks Terrain that is currently being
used.

#### Parameters:

No Parameters

#### Returns:

Version(string) – A string with the version info. (char* in C++)

#### Syntax:

DarkBASIC

```BASIC
Version$ = BT GetVersion()
```

DarkGDK

```C++
char* Version = BT_GetVersion();
```

DarkGDK (OOP)

```C++
char* Version = Blitzwerks Terrain->GetVersion();
```

### BT GETSTATISTIC

This command returns a statistic. The only parameter is the statistic code which sets which
statistic to return. The values returned is the amount of work Blitzwerks Terrain has done
so far in its render loop. To get statistics of all the terrains put together, I recommend that
you call this after rendering all the terrains and before sync is called.
Code

#### Returns:

 - 1 Polygon count
 - 2 Draw calls
 - 3 Cull checks

#### Example:

DarkBASIC:

```BASIC
PolygonCount = BT GetStatistic(1)
DrawCalls = BT GetStatistic(2)
CullChecks = BT GetStatistic(2)
```

DarkGDK:

```C++
int PolygonCount = BT_GetStatistic(1);
int DrawCalls = BT_GetStatistic(2);
int CullChecks = BT_GetStatistic(3);
```

DarkGDK (OOP):

```C++
int PolygonCount = BT_Main::GetStatistic(1);
int DrawCalls = BT_Main::GetStatistic(2);
int CullChecks = BT_Main::GetStatistic(3);
```

### BT ENABLEAUTORENDER

This command enables automatic rendering. When this command is called, you do not
have to call any updating or rendering functions inside your loop.

#### Parameters:

Enabled(bool) – Set this to 1 to enable auto render, 0 to disable auto render.

#### Returns:

Nothing

#### Syntax:

DarkBASIC

```BASIC
BT EnableAutoRender 1
```

DarkGDK

```C++
BT_EnableAutoRender(1);
```

DarkGDK (OOP)

```C++
Blitzwerks Terrain->EnableAutoRender(1);
```
