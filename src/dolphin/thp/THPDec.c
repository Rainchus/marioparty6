#include <dolphin/PPCArch.h>
#include <dolphin/thp.h>

static THPHuffmanTab *Ydchuff ATTRIBUTE_ALIGN(32);
static THPHuffmanTab *Udchuff ATTRIBUTE_ALIGN(32);
static THPHuffmanTab *Vdchuff ATTRIBUTE_ALIGN(32);
static THPHuffmanTab *Yachuff ATTRIBUTE_ALIGN(32);
static THPHuffmanTab *Uachuff ATTRIBUTE_ALIGN(32);
static THPHuffmanTab *Vachuff ATTRIBUTE_ALIGN(32);
static f32 __THPIDCTWorkspace[64] ATTRIBUTE_ALIGN(32);
static u8 *__THPHuffmanBits;
static u8 *__THPHuffmanSizeTab;
static u16 *__THPHuffmanCodeTab;
static THPSample *Gbase ATTRIBUTE_ALIGN(32);
static u32 Gwid ATTRIBUTE_ALIGN(32);
static f32 *Gq ATTRIBUTE_ALIGN(32);
static u8 *__THPLCWork512[3];
static u8 *__THPLCWork640[3];
static u32 __THPOldGQR5;
static u32 __THPOldGQR6;
static u8 *__THPWorkArea;
static THPCoeff *__THPMCUBuffer[6];
static THPFileInfo *__THPInfo;
static BOOL __THPInitFlag = FALSE;

#define THPROUNDUP(a, b) ((((s32)(a)) + ((s32)(b)-1L)) / ((s32)(b)))

/* Unresolved here; the extracted original object replaces this nonmatching translation unit. */
void __THPGQRSetup(void);
void __THPGQRRestore(void);
void __THPInverseDCTNoYPos(THPCoeff *in, u32 xPos);
void __THPInverseDCTY8(THPCoeff *in, u32 xPos);
void __THPHuffDecodeDCTCompY(THPFileInfo *info, THPCoeff *block);
void __THPHuffDecodeDCTCompU(THPFileInfo *info, THPCoeff *block);
void __THPHuffDecodeDCTCompV(THPFileInfo *info, THPCoeff *block);

s32 THPVideoDecode(void *file, void *tileY, void *tileU, void *tileV, void *work)
{
    u8 all_done, status;
    s32 errorCode;

    if (!file) {
        goto _err_no_input;
    }

    if (tileY == NULL || tileU == NULL || tileV == NULL) {
        goto _err_no_output;
    }

    if (!work) {
        goto _err_no_work;
    }

    if (!(PPCMfhid2() & 0x10000000)) {
        goto _err_lc_not_enabled;
    }

    if (__THPInitFlag == FALSE) {
        goto _err_not_initialized;
    }

    __THPWorkArea = (u8 *)work;
    __THPInfo = (THPFileInfo *)OSRoundUp32B(__THPWorkArea);
    __THPWorkArea = (u8 *)OSRoundUp32B(__THPWorkArea) + sizeof(THPFileInfo);
    DCZeroRange(__THPInfo, sizeof(THPFileInfo));
    __THPInfo->cnt = 33;
    __THPInfo->decompressedY = 0;
    __THPInfo->c = (u8 *)file;
    all_done = FALSE;

    for (;;) {
        if ((*(__THPInfo->c)++) != 255) {
            goto _err_bad_syntax;
        }

        while (*__THPInfo->c == 255) {
            ((__THPInfo->c)++);
        }

        status = (*(__THPInfo->c)++);

        if (status <= 0xD7) {
            if (status == 196) {
                status = __THPReadHuffmanTableSpecification();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 192) {
                status = __THPReadFrameHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xD8 <= status && status <= 0xDF) {
            if (status == 221) {
                __THPRestartDefinition();
            }

            else if (status == 219) {
                status = __THPReadQuantizationTable();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 218) {
                status = __THPReadScaneHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }

                all_done = TRUE;
            }
            else if (status == 216) {
                // empty but required for match
            }
            else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xE0 <= status) {
            if ((224 <= status && status <= 239) || status == 254) {
                __THPInfo->c += (__THPInfo->c)[0] << 8 | (__THPInfo->c)[1];
            }
            else {
                goto _err_unsupported_marker;
            }
        }

        if (all_done) {
            break;
        }
    }

    __THPSetupBuffers();
    __THPDecompressYUV(tileY, tileU, tileV);
    return 0;

_err_no_input:
    errorCode = 25;
    goto _err_exit;

_err_no_output:
    errorCode = 27;
    goto _err_exit;

_err_no_work:
    errorCode = 26;
    goto _err_exit;

_err_unsupported_marker:
    errorCode = 11;
    goto _err_exit;

_err_bad_resource:
    errorCode = 1;
    goto _err_exit;

_err_no_mem:
    errorCode = 6;
    goto _err_exit;

_err_bad_syntax:
    errorCode = 3;
    goto _err_exit;

_err_bad_status:
    errorCode = status;
    goto _err_exit;

_err_lc_not_enabled:
    errorCode = 28;
    goto _err_exit;

_err_not_initialized:
    errorCode = 29;
    goto _err_exit;

_err_exit:
    return errorCode;
}

