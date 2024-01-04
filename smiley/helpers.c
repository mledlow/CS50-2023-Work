#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++) //double for loop to iterate through the image's height and width
        {
            if ((image[h][w].rgbtBlue) == 0 && (image[h][w].rgbtGreen == 0) && (image[h][w].rgbtRed == 0)) //if all colors are 0, black pixel
            {
                image[h][w].rgbtBlue = 0xFF;
                image[h][w].rgbtGreen = 0xFF;
                image[h][w].rgbtRed = 0xFF; //change all black pixels to be white
            }
        }
    }
}
