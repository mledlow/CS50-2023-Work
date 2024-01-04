#include "helpers.h"
#include "math.h"

int get_kernal_coefficient(int x, int y, int isY);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float averagePixelColor = 0; //this is our average pixel color that we will calc for each
    int roundedPixelColor = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            averagePixelColor = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0;
            roundedPixelColor = round(averagePixelColor);
            image[i][j].rgbtBlue = roundedPixelColor; //calculates average of RGB and assigns it to all
            image[i][j].rgbtRed = roundedPixelColor;
            image[i][j].rgbtGreen = roundedPixelColor;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE placeholderPixel;
    placeholderPixel.rgbtBlue = 0;
    placeholderPixel.rgbtRed = 0;
    placeholderPixel.rgbtGreen = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            placeholderPixel = image[i][j]; //our current pixel
            image[i][j] = image[i][width - (j + 1)]; //gets the opposite side's pixel
            image[i][width - (j + 1)] = placeholderPixel; //and swaps it with the one at the j-th place
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE array[height][width]; //copy array where we will store things

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array[i][j].rgbtBlue = 0;
            array[i][j].rgbtRed = 0;
            array[i][j].rgbtGreen = 0; //goes through our copy image array and initializes it
        }
    }

    float arrayRed = 0;
    float arrayBlue = 0;
    float arrayGreen = 0; //a set of variables that we will use for RGB of copy array
    float boxCount = 0; //how many boxes we are averaging from with box blur

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) //iterate through pixels by height and width
        {
            boxCount = 0;
            arrayRed = 0;
            arrayBlue = 0;
            arrayGreen = 0; //reset these 4 variables

            for (int y = (i - 1); y < (i + 2); y++) //check around the pixel and begin box blur in 3x3 grid
            {
                for (int x = (j - 1); x < (j + 2); x++)
                {
                    if (y < 0 || y >= height || x < 0 || x >= width)
                    {
                        //do nothing, because we are out of bounds
                    }
                    else
                    {
                        arrayRed += image[y][x].rgbtRed;
                        arrayBlue += image[y][x].rgbtBlue;
                        arrayGreen += image[y][x].rgbtGreen; //adds image RGB to respective variables and increments boxcount
                        boxCount++;
                    }
                }
            }
            arrayRed /= boxCount;
            arrayBlue /= boxCount;
            arrayGreen /= boxCount; //average the RGB over the number of boxes used

            array[i][j].rgbtRed = round(arrayRed);
            array[i][j].rgbtBlue = round(arrayBlue);
            array[i][j].rgbtGreen = round(arrayGreen); //rounds the average and sets it to that pixel in array
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = array[i][j].rgbtRed;
            image[i][j].rgbtGreen = array[i][j].rgbtGreen;
            image[i][j].rgbtBlue = array[i][j].rgbtBlue; //at the end, assign our array to be the blurred image
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE array[height][width]; //copy array where we will store things

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array[i][j].rgbtRed = 0;
            array[i][j].rgbtGreen = 0;
            array[i][j].rgbtBlue = 0; //goes through our copy image array and initializes it
        }
    }

    float kernalRedX = 0;
    float kernalRedY = 0;
    float kernalGreenX = 0;
    float kernalGreenY = 0;
    float kernalBlueX = 0;
    float kernalBlueY = 0;
    float kernalRed = 0;
    float kernalGreen = 0;
    float kernalBlue = 0;
    int kernalXCoefficient = 0;
    int kernalYCoefficient = 0;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) //iterate through pixels by height and width
        {
            kernalRedX = 0;
            kernalRedY = 0;
            kernalGreenX = 0;
            kernalGreenY = 0;
            kernalBlueX = 0;
            kernalBlueY = 0; //resets all kernels
            kernalRed = 0;
            kernalGreen = 0;
            kernalBlue = 0;

            for (int y = -1; y <= 1; y++) //check around the pixel in 3x3 box
            {
                for (int x = -1; x <= 1; x++)
                {

                    kernalXCoefficient = get_kernal_coefficient(x, y, 0);
                    kernalYCoefficient = get_kernal_coefficient(x, y, 1);

                    if ((i + y) < 0 || (i + y) >= height || (j + x) < 0 || (j + x) >= width)
                    {
                        //don't add anything since we're out of bounds
                    }
                    else
                    {
                        kernalRedX += (image[y + i][x + j].rgbtRed * kernalXCoefficient);
                        kernalRedY += (image[y + i][x + j].rgbtRed * kernalYCoefficient);
                        kernalGreenX += (image[y + i][x + j].rgbtGreen * kernalXCoefficient);
                        kernalGreenY += (image[y + i][x + j].rgbtGreen * kernalYCoefficient);
                        kernalBlueX += (image[y + i][x + j].rgbtBlue * kernalXCoefficient);
                        kernalBlueY += (image[y + i][x + j].rgbtBlue * kernalYCoefficient);
                    }
                }
            }

            kernalRed = sqrt((kernalRedX * kernalRedX) + (kernalRedY * kernalRedY));
            kernalGreen = sqrt((kernalGreenX * kernalGreenX) + (kernalGreenY * kernalGreenY));
            kernalBlue = sqrt((kernalBlueX * kernalBlueX) + (kernalBlueY * kernalBlueY)); //calculates the overall kernel via sqrt

            if (kernalRed > 255)
            {
                array[i][j].rgbtRed = 255;
            }
            else
            {
                array[i][j].rgbtRed = round(kernalRed);
            }

            if (kernalGreen > 255)
            {
                array[i][j].rgbtGreen = 255;
            }
            else
            {
                array[i][j].rgbtGreen = round(kernalGreen);
            }

            if (kernalBlue > 255)
            {
                array[i][j].rgbtBlue = 255; // caps all colors at 255 or lower
            }
            else
            {
                array[i][j].rgbtBlue = round(kernalBlue);  //rounds kernals and stores
            }

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = array[i][j].rgbtRed;
            image[i][j].rgbtGreen = array[i][j].rgbtGreen;
            image[i][j].rgbtBlue = array[i][j].rgbtBlue; //at the end, assign our array to be the blurred image
        }
    }

    return;
}


int get_kernal_coefficient(int x, int y, int isY)
{
    int kernalCoeff = 0;
    if (isY) //then we're returning the y coefficient, not the x
    {
        if (x == 0) //in the middle of grid
        {
            kernalCoeff = 2 * y;
        }
        else
        {
            kernalCoeff = 1 * y;
        }
    }
    else
    {
        if (y == 0)
        {
            kernalCoeff = 2 * x;
        }
        else
        {
            kernalCoeff = 1 * x;
        }
    }
    return kernalCoeff;
}