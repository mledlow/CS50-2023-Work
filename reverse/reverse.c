#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    if (argc != 3) //if there aren't 2 command line arguments
    {
        printf("Usage: ./reverse input.wav output.wav\n"); //print how to use and return
        return 1;
    }

    char *infile = argv[1];
    char *outfile = argv[2]; //stores strings for in and out files

    FILE *inptr = fopen(infile, "r"); //open infile in read-only mode
    if (inptr == NULL)
    {
        printf("Cannot find input file %s.\n", infile);
        return 1; //prints that it cannot find the input and returns
    }

    WAVHEADER wh;
    fread(&wh, sizeof(WAVHEADER), 1, inptr); //the header is read to wh

    if (!check_format(wh)) //if the file is not wave format, tell user
    {
        printf("Input file %s is not WAV format.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w"); //open outfile in write mode
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Cannot create output file %s.\n", outfile);
        return 3; //closes inptr file and prints that it can't create output
    }

    fwrite(&wh, sizeof(WAVHEADER), 1, outptr); //writes the header to the outptr

    int blockSize = get_block_size(wh); //gets the block size of our header
    int headerSize = 44;

    int array[blockSize]; //makes an array of blockSize

    fseek(inptr, -1 * blockSize, SEEK_END); //starts us off at the end block of the inptr file

    while (ftell(inptr) >= headerSize) //while our input position is bigger or equal to how large a header is
    {
        fread(array, blockSize, 1, inptr);
        fwrite(array, blockSize, 1, outptr); //reads the input file at position and writes it to the outptr
        fseek(inptr, -2 * blockSize, SEEK_CUR); //go back 2 blocks from here, since read goes 1 to the right, to get to new block
    }

    //this closes the files finally after both are done with
    fclose(inptr);
    fclose(outptr);

}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1; //if characters are WAVE, then we have right header
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    return (header.numChannels * header.bitsPerSample / 8); //divide by 8 to convert to bytes per sample
}