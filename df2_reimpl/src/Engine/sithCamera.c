#include "sithCamera.h"

#include "World/sithSector.h"
#include "World/sithThing.h"
#include "World/sithWorld.h"
#include "World/jkPlayer.h"
#include "World/sithUnk3.h"
#include "Primitives/rdMath.h"
#include "Engine/sithTime.h"
#include "jk.h"

static rdVector3 sithCamera_trans = {0.0, 0.3, 0.0};
static rdVector3 sithCamera_trans2 = {0.0, 0.2, 0.0};
static rdVector3 sithCamera_trans3 = {0.0, 1.0, 1.0};

void sithCamera_FollowFocus(sithCamera *cam)
{
    rdVector3 mode64Tmp;
    rdVector3 v76;
    rdVector3 v2; // [esp+2Ch] [ebp-60h] BYREF
    rdVector3 a1; // [esp+38h] [ebp-54h] BYREF
    rdVector3 v84; // [esp+44h] [ebp-48h] BYREF
    rdVector3 rot; // [esp+50h] [ebp-3Ch] BYREF
    rdMatrix34 out; // [esp+5Ch] [ebp-30h] BYREF

    sithThing* focusThing = cam->primaryFocus;
    float v77 = sithCamera_povShakeF2 * sithTime_deltaSeconds;
    float v78 = sithCamera_povShakeF1 * sithTime_deltaSeconds;
    switch ( cam->cameraPerspective )
    {
        case 1:
            rdMatrix_Copy34(&cam->viewMat, &focusThing->lookOrientation);
            if ( focusThing->move_type == MOVETYPE_PATH && focusThing->rdthing.hierarchyNodeMatrices)
            {
                rdMatrix_Copy34(&cam->viewMat, focusThing->rdthing.hierarchyNodeMatrices);
            }
            else
            {
                if ( focusThing->thingType == THINGTYPE_ACTOR || focusThing->thingType == THINGTYPE_PLAYER )
                {
                    rdVector_Copy3(&v76, &focusThing->actorParams.eyePYR);
                }
                else
                {
                    rdVector_Zero3(&v76);
                }

                if ( focusThing->move_type == MOVETYPE_PHYSICS )
                {
                    v76.z = rdMath_clampf(5.0 * rdVector_Dot3(&focusThing->lookOrientation.rvec, &focusThing->physicsParams.vel), -8.0, 8.0); 
                }
                if ( focusThing == g_localPlayerThing )
                {
                    rdVector_Add3Acc(&v76, &sithCamera_povShakeVector2);
                }
                rdMatrix_PreRotate34(&cam->viewMat, &v76);
                rdMatrix_PostTranslate34(&cam->viewMat, &focusThing->position);
                if ( focusThing->thingType == THINGTYPE_ACTOR || focusThing->thingType == THINGTYPE_PLAYER )
                {
                    rdMatrix_PreTranslate34(&cam->viewMat, &focusThing->actorParams.eyeOffset);
                    if ( focusThing == g_localPlayerThing )
                        rdMatrix_PreTranslate34(&cam->viewMat, &sithCamera_povShakeVector1);
                }
                rdMatrix_Normalize34(&cam->viewMat);
            }
            cam->sector = sithUnk3_GetSectorLookAt(focusThing->sector, &focusThing->position, &cam->viewMat.scale, 0.02);
            break;
        case 4:
            if ( focusThing->thingType == THINGTYPE_ACTOR || focusThing->thingType == THINGTYPE_PLAYER )
            {
                rdVector_Copy3(&v76, &focusThing->actorParams.eyePYR);
            }
            else
            {
                rdVector_Zero3(&v76);
            }
            rdMatrix_Copy34(&out, &focusThing->lookOrientation);
            rdMatrix_PreRotate34(&out, &v76);
            rdMatrix_PostTranslate34(&out, &focusThing->position);
            if ( focusThing->thingType == THINGTYPE_ACTOR || focusThing->thingType == THINGTYPE_PLAYER )
                rdMatrix_PostTranslate34(&out, &focusThing->actorParams.eyeOffset);
            cam->sector = sithCamera_create_unk_struct(0, focusThing->sector, &focusThing->position, &out.scale, 0.02, 8704);
            rdVector_Copy3(&v84, &out.scale);
            rdMatrix_Copy34(&cam->viewMat, &out);
            rdMatrix_PreTranslate34(&out, &sithCamera_trans);
            rdMatrix_PreTranslate34(&cam->viewMat, &cam->vec3_3);
            rdMatrix_LookAt(&cam->viewMat, &cam->viewMat.scale, &out.scale, 0.0);
            cam->sector = sithCamera_create_unk_struct(0, cam->sector, &v84, &cam->viewMat.scale, 0.02, 8704);
            break;
        case 32:
            rdMatrix_TransformVector34(&a1, &sithCamera_trans2, &sithCamera_focusMat);
            v2 = (rdVector3){0.0, 0.0, 0.050000001};
            rdVector_Sub3(&v2, &focusThing->position, &v2);
            rdVector_Add3Acc(&a1, &v2);
            rdMatrix_LookAt(&cam->viewMat, &a1, &v2, 0.0);
            cam->sector = sithCamera_create_unk_struct(0, focusThing->sector, &focusThing->position, &cam->viewMat.scale, 0.02, 8704);
            rot.x = 0.0;
            rot.y = sithTime_deltaSeconds * 8.0;
            rot.z = 0.0;
            rdMatrix_PostRotate34(&sithCamera_focusMat, &rot);
            rdMatrix_Normalize34(&sithCamera_focusMat);
            break;
        case 64:
            rdVector_Normalize3Acc(&sithCamera_trans3);
            rdVector_Neg3(&mode64Tmp, &sithCamera_trans3);
            
            cam->viewMat.lvec.x = mode64Tmp.x;
            cam->viewMat.lvec.y = mode64Tmp.y;
            cam->viewMat.lvec.z = mode64Tmp.z;
            cam->viewMat.rvec.x = (1.0 * mode64Tmp.y) - (0.0 * mode64Tmp.z);
            cam->viewMat.rvec.y = (0.0 * mode64Tmp.z) - (1.0 * mode64Tmp.x);
            cam->viewMat.rvec.z = (0.0 * mode64Tmp.x) - (0.0 * mode64Tmp.y);
            cam->viewMat.uvec.x = (((0.0 * mode64Tmp.z) - (1.0 * mode64Tmp.x)) * mode64Tmp.z) - (((0.0 * mode64Tmp.x) - (0.0 * mode64Tmp.y)) * mode64Tmp.y);
            cam->viewMat.uvec.y = (((0.0 * mode64Tmp.x) - (0.0 * mode64Tmp.y)) * mode64Tmp.x) - ((1.0 * mode64Tmp.y) - (0.0 * mode64Tmp.z)) * mode64Tmp.z;
            cam->viewMat.uvec.z = (((1.0 * mode64Tmp.y) - (0.0 * mode64Tmp.z)) * mode64Tmp.y) - (((0.0 * mode64Tmp.z) - (1.0 * mode64Tmp.x)) * mode64Tmp.x);

            rdMatrix_Normalize34(&cam->viewMat);
            rdVector_Scale3(&cam->viewMat.scale, &sithCamera_trans3, 0.2);

            rdMatrix_PostTranslate34(&cam->viewMat, &focusThing->position);
            cam->sector = sithCamera_create_unk_struct(0, focusThing->sector, &focusThing->position, &cam->viewMat.scale, 0.02, 8704);
            break;
        case 128:
            rdMatrix_Copy34(&cam->viewMat, &sithCamera_viewMat);
            rdMatrix_PostTranslate34(&cam->viewMat, &focusThing->position);
            cam->sector = sithUnk3_GetSectorLookAt(focusThing->sector, &focusThing->position, &cam->viewMat.scale, 0.02);
            break;
        default:
            break;
    }
    cam->vec3_1 = cam->viewMat.scale;
    rdMatrix_ExtractAngles34(&cam->viewMat, &cam->vec3_2);

    // TODO what inlined func is this
    if ( sithCamera_povShakeVector1.x < 0.0 )
    {
        if ( sithCamera_povShakeVector1.x >= 0.0 )
            goto LABEL_42;
        float v42 = v78 + sithCamera_povShakeVector1.x;
        if ( v42 < 0.0 )
        {
            sithCamera_povShakeVector1.x = v42;
            goto LABEL_42;
        }
    }
    else
    {
        float v41 = sithCamera_povShakeVector1.x - v78;
        if ( v41 > 0.0 )
        {
            sithCamera_povShakeVector1.x = v41;
            goto LABEL_42;
        }
    }
    sithCamera_povShakeVector1.x = 0.0;
LABEL_42:

    if ( sithCamera_povShakeVector1.y < 0.0 )
    {
        if ( sithCamera_povShakeVector1.y >= 0.0 )
            goto LABEL_49;
        float v48 = v78 + sithCamera_povShakeVector1.y;
        if ( v48 < 0.0 )
        {
            sithCamera_povShakeVector1.y = v48;
            goto LABEL_49;
        }
    }
    else
    {
        float v47 = sithCamera_povShakeVector1.y - v78;
        if ( v47 > 0.0 )
        {
            sithCamera_povShakeVector1.y = v47;
            goto LABEL_49;
        }
    }
    sithCamera_povShakeVector1.y = 0.0;
LABEL_49:
    if ( sithCamera_povShakeVector1.z < 0.0 )
    {
        if ( sithCamera_povShakeVector1.z >= 0.0 )
            goto LABEL_56;
        float v54 = v78 + sithCamera_povShakeVector1.z;
        if ( v54 < 0.0 )
        {
            sithCamera_povShakeVector1.z = v54;
            goto LABEL_56;
        }
    }
    else
    {
        float v53 = sithCamera_povShakeVector1.z - v78;
        if ( v53 > 0.0 )
        {
            sithCamera_povShakeVector1.z = v53;
            goto LABEL_56;
        }
    }
    sithCamera_povShakeVector1.z = 0.0;
LABEL_56:
    if ( sithCamera_povShakeVector2.x < 0.0 )
    {
        if ( sithCamera_povShakeVector2.x >= 0.0 )
            goto LABEL_63;
        float v60 = v77 + sithCamera_povShakeVector2.x;
        if ( v60 < 0.0 )
        {
            sithCamera_povShakeVector2.x = v60;
            goto LABEL_63;
        }
    }
    else
    {
        float v59 = sithCamera_povShakeVector2.x - v77;
        if ( v59 > 0.0 )
        {
            sithCamera_povShakeVector2.x = v59;
            goto LABEL_63;
        }
    }
    sithCamera_povShakeVector2.x = 0.0;
LABEL_63:
    if ( sithCamera_povShakeVector2.y < 0.0 )
    {
        if ( sithCamera_povShakeVector2.y >= 0.0 )
            goto LABEL_70;
        float v66 = v77 + sithCamera_povShakeVector2.y;
        if ( v66 < 0.0 )
        {
            sithCamera_povShakeVector2.y = v66;
            goto LABEL_70;
        }
    }
    else
    {
        float v65 = sithCamera_povShakeVector2.y - v77;
        if ( v65 > 0.0 )
        {
            sithCamera_povShakeVector2.y = v65;
            goto LABEL_70;
        }
    }
    sithCamera_povShakeVector2.y = 0.0;
LABEL_70:
    if ( sithCamera_povShakeVector2.z < 0.0 )
    {
        if ( sithCamera_povShakeVector2.z >= 0.0 )
            return;
        float v72 = v77 + sithCamera_povShakeVector2.z;
        if ( v72 < 0.0 )
        {
            sithCamera_povShakeVector2.z = v72;
            return;
        }
    }
    else
    {
        float v71 = sithCamera_povShakeVector2.z - v77;
        if ( v71 > 0.0 )
        {
            sithCamera_povShakeVector2.z = v71;
            return;
        }
    }
    sithCamera_povShakeVector2.z = 0.0;
}