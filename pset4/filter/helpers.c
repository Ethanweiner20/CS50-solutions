#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Notes:
// - Directly alter "image" itself by using the image pointer
// - Remember to round any resulting floating point numbers to the nearest integers

// Constants and definitions

const int KERNEL_SIZE = 9;

typedef struct
{
    RGBTRIPLE pixels[KERNEL_SIZE];
    int size;
}
KERNEL;

const int GX_KERNEL[KERNEL_SIZE] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
const int GY_KERNEL[KERNEL_SIZE] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

// Headers

void copy_image(int height, int width, RGBTRIPLE old_image[height][width], RGBTRIPLE new_image[height][width]);
// Better to return an expression or modify image directly? Better design?
void blur_pixel(int height, int width, RGBTRIPLE copy[height][width], RGBTRIPLE image[height][width], int row, int col);
KERNEL create_kernel(int height, int width, RGBTRIPLE image[height][width], int row, int col);
RGBTRIPLE average_color(RGBTRIPLE pixels[], int num_pixels);
void black_border(int height, int width, RGBTRIPLE image[height][width]);
// Better to return an expression or modify image directly? Better design?
void sobel(int height, int width, RGBTRIPLE copy[height][width], RGBTRIPLE image[height][width], int row, int col);
BYTE sobel_value(char color, KERNEL kernel);
RGBTRIPLE sobel_pixel(KERNEL kernel, RGBTRIPLE pixel);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            RGBTRIPLE pixel = image[row][col];
            int avg = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
            // Assign to a new RGBTriple
            image[row][col].rgbtBlue = avg;
            image[row][col].rgbtGreen = avg;
            image[row][col].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // IDEA:
    // - For each row:
    // --- At column "col", swap with the pixel at "width-col"
    // --- Swapping procedure will require a temporarily memory allocation (use malloc?)

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width / 2; col++)
        {
            // Create a temporary location for the opposite pixel
            // opposite_pixel SHOULD contain a value, not an address?
            RGBTRIPLE opposite_pixel = image[row][width - col - 1];
            image[row][width - col - 1] = image[row][col];
            image[row][col] = opposite_pixel;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Use a copy of the image so that we can keep the blurred result separate
    RGBTRIPLE copy[height][width];
    copy_image(height, width, image, copy);

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            blur_pixel(height, width, copy, image, row, col);
        }
    }

    return;
}

void copy_image(int height, int width, RGBTRIPLE old_image[height][width], RGBTRIPLE new_image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            new_image[row][col] = old_image[row][col];
        }
    }
    return;
}

// ????????? Unnecessary to pass entire image? Should I pass a new 3x3 image of only the necessary pixels to apply the Sobel operator????????
void blur_pixel(int height, int width, RGBTRIPLE copy[height][width], RGBTRIPLE image[height][width], int row, int col)
{
    // Determine the pixels to use for the blur
    KERNEL kernel = create_kernel(height, width, copy, row, col);
    // Use the pixels for the blur
    image[row][col] = average_color(kernel.pixels, kernel.size);
}

// create_kernel : Int Int Image Int Int -> [List-of RGBTRIPLE]
// Provides an array of pixels, from _image_ bordering and including the pixel located at _row_ and _col_
// N.B. Will add a maximum of nine pixels, but excludes any pixels outside of the border
// N.B. Order of pixels: Top-left to bottom-right
KERNEL create_kernel(int height, int width, RGBTRIPLE image[height][width], int row, int col)
{
    KERNEL kernel;

    // Retrieve all the surrounding pixels and add to an array
    int size = 0;
    int counter = 0;
    // Iterate over 3 rows (one above, one on, and one below pixel)
    for (int i = row - 1; i <= row + 1; i++)
    {
        // Iterate over 3 columns (one left, one on, and one right of pixel)
        for (int j = col - 1; j <= col + 1; j++)
        {
            // Out of bounds
            if (i < 0 || i >= height || j < 0 || j >= width)
            {
                RGBTRIPLE empty_pixel;
                empty_pixel.rgbtRed = 0;
                empty_pixel.rgbtBlue = 0;
                empty_pixel.rgbtGreen = 0;
                kernel.pixels[counter] = empty_pixel;
            }
            else
            {
                kernel.pixels[counter] = image[i][j];
                size++;
            }
            counter++;
        }
    }

    kernel.size = size;
    return kernel;
}

