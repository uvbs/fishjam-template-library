/*****************************************************************************

 quantize.c - quantize a high resolution image into lower one

 Based on: "Color Image Quantization for frame buffer Display", by
 Paul Heckbert SIGGRAPH 1982 page 297-307.

 This doesn't really belong in the core library, was undocumented,
 and was removed in 4.2.  Then it turned out some client apps were
 actually using it, so it was restored in 5.0.

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "gif_lib.h"
#include "gif_lib_private.h"

#define ABS(x)    ((x) > 0 ? (x) : (-(x)))

#define COLOR_ARRAY_SIZE 32768
#define BITS_PER_PRIM_COLOR 5
#define MAX_PRIM_COLOR      0x1f

static int SortRGBAxis;

typedef struct QuantizedColorType {
    GifByteType RGB[3];
    GifByteType NewColorIndex;
    long Count;
    struct QuantizedColorType *Pnext;
} QuantizedColorType;

typedef struct NewColorMapType {
    GifByteType RGBMin[3], RGBWidth[3];
    unsigned int NumEntries; /* # of QuantizedColorType in linked list below */
    unsigned long Count; /* Total number of pixels in all the entries */
    QuantizedColorType *QuantizedColors;
} NewColorMapType;

static int SubdivColorMap(NewColorMapType * NewColorSubdiv,
                          unsigned int ColorMapSize,
                          unsigned int *NewColorMapSize);
static int SortCmpRtn(const void *Entry1, const void *Entry2);


int GifQuantizeRGBBuffer(unsigned int Width, unsigned int Height, 
                         int ColorRes, unsigned char *RGBBuffer, 
                         GifColorType * ColorMap, GifByteType * GIFBuffer)
{
    unsigned int i = 0, j = 0;
    GifByteType *RedInput = NULL, *GreenInput = NULL, *BlueInput = NULL;
    unsigned int nIndex = 0;
    unsigned int nWidthBytes  = 0;

    /* Convert the RGB Buffer into 3 separated buffers: */
    unsigned int nLength = Width * Height;
    RedInput = (GifByteType *) malloc(sizeof(GifByteType) * nLength);
    GreenInput = (GifByteType *) malloc(sizeof(GifByteType) * nLength);
    BlueInput = (GifByteType *) malloc(sizeof(GifByteType) * nLength);

    memset(RedInput, 0, nLength);
    memset(GreenInput, 0, nLength);
    memset(BlueInput, 0, nLength);

    switch (ColorRes)
    {
    case 24://RGB
        {
            //4字节对齐来计算
            nWidthBytes = (Width * 24 + 31) / 32 * 4;
            nIndex = 0;
            for (j = 0; j < Height; j++)
            {
                for (i = 0; i < Width; i++)
                {
                    RedInput[nIndex] = RGBBuffer[ j * nWidthBytes + i * 3 + 2];         // & 0xff;
                    GreenInput[nIndex] = (RGBBuffer[j * nWidthBytes + i * 3 + 1]); // >> 8) & 0xff;
                    BlueInput[nIndex] = (RGBBuffer[j * nWidthBytes + i * 3]);   // >> 16) & 0xff;
                    nIndex++;
                }
            }
            break;
        }
    case 32: //RGBA
        {
            for (i = 0; i < nLength; i++) {
                RedInput[i] = RGBBuffer[i * 4 + 2];
                GreenInput[i] = (RGBBuffer[i * 4 + 1]);  // >> 8) & 0xff;
                BlueInput[i] = (RGBBuffer[i * 4]);   // >> 16) & 0xff;
            }
            break;
        }
    default:
        //assert(false && TEXT("TODO"));
        break;
    }
    
    for (i = 0; i < 256; i++){
        ColorMap[i].Red = ColorMap[i].Green = ColorMap[i].Blue = 0;
    }
    i = 256;
    if(GifQuantizeBuffer(Width, Height, &i, RedInput, GreenInput, BlueInput, GIFBuffer, ColorMap) == GIF_ERROR)
    {
        i = -1;
    }
    free(RedInput);
    free(GreenInput);
    free(BlueInput);

    return i;    /* Real number of colors in color table. */
}