static void __THPSetupBuffers(void)
{
    u8 i;
    THPCoeff *buffer;

    buffer = (THPCoeff *)OSRoundUp32B(__THPWorkArea);

    for (i = 0; i < 6; i++) {
        __THPMCUBuffer[i] = &buffer[i * 64];
    }
}

static u8 __THPReadFrameHeader(void)
{
    u8 i, utmp8;

    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);

    if (utmp8 != 8) {
        return 10;
    }

    __THPInfo->yPixelSize = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    __THPInfo->xPixelSize = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);
    if (utmp8 != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        utmp8 = (*(__THPInfo->c)++);
        utmp8 = (*(__THPInfo->c)++);
        if ((i == 0 && utmp8 != 0x22) || (i > 0 && utmp8 != 0x11)) {
            return 19;
        }

        __THPInfo->components[i].quantizationTableSelector = (*(__THPInfo->c)++);
    }

    return 0;
}

static u8 __THPReadScaneHeader(void)
{
    u8 i, utmp8;
    __THPInfo->c += 2;

    utmp8 = (*(__THPInfo->c)++);

    if (utmp8 != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        utmp8 = (*(__THPInfo->c)++);

        utmp8 = (*(__THPInfo->c)++);
        __THPInfo->components[i].DCTableSelector = (u8)(utmp8 >> 4);
        __THPInfo->components[i].ACTableSelector = (u8)(utmp8 & 15);

        if ((__THPInfo->validHuffmanTabs & (1 << ((utmp8 >> 4)))) == 0) {
            return 15;
        }

        if ((__THPInfo->validHuffmanTabs & (1 << ((utmp8 & 15) + 1))) == 0) {
            return 15;
        }
    }

    __THPInfo->c += 3;
    __THPInfo->MCUsPerRow = (u16)THPROUNDUP(__THPInfo->xPixelSize, 16);
    __THPInfo->components[0].predDC = 0;
    __THPInfo->components[1].predDC = 0;
    __THPInfo->components[2].predDC = 0;
    return 0;
}

static u8 __THPReadQuantizationTable(void)
{
    u16 length, id, i, row, col;
    f32 q_temp[64];

    length = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    length -= 2;

    for (;;) {
        id = (*(__THPInfo->c)++);

        for (i = 0; i < 64; i++) {
            q_temp[__THPJpegNaturalOrder[i]] = (f32)(*(__THPInfo->c)++);
        }

        i = 0;
        for (row = 0; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                __THPInfo->quantTabs[id][i] = (f32)((f64)q_temp[i] * __THPAANScaleFactor[row] * __THPAANScaleFactor[col]);
                i++;
            }
        }

        length -= 65;
        if (!length) {
            break;
        }
    }

    return 0;
}

static u8 __THPReadHuffmanTableSpecification(void)
{
    u8 t_class, id, i, tab_index;
    u16 length, num_Vij;

    __THPHuffmanSizeTab = __THPWorkArea;
    __THPHuffmanCodeTab = (u16 *)((u32)__THPWorkArea + 256 + 1);
    length = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    length -= 2;

    for (;;) {
        i = (*(__THPInfo->c)++);
        id = (u8)(i & 15);
        t_class = (u8)(i >> 4);
        __THPHuffmanBits = __THPInfo->c;
        tab_index = (u8)((id << 1) + t_class);
        num_Vij = 0;

        for (i = 0; i < 16; i++) {
            num_Vij += (*(__THPInfo->c)++);
        }

        __THPInfo->huffmanTabs[tab_index].Vij = __THPInfo->c;
        __THPInfo->c += num_Vij;
        __THPHuffGenerateSizeTable();
        __THPHuffGenerateCodeTable();
        __THPHuffGenerateDecoderTables(tab_index);
        __THPInfo->validHuffmanTabs |= 1 << tab_index;
        length -= 17 + num_Vij;

        if (length == 0) {
            break;
        }
    }

    return 0;
}