// average_color: [List-of RGBTRIPLEs] Int -> RGBTRIPLE
// Produce a pixel that is the average color of the given _pixels_
RGBTRIPLE average_color(RGBTRIPLE pixels[], int num_pixels)
{
    float red_sum = 0;
    float green_sum = 0;
    float blue_sum = 0;

    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        red_sum += pixels[i].rgbtRed;
        green_sum += pixels[i].rgbtGreen;
        blue_sum += pixels[i].rgbtBlue;
    }

    // Create a new pixel with the average values
    RGBTRIPLE average_pixel;
    average_pixel.rgbtRed = round(red_sum / num_pixels);
    average_pixel.rgbtGreen = round(green_sum / num_pixels);
    average_pixel.rgbtBlue = round(blue_sum / num_pixels);

    return average_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Plan:
    // 1. Manipulate input image, converting all border pixels to black
    // 2. For each pixel (except border pixels):
    // - For each color:
    // --- Generate a GX and GY value
    // --- Generate a final value (Sobel operator)
    // --- Use this final value as the new value of the pixel (stronger edge detection ==> brighter color)

    RGBTRIPLE copy[height][width];
    copy_image(height, width, image, copy);
    // black_border(height, width, copy);

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            sobel(height, width, copy, image, row, col);
        }
    }

    return;
}

// sobel : Int Int Image Image Int Int -> _
// Alter the colors of the pixel located at row _row_ and column _col_ to reflect its sobel factor
// Idea: Focus on one color at a time
// ????????? Unnecessary to pass entire image? Should I pass a new 3x3 image of only the necessary pixels to apply the Sobel operator????????
void sobel(int height, int width, RGBTRIPLE copy[height][width], RGBTRIPLE image[height][width], int row, int col)
{
    // Array of the pixels to which we will apply the kernel
    KERNEL kernel = create_kernel(height, width, copy, row, col);

    // Directly manipulate the pixel using the kernel
    image[row][col] = sobel_pixel(kernel, image[row][col]);

}

// sobel
// Apply the sobel filter to one _pixel_ using the surrounding _kernel_
// Coulidn't directly alter the element of an array -- why is that?
RGBTRIPLE sobel_pixel(KERNEL kernel, RGBTRIPLE pixel)
{
    pixel.rgbtRed = sobel_value('r', kernel);
    pixel.rgbtBlue = sobel_value('b', kernel);
    pixel.rgbtGreen = sobel_value('g', kernel);
    return pixel;
}

BYTE sobel_value(char color, KERNEL kernel)
{
    int gx = 0;
    int gy = 0;

    for (int i = 0; i < KERNEL_SIZE; i++)
    {
        switch (color)
        {
            case 'r' :
                gx += kernel.pixels[i].rgbtRed * GX_KERNEL[i];
                gy += kernel.pixels[i].rgbtRed * GY_KERNEL[i];
                break;
            case 'b' :
                gx += kernel.pixels[i].rgbtBlue * GX_KERNEL[i];
                gy += kernel.pixels[i].rgbtBlue * GY_KERNEL[i];
                break;
            case 'g' :
                gx += kernel.pixels[i].rgbtGreen * GX_KERNEL[i];
                gy += kernel.pixels[i].rgbtGreen * GY_KERNEL[i];
                break;
            default :
                break;
        }

    }

    // Round to the nearest integer
    int result = round(sqrt(pow(gx, 2) + pow(gy, 2)));

    // Cap at 255
    if (result < 255)
    {
        return result;
    }
    return 255;
}

// black_border
// Changes the pixels along the edges and corners of _image_ to black
void black_border(int height, int width, RGBTRIPLE image[height][width])
{
    // Horizontal margins
    for (int col = 0; col < width; col++)
    {
        // Top margin
        image[0][col].rgbtRed = 0;
        image[0][col].rgbtGreen = 0;
        image[0][col].rgbtBlue = 0;

        // Bottom margin
        image[height - 1][col].rgbtRed = 0;
        image[height - 1][col].rgbtGreen = 0;
        image[height - 1][col].rgbtBlue = 0;
    }

    // Vertical margins
    for (int row = 0; row < height; row++)
    {
        // Left margin
        image[row][0].rgbtRed = 0;
        image[row][0].rgbtGreen = 0;
        image[row][0].rgbtBlue = 0;

        // Right margin
        image[row][width - 1].rgbtRed = 0;
        image[row][width - 1].rgbtGreen = 0;
        image[row][width - 1].rgbtBlue = 0;
    }
}