/******************************************************************************
 Quantize high resolution image into lower one. Input image consists of a
 2D array for each of the RGB colors with size Width by Height. There is no
 Color map for the input. Output is a quantized image with 2D array of
 indexes into the output color map.
   Note input image can be 24 bits at the most (8 for red/green/blue) and
 the output has 256 colors at the most (256 entries in the color map.).
 ColorMapSize specifies size of color map up to 256 and will be updated to
 real size before returning.
   Also non of the parameter are allocated by this routine.
   This function returns GIF_OK if successful, GIF_ERROR otherwise.
******************************************************************************/
int
GifQuantizeBuffer(unsigned int Width,
               unsigned int Height,
               unsigned int *ColorMapSize,
               GifByteType * RedInput,
               GifByteType * GreenInput,
               GifByteType * BlueInput,
               GifByteType * OutputBuffer,
               GifColorType * OutputColorMap) {

    unsigned int Index = 0, NumOfEntries = 0;
    unsigned int i = 0, j = 0;
    int MaxRGBError[3] = {0};
    unsigned int NewColorMapSize = 0;
    long Red = 0, Green = 0, Blue = 0;
    
    //定义结果颜色表的空间
    NewColorMapType NewColorSubdiv[256] = {0};
    QuantizedColorType *ColorArrayEntries = NULL, *QuantizedColor = NULL;

    //进行八叉树量化计算时的容器，此处 BITS_PER_PRIM_COLOR(=5) 表示去掉了最后的3层，因此支持的颜色类型为 2^(5*3) = 2^15 = 32768(COLOR_ARRAY_SIZE)
    ColorArrayEntries = (QuantizedColorType *)malloc( sizeof(QuantizedColorType) * COLOR_ARRAY_SIZE);
    if (ColorArrayEntries == NULL) {
        return GIF_ERROR;
    }
    memset(ColorArrayEntries, 0, sizeof(QuantizedColorType) * COLOR_ARRAY_SIZE);

    //TODO: 这里设置的RGB值是干什么?
    for (i = 0; i < COLOR_ARRAY_SIZE; i++) {
        ColorArrayEntries[i].RGB[0] = i >> (2 * BITS_PER_PRIM_COLOR);
        ColorArrayEntries[i].RGB[1] = (i >> BITS_PER_PRIM_COLOR) & MAX_PRIM_COLOR;
        ColorArrayEntries[i].RGB[2] = i & MAX_PRIM_COLOR;
        ColorArrayEntries[i].Count = 0;
    }

    /* Sample the colors and their distribution: */
    for (i = 0; i < (int)(Width * Height); i++) {
        Index = (  (RedInput[i] >> (8 - BITS_PER_PRIM_COLOR)) << (2 * BITS_PER_PRIM_COLOR))
                 + ((GreenInput[i] >> (8 - BITS_PER_PRIM_COLOR)) << BITS_PER_PRIM_COLOR)
                 + (BlueInput[i] >> (8 - BITS_PER_PRIM_COLOR));
        ColorArrayEntries[Index].Count++;
    }

    /* Put all the colors in the first entry of the color map, and call the
     * recursive subdivision process.  */
    for (i = 0; i < 256; i++) {
        NewColorSubdiv[i].QuantizedColors = NULL;
        NewColorSubdiv[i].Count = NewColorSubdiv[i].NumEntries = 0;
        for (j = 0; j < 3; j++) {
            NewColorSubdiv[i].RGBMin[j] = 0;
            NewColorSubdiv[i].RGBWidth[j] = 255;
        }
    }

    //将不空的量化结果形成链表结构, 链表头为第一个不NULL的
    /* Find the non empty entries in the color table and chain them: */
    for (i = 0; i < COLOR_ARRAY_SIZE; i++){
        if (ColorArrayEntries[i].Count > 0){
            break;
        }
    }

    QuantizedColor = NewColorSubdiv[0].QuantizedColors = &ColorArrayEntries[i];
    NumOfEntries = 1;
    while (++i < COLOR_ARRAY_SIZE){
        if (ColorArrayEntries[i].Count > 0) {
            QuantizedColor->Pnext = &ColorArrayEntries[i];
            QuantizedColor = &ColorArrayEntries[i];
            NumOfEntries++;
        }
    }
    QuantizedColor->Pnext = NULL;

    NewColorSubdiv[0].NumEntries = NumOfEntries; /* Different sampled colors */
    NewColorSubdiv[0].Count = ((long)Width) * Height; /* Pixels */

    //合并多的量化结果
    NewColorMapSize = 1;
    if (SubdivColorMap(NewColorSubdiv, *ColorMapSize, &NewColorMapSize) != GIF_OK) {
        free((char *)ColorArrayEntries);
        return GIF_ERROR;
    }
    if (NewColorMapSize < *ColorMapSize) {
        /* And clear rest of color map: */
        for (i = NewColorMapSize; i < *ColorMapSize; i++){
            OutputColorMap[i].Red = OutputColorMap[i].Green = OutputColorMap[i].Blue = 0;
        }
    }

    //计算使用同一个颜色表索引的多个颜色的平均值
    /* Average the colors in each entry to be the color to be used in the
     * output color map, and plug it into the output color map itself. */
    for (i = 0; i < NewColorMapSize; i++) {
        if ((j = NewColorSubdiv[i].NumEntries) > 0) {
            QuantizedColor = NewColorSubdiv[i].QuantizedColors;
            Red = Green = Blue = 0;
            while (QuantizedColor) {
                QuantizedColor->NewColorIndex = i;
                Red += QuantizedColor->RGB[0];
                Green += QuantizedColor->RGB[1];
                Blue += QuantizedColor->RGB[2];
                QuantizedColor = QuantizedColor->Pnext;
            }
            OutputColorMap[i].Red = (GifByteType)((Red << (8 - BITS_PER_PRIM_COLOR)) / j);
            OutputColorMap[i].Green = (GifByteType)((Green << (8 - BITS_PER_PRIM_COLOR)) / j);
            OutputColorMap[i].Blue = (GifByteType)((Blue << (8 - BITS_PER_PRIM_COLOR)) / j);
        }
    }

    /* Finally scan the input buffer again and put the mapped index in the
     * output buffer.  */
    MaxRGBError[0] = MaxRGBError[1] = MaxRGBError[2] = 0;
    for (i = 0; i < (int)(Width * Height); i++) {
        Index = ((RedInput[i] >> (8 - BITS_PER_PRIM_COLOR)) << (2 * BITS_PER_PRIM_COLOR))
              + ((GreenInput[i] >> (8 - BITS_PER_PRIM_COLOR)) << BITS_PER_PRIM_COLOR) 
              + (BlueInput[i] >> (8 - BITS_PER_PRIM_COLOR));
        Index = ColorArrayEntries[Index].NewColorIndex;
        OutputBuffer[i] = Index;
        if (MaxRGBError[0] < ABS(OutputColorMap[Index].Red - RedInput[i])){
            MaxRGBError[0] = ABS(OutputColorMap[Index].Red - RedInput[i]);
        }
        if (MaxRGBError[1] < ABS(OutputColorMap[Index].Green - GreenInput[i])){
            MaxRGBError[1] = ABS(OutputColorMap[Index].Green - GreenInput[i]);
        }
        if (MaxRGBError[2] < ABS(OutputColorMap[Index].Blue - BlueInput[i])){
            MaxRGBError[2] = ABS(OutputColorMap[Index].Blue - BlueInput[i]);
        }
    }

#ifdef DEBUG
    fprintf(stderr,
            "Quantization L(0) errors: Red = %d, Green = %d, Blue = %d.\n",
            MaxRGBError[0], MaxRGBError[1], MaxRGBError[2]);
#endif /* DEBUG */

    free((char *)ColorArrayEntries);

    *ColorMapSize = NewColorMapSize;

    return GIF_OK;
}

