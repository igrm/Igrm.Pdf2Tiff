#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

#define MAGICKCORE_QUANTUM_DEPTH 16
#define MAGICKCORE_HDRI_ENABLE 0

void ThrowWandException(MagickWand* wand);

int main(int argc, char **argv) 
{
    MagickBooleanType status;
    MagickWand* magick_wand;
    PixelWand* color;
    int page_count = 0;
    int current_page = 0;

    if (argc != 4)
    {
        printf("Usage: %s document page image\n",argv[0]);
        exit(0);
    }

    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetResolution(magick_wand, 300, 300);
    status = MagickReadImage(magick_wand,argv[1]);
    if (status == MagickFalse) ThrowWandException(magick_wand);
    MagickResetIterator(magick_wand);

    page_count = MagickGetNumberImages(magick_wand);
    current_page = atoi(argv[2]);

    if(page_count <= 0)
    {
        printf("No pages in the document");
        exit(0);
    }
    else if (page_count > 0 && page_count < current_page) 
    {
        printf("Page %d not found", page_count);
        exit(0);
    }
  
    color = NewPixelWand();
    PixelSetColor(color, "white");

    printf("Processing page %d",current_page);
    MagickSetIteratorIndex(magick_wand, current_page-1);
    MagickSetImageAlphaChannel(magick_wand, RemoveAlphaChannel);
    MagickSetImageBackgroundColor(magick_wand, color);

    MagickSetFormat(magick_wand, "tiff");
    MagickWriteImages(magick_wand, argv[3], 1);

    magick_wand = DestroyMagickWand(magick_wand);
    color = DestroyPixelWand(color);

    MagickWandTerminus();
    return(0);
}

void ThrowWandException(MagickWand* wand) 
{ 
    char *description;
    ExceptionType severity;
    description = MagickGetException(wand,&severity);
    fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description);
    description = (char*) MagickRelinquishMemory(description); 
    exit(-1);
}