static void __THPHuffGenerateSizeTable(void)
{
    s32 p, l, i;
    p = 0;

    for (l = 1; l <= 16; l++) {
        i = (s32)__THPHuffmanBits[l - 1];
        while (i--) {
            __THPHuffmanSizeTab[p++] = (u8)l;
        }
    }

    __THPHuffmanSizeTab[p] = 0;
}

static void __THPHuffGenerateCodeTable(void)
{
    u8 si;
    u16 p, code;

    p = 0;
    code = 0;
    si = __THPHuffmanSizeTab[0];

    while (__THPHuffmanSizeTab[p]) {
        while (__THPHuffmanSizeTab[p] == si) {
            __THPHuffmanCodeTab[p++] = code;
            code++;
        }

        code <<= 1;
        si++;
    }
}

static void __THPHuffGenerateDecoderTables(u8 tabIndex)
{
    s32 p, l;
    THPHuffmanTab *h;

    p = 0;
    h = &__THPInfo->huffmanTabs[tabIndex];
    for (l = 1; l <= 16; l++) {
        if (__THPHuffmanBits[l - 1]) {
            h->valPtr[l] = p - __THPHuffmanCodeTab[p];
            p += __THPHuffmanBits[l - 1];
            h->maxCode[l] = __THPHuffmanCodeTab[p - 1];
        }
        else {
            h->maxCode[l] = -1;
            h->valPtr[l] = -1;
        }
    }

    h->maxCode[17] = 0xfffffL;
}

static void __THPRestartDefinition(void)
{
    __THPInfo->RST = TRUE;
    __THPInfo->c += 2;
    __THPInfo->nMCU = (u16)((__THPInfo->c)[0] << 8 | (__THPInfo->c)[1]);
    __THPInfo->c += 2;
    __THPInfo->currMCU = __THPInfo->nMCU;
}

void __THPPrepBitStream(void)
{
    u32 *ptr;
    u32 offset, i, j, k;

    ptr = (u32 *)((u32)__THPInfo->c & 0xFFFFFFFC);
    offset = (u32)__THPInfo->c & 3;

    if (__THPInfo->cnt != 33) {
        __THPInfo->cnt -= (3 - offset) * 8;
    }
    else {
        __THPInfo->cnt = (offset * 8) + 1;
    }

    __THPInfo->c = (u8 *)ptr;
    __THPInfo->currByte = *ptr;

    for (i = 0; i < 4; i++) {
        if (__THPInfo->validHuffmanTabs & (1 << i)) {
            for (j = 0; j < 32; j++) {
                __THPInfo->huffmanTabs[i].quick[j] = 0xFF;

                for (k = 0; k < 5; k++) {
                    s32 code = (s32)(j >> (5 - k - 1));

                    if (code <= __THPInfo->huffmanTabs[i].maxCode[k + 1]) {
                        __THPInfo->huffmanTabs[i].quick[j] = __THPInfo->huffmanTabs[i].Vij[(s32)(code + __THPInfo->huffmanTabs[i].valPtr[k + 1])];
                        __THPInfo->huffmanTabs[i].increment[j] = (u8)(k + 1);
                        k = 99;
                    }
                    else {
                    }
                }
            }
        }
    }

    {
        s32 YdcTab, UdcTab, VdcTab, YacTab, UacTab, VacTab;

        YdcTab = (__THPInfo->components[0].DCTableSelector << 1);
        UdcTab = (__THPInfo->components[1].DCTableSelector << 1);
        VdcTab = (__THPInfo->components[2].DCTableSelector << 1);

        YacTab = (__THPInfo->components[0].ACTableSelector << 1) + 1;
        UacTab = (__THPInfo->components[1].ACTableSelector << 1) + 1;
        VacTab = (__THPInfo->components[2].ACTableSelector << 1) + 1;

        Ydchuff = &__THPInfo->huffmanTabs[YdcTab];
        Udchuff = &__THPInfo->huffmanTabs[UdcTab];
        Vdchuff = &__THPInfo->huffmanTabs[VdcTab];

        Yachuff = &__THPInfo->huffmanTabs[YacTab];
        Uachuff = &__THPInfo->huffmanTabs[UacTab];
        Vachuff = &__THPInfo->huffmanTabs[VacTab];
    }
}