/******************************************************************************
 Routine to subdivide the RGB space recursively using median cut in each
 axes alternatingly until ColorMapSize different cubes exists.
 The biggest cube in one dimension is subdivide unless it has only one entry.
 Returns GIF_ERROR if failed, otherwise GIF_OK.
*******************************************************************************/
static int
SubdivColorMap(NewColorMapType * NewColorSubdiv,
               unsigned int ColorMapSize,
               unsigned int *NewColorMapSize) {

    int MaxSize = 0;
    unsigned int i = 0, j = 0, Index = 0, NumEntries = 0, MinColor = 0, MaxColor = 0;
    long Sum = 0, Count = 0;
    QuantizedColorType *QuantizedColor = NULL, **SortArray = NULL;

    while (ColorMapSize > *NewColorMapSize) {
        /* Find candidate for subdivision: */
        MaxSize = -1;
        for (i = 0; i < *NewColorMapSize; i++) {
            for (j = 0; j < 3; j++) {
                if ((((int)NewColorSubdiv[i].RGBWidth[j]) > MaxSize) && (NewColorSubdiv[i].NumEntries > 1)) {
                    MaxSize = NewColorSubdiv[i].RGBWidth[j];
                    Index = i;
                    SortRGBAxis = j;
                }
            }
        }

        if (MaxSize == -1){
            return GIF_OK;
        }

        /* Split the entry Index into two along the axis SortRGBAxis: */

        /* Sort all elements in that entry along the given axis and split at
         * the median.  */
        SortArray = (QuantizedColorType **)malloc(sizeof(QuantizedColorType *) * NewColorSubdiv[Index].NumEntries);
        if (SortArray == NULL){
            return GIF_ERROR;
        }
        for (j = 0, QuantizedColor = NewColorSubdiv[Index].QuantizedColors;
             j < NewColorSubdiv[Index].NumEntries && QuantizedColor != NULL;
             j++, QuantizedColor = QuantizedColor->Pnext)
        {
            SortArray[j] = QuantizedColor;
        }

    /*
     * Because qsort isn't stable, this can produce differing 
     * results for the order of tuples depending on platform
     * details of how qsort() is implemented.
     *
     * We mitigate this problem by sorting on all three axes rather
     * than only the one specied by SortRGBAxis; that way the instability
     * can only become an issue if there are multiple color indices
     * referring to identical RGB tuples.  Older versions of this 
     * sorted on only the one axis.
     */
        qsort(SortArray, NewColorSubdiv[Index].NumEntries, sizeof(QuantizedColorType *), SortCmpRtn);

        /* Relink the sorted list into one: */
        for (j = 0; j < NewColorSubdiv[Index].NumEntries - 1; j++){
            SortArray[j]->Pnext = SortArray[j + 1];
        }

        SortArray[NewColorSubdiv[Index].NumEntries - 1]->Pnext = NULL;
        NewColorSubdiv[Index].QuantizedColors = QuantizedColor = SortArray[0];
        free((char *)SortArray);

        /* Now simply add the Counts until we have half of the Count: */
        Sum = NewColorSubdiv[Index].Count / 2 - QuantizedColor->Count;
        NumEntries = 1;
        Count = QuantizedColor->Count;
        while (QuantizedColor->Pnext != NULL 
            && (Sum -= QuantizedColor->Pnext->Count) >= 0 
            && (QuantizedColor->Pnext->Pnext != NULL)) 
        {
            QuantizedColor = QuantizedColor->Pnext;
            NumEntries++;
            Count += QuantizedColor->Count;
        }
        /* Save the values of the last color of the first half, and first
         * of the second half so we can update the Bounding Boxes later.
         * Also as the colors are quantized and the BBoxes are full 0..255,
         * they need to be rescaled.
         */
        MaxColor = QuantizedColor->RGB[SortRGBAxis]; /* Max. of first half */
    /* coverity[var_deref_op] */
        MinColor = QuantizedColor->Pnext->RGB[SortRGBAxis]; /* of second */
        MaxColor <<= (8 - BITS_PER_PRIM_COLOR);
        MinColor <<= (8 - BITS_PER_PRIM_COLOR);

        /* Partition right here: */
        NewColorSubdiv[*NewColorMapSize].QuantizedColors = QuantizedColor->Pnext;
        QuantizedColor->Pnext = NULL;
        NewColorSubdiv[*NewColorMapSize].Count = Count;
        NewColorSubdiv[Index].Count -= Count;
        NewColorSubdiv[*NewColorMapSize].NumEntries = NewColorSubdiv[Index].NumEntries - NumEntries;
        NewColorSubdiv[Index].NumEntries = NumEntries;
        for (j = 0; j < 3; j++) {
            NewColorSubdiv[*NewColorMapSize].RGBMin[j] = NewColorSubdiv[Index].RGBMin[j];
            NewColorSubdiv[*NewColorMapSize].RGBWidth[j] = NewColorSubdiv[Index].RGBWidth[j];
        }
        NewColorSubdiv[*NewColorMapSize].RGBWidth[SortRGBAxis] = NewColorSubdiv[*NewColorMapSize].RGBMin[SortRGBAxis]
            + NewColorSubdiv[*NewColorMapSize].RGBWidth[SortRGBAxis] - MinColor;
        NewColorSubdiv[*NewColorMapSize].RGBMin[SortRGBAxis] = MinColor;

        NewColorSubdiv[Index].RGBWidth[SortRGBAxis] = MaxColor - NewColorSubdiv[Index].RGBMin[SortRGBAxis];

        (*NewColorMapSize)++;
    }

    return GIF_OK;
}

/****************************************************************************
 Routine called by qsort to compare two entries.
*****************************************************************************/

static int
SortCmpRtn(const void *Entry1,
           const void *Entry2) {
       QuantizedColorType *entry1 = (*((QuantizedColorType **) Entry1));
       QuantizedColorType *entry2 = (*((QuantizedColorType **) Entry2));

       /* sort on all axes of the color space! */
       int hash1 = entry1->RGB[SortRGBAxis] * 256 * 256
                + entry1->RGB[(SortRGBAxis+1) % 3] * 256
                + entry1->RGB[(SortRGBAxis+2) % 3];
       int hash2 = entry2->RGB[SortRGBAxis] * 256 * 256
                + entry2->RGB[(SortRGBAxis+1) % 3] * 256
                + entry2->RGB[(SortRGBAxis+2) % 3];

    return hash1 - hash2;
}

/* end */
