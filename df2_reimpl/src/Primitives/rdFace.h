#ifndef _RDFACE_H
#define _RDFACE_H

#include "Engine/rdMaterial.h"
#include "Primitives/rdVector.h"

#define rdFace_New_ADDR       (0x0046D150)
#define rdFace_NewEntry_ADDR  (0x0046D1A0)
#define rdFace_Free_ADDR      (0x0046D1E0)
#define rdFace_FreeEntry_ADDR (0x0046D220)

typedef struct rdVertexIdxInfo
{
    uint32_t numVertices;
    int* vertexPosIdx;
    int* vertexUVIdx;
    rdVector3* vertices;
    rdVector2* extraUV;
    uint32_t field_14;
    uint32_t field_18;
} rdVertexIdxInfo;

typedef struct rdFace
{
    int num;
    int type;
    int geometryMode;
    int lightingMode;
    int textureMode;
    int numVertices;
    int* vertexPosIdx;
    int* vertexUVIdx;
    rdMaterial* material;
    int field_24;
    rdVector2 field_28;
    float extralight;
    rdVector3 normal;
} rdFace;

rdFace *rdFace_New();
int rdFace_NewEntry(rdFace* out);
void rdFace_Free(rdFace *face);
void rdFace_FreeEntry(rdFace *face);

#endif // _RDFACE_H