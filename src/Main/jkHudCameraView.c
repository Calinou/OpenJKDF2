#include "jkHudCameraView.h"

#include "Win95/Windows.h"
#include "Win95/stdDisplay.h"
#include "General/stdBitmap.h"
#include "General/stdFont.h"
#include "General/stdString.h"
#include "stdPlatform.h"
#include "Cog/sithCog.h"
#include "Main/Main.h"
#include "jk.h"

static int jkHudCameraView_bInitted = 0;
static int jkHudCameraView_bOpened = 0;
static int jkHudCameraView_w_005b08e0 = 0;
static int jkHudCameraView_h_005b08e4 = 0;

static stdBitmap* jkHudCameraView_pBitmap1 = NULL;
static stdBitmap* jkHudCameraView_pBitmap2 = NULL;
static stdBitmap* jkHudCameraView_pBitmap3 = NULL;
static stdBitmap* jkHudCameraView_pBitmap4 = NULL;
static stdFont* jkHudCameraView_pFont1 = NULL;

static jkHudMotsBitmap jkHudCameraView_aBitmaps[4] = {
    {&jkHudCameraView_pBitmap1, "HudA8.bm", "HudA16.bm", 0x43, 50, 50, 0x0, 0x0},
    {&jkHudCameraView_pBitmap2, "HudB8.bm", "HudB16.bm", 0x43, 590, 430, 0x0, 0x0},
    {&jkHudCameraView_pBitmap3, "HudC8.bm", "HudC16.bm", 0x43, 50, 430, 0x0, 0x0},
    {&jkHudCameraView_pBitmap4, "HudD8.bm", "HudD16.bm", 0x43, 590, 50, 0x0, 0x0},
};

static jkHudMotsFont jkHudCameraView_aFonts[1] = {
    {&jkHudCameraView_pFont1, "AmoNums.sft", "AmoNums.sft"},
};

void jkHudCameraView_Startup(void)
{
    jkHudCameraView_bInitted = 1;
}

void jkHudCameraView_Shutdown(void)
{
    jkHudCameraView_bInitted = 0;
}


int jkHudCameraView_Open(void)
{
    int iVar1;
    int iVar2;
    stdBitmap *psVar3;
    int lVar7;
    int lVar8;
    char *pcVar9;
    char *pcVar10;
    char local_80 [128];
    
    iVar1 = Video_modeStruct.aViewSizes[Video_modeStruct.viewSizeIdx].xMin;
    iVar2 = Video_modeStruct.aViewSizes[Video_modeStruct.viewSizeIdx].yMin;
    lVar7 = (int)(Video_modeStruct.aViewSizes[Video_modeStruct.viewSizeIdx].xMax - (float)(iVar1 / 2));
    lVar8 = (int)(Video_modeStruct.aViewSizes[Video_modeStruct.viewSizeIdx].yMax - (float)(iVar2 / 2));
    jkHudCameraView_w_005b08e0 = (iVar1 * 1000 + 320) / 640;
    jkHudCameraView_h_005b08e4 = (iVar2 * 1000 + 240) / 480;

    if (jkHudCameraView_bOpened != 0) {
        return 0;
    }

    jkHudMotsBitmap* pBmIter = jkHudCameraView_aBitmaps;

    do 
    {
        if (Video_format.format.bpp == 8) {
            pcVar10 = pBmIter->path8bpp;
            pcVar9 = "ui\\bm\\%s";
        }
        else {
            pcVar10 = pBmIter->path16bpp;
            pcVar9 = "ui\\bm\\%s";
        }
        _sprintf(local_80,pcVar9,pcVar10);
        psVar3 = stdBitmap_Load2(local_80,0,0);
        *pBmIter->pBitmap = psVar3;
        pBmIter->unk4 = (int)(pBmIter->unk2 * iVar1) / 640 + lVar7;
        pBmIter->unk5 = (int)(pBmIter->unk3 * iVar2) / 480 + lVar8;
        switch(pBmIter->unk1) 
        {
            case 0x42:
                pBmIter->unk4 -= ((uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).width >> 1);
                pBmIter->unk5 -= ((*(*pBmIter->pBitmap)->mipSurfaces)->format).height;
                break;
            case 0x43:
                pBmIter->unk4 -= ((uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).width >> 1);
                pBmIter->unk5 -= ((uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).height >> 1);
                break;
            case 0x4c:
                pBmIter->unk5 -= ((uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).height >> 1);
                break;
            case 0x52:
                pBmIter->unk4 -= ((*(*pBmIter->pBitmap)->mipSurfaces)->format).width;
                pBmIter->unk5 -= (uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).height >> 1;
                break;
            case 0x54:
                pBmIter->unk4 = pBmIter->unk4 - ((uint32_t)((*(*pBmIter->pBitmap)->mipSurfaces)->format).width >> 1);
                break;
            default:
                break;
        }
        if (*pBmIter->pBitmap == (stdBitmap *)0x0) {
            Windows_GameErrorMsgbox("ERR_CANNOT_LOAD_FILE %s",local_80);
        }
        else {
            stdBitmap_ConvertColorFormat(&Video_format.format,*pBmIter->pBitmap);
        }

        pBmIter++;
        
        if (pBmIter >= &jkHudCameraView_aBitmaps[4]) {
            break;
        }
    } while( 1 );

    jkHudCameraView_bOpened = 1;
    return 1;
}


void jkHudCameraView_Close(void)
{
    jkHudMotsBitmap *pjVar1;
    jkHudMotsFont *pjVar2;
    
    pjVar1 = jkHudCameraView_aBitmaps;
    do {
        if (*pjVar1->pBitmap) {
            stdBitmap_Free(*pjVar1->pBitmap);
            *pjVar1->pBitmap = NULL;
        }
        pjVar1 = pjVar1 + 1;
    } while (pjVar1 < &jkHudCameraView_aBitmaps[4]);

    pjVar2 = jkHudCameraView_aFonts;
    do {
        if (*pjVar2->pFont) {
            stdFont_Free(*pjVar2->pFont);
            *pjVar2->pFont = NULL;
        }
        pjVar2 = pjVar2 + 1;
    } while (pjVar2 < &jkHudCameraView_aFonts[1]);

    if (jkHudCameraView_bOpened != 0) {
        jkHudCameraView_bOpened = 0;
    }
}


void jkHudCameraView_Draw(void)
{
    if (!jkHudCameraView_bOpened) return;
    if (sithWorld_pCurrentWorld->playerThing->type != SITH_THING_PLAYER) return;

    jkHudMotsBitmap* pBmIter = jkHudCameraView_aBitmaps;

#ifndef SDL2_RENDER
    stdVBuffer* pOverlayBuffer = Video_pMenuBuffer;
    rdCanvas* pOverlayCanvas = Video_pCanvas;
#else
    stdVBuffer* pOverlayBuffer = Video_pCanvasOverlayMap->vbuffer;
    rdCanvas* pOverlayCanvas = Video_pCanvasOverlayMap;
    stdDisplay_VBufferLock(pOverlayBuffer);
#endif

    do 
    {
        stdDisplay_VBufferCopy(pOverlayBuffer, *(*pBmIter->pBitmap)->mipSurfaces, pBmIter->unk4, pBmIter->unk5, NULL, 1);
        pBmIter++;
    } while (pBmIter < &jkHudCameraView_aBitmaps[4]);

#ifdef SDL2_RENDER
    stdDisplay_VBufferUnlock(pOverlayBuffer);
#endif
}