static void __THPDecompressYUV(void *tileY, void *tileU, void *tileV)
{
    u16 currentY, targetY;
    __THPInfo->dLC[0] = tileY;
    __THPInfo->dLC[1] = tileU;
    __THPInfo->dLC[2] = tileV;

    currentY = __THPInfo->decompressedY;
    targetY = __THPInfo->yPixelSize;

    __THPGQRSetup();
    __THPPrepBitStream();

    if (__THPInfo->xPixelSize == 512 && targetY == 448) {
        while (currentY < targetY) {
            __THPDecompressiMCURow512x448();
            currentY += 16;
        }
    }
    else if (__THPInfo->xPixelSize == 640 && targetY == 480) {
        while (currentY < targetY) {
            __THPDecompressiMCURow640x480();
            currentY += 16;
        }
    }
    else {
        while (currentY < targetY) {
            __THPDecompressiMCURowNxN();
            currentY += 16;
        }
    }

    __THPGQRRestore();
}

static void __THPDecompressiMCURow512x448(void)
{
    u8 cl_num;
    u32 x_pos;
    THPComponent *comp;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork512[0];
        Gwid = 512;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
        __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
        __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
        __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork512[1];
        Gwid = 256;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);
        comp = &__THPInfo->components[2];
        Gbase = __THPLCWork512[2];
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            if ((--__THPInfo->currMCU) == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 33) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork512[0], 0x2000);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork512[1], 0x800);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork512[2], 0x800);

    __THPInfo->dLC[0] += 0x2000;
    __THPInfo->dLC[1] += 0x800;
    __THPInfo->dLC[2] += 0x800;
}

static void __THPDecompressiMCURow640x480(void)
{
    u8 cl_num;
    u32 x_pos;
    THPComponent *comp;

    LCQueueWait(3);

    {
        for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
            THPFileInfo *um = __THPInfo;
            __THPHuffDecodeDCTCompY(um, __THPMCUBuffer[0]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
            __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
            __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

            comp = &__THPInfo->components[0];
            Gbase = __THPLCWork640[0];
            Gwid = 640;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos = (u32)(cl_num * 16);
            __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
            __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
            __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
            __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

            comp = &__THPInfo->components[1];
            Gbase = __THPLCWork640[1];
            Gwid = 320;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos /= 2;
            __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);

            comp = &__THPInfo->components[2];
            Gbase = __THPLCWork640[2];
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

            if (__THPInfo->RST != 0) {
                __THPInfo->currMCU--;
                if (__THPInfo->currMCU == 0) {
                    __THPInfo->currMCU = __THPInfo->nMCU;

                    __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                    if (__THPInfo->cnt > 32) {
                        __THPInfo->cnt = 33;
                    }

                    __THPInfo->components[0].predDC = 0;
                    __THPInfo->components[1].predDC = 0;
                    __THPInfo->components[2].predDC = 0;
                }
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0], 0x2800);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1], 0xA00);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2], 0xA00);

    __THPInfo->dLC[0] += 0x2800;
    __THPInfo->dLC[1] += 0xA00;
    __THPInfo->dLC[2] += 0xA00;
}

static void __THPDecompressiMCURowNxN(void)
{
    u8 cl_num;
    u32 x_pos, x;
    THPComponent *comp;

    x = __THPInfo->xPixelSize;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork640[0];
        Gwid = x;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
        __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
        __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
        __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork640[1];
        Gwid = x / 2;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);

        comp = &__THPInfo->components[2];
        Gbase = __THPLCWork640[2];
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            __THPInfo->currMCU--;
            if (__THPInfo->currMCU == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 32) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0], ((4 * sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1], ((sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2], ((sizeof(u8) * 64) * (x / 16)));
    __THPInfo->dLC[0] += ((4 * sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[1] += ((sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[2] += ((sizeof(u8) * 64) * (x / 16));
}
