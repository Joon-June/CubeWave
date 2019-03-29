#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NUM_PRISMS 169
#define PI 3.14159265359
#define MAX_DISTANCE 100
#define INITIAL_HEIGHT 20
#define RAD_TO_DEG 57.2957795131

const short int LIME = 0xE736;
const short int DARK_LIME = 0x85D6;
const short int SHADE = 0x3AB0;
const short int IVORY = 0XFFFE;
const short int SHADE_TO_LIME_1 = 0x42B0;
const short int SHADE_TO_LIME_2 = 0x6BD1;
const short int SHADE_TO_LIME_3 = 0xD695;
const short int DARK_LIME_TO_SHADE_1 = 0x6453;
const short int DARK_LIME_TO_SHADE_2 = 0x4B71;
const short int DARK_LIME_TO_LIME_1 = 0xB696;
const short int DARK_LIME_TO_LIME_2 = 0xCED6;
const short int MIDDLE_TO_TOP = 0x7473;

double oscillationRate = 0.2;


/* Global variables for drawing */
volatile int pixel_buffer_start; // global variable
volatile int* pixel_ctrl_ptr;
volatile int *sFlag = (int *)0xFF20302C;

typedef struct RectangularPrism{

    int xloc;
    int yloc;

    // Length of rectangular prism
    // it should be half the actual length (will draw the prism up and down with respect to)
    // the middle point of the prism - (xloc, yloc)
    int height;

    double angle;

}RectPrism;

/*  Function Declarations */
void drawAPrism(RectPrism rectPrism);
void draw_line(short int x0, short int y0, short int x1, short int y1, short int color);
void plot_pixel(int x, int y, short int LINE_COLOR);
void swap(short int* e1, short int* e2);
void clear_screen();
void SBitSync();
// double cos(double x);
// double pow(double x, int power);


int main(){
    int i, j;
    srand(423);
    pixel_ctrl_ptr = (int *)0xFF203020;

    // initialize location and direction of prisms
    RectPrism rectPrism[NUM_PRISMS];

    int numPrismForEachRow = 1;
    int prismXLoc = 159;
    int prismYLoc = 40;
    // Set upper half of cube
    for(i = 0; i < 91;){

        for(j = 0; j < numPrismForEachRow; i++, j++){
            rectPrism[i].xloc = prismXLoc;
            rectPrism[i].yloc = prismYLoc;
            rectPrism[i].height = INITIAL_HEIGHT;

            prismXLoc = prismXLoc + 14;
            
        }

        prismXLoc = 159 - numPrismForEachRow * 7;
        prismYLoc = prismYLoc + 7;
        numPrismForEachRow++;

    }
    

    numPrismForEachRow = 12;
    prismXLoc = 159 - (numPrismForEachRow - 1) * 7;

    // Set lower half of cube
    for(i = 91; i < 169;){

        for(j = 0; j < numPrismForEachRow; i++, j++){
            rectPrism[i].xloc = prismXLoc;
            rectPrism[i].yloc = prismYLoc;
            rectPrism[i].height = INITIAL_HEIGHT;

            prismXLoc = prismXLoc + 14;
            
        }

        numPrismForEachRow--;
        prismXLoc = 159 - (numPrismForEachRow - 1) * 7;
        prismYLoc = prismYLoc + 7;

    }

    /* initialize osciilation */
    int middleX = rectPrism[(NUM_PRISMS - 1)/2].xloc;
    int middleY = rectPrism[(NUM_PRISMS - 1)/2].yloc;

    for(i = 0; i < NUM_PRISMS; i++){
        // Calcualte distance between current prism and middle prism
        double distanceFromMiddle = pow(rectPrism[i].xloc - middleX, 2) + 
                                    pow(rectPrism[i].yloc - middleY, 2);
        
        // 0 < ratio < 1
        // larger as it gets closer to the middle prism
        double ratio = distanceFromMiddle / MAX_DISTANCE;

        // Map the ratio into [0, 2 * pi]
        rectPrism[i].angle = ratio * PI * 2;

        rectPrism[i].height = INITIAL_HEIGHT * (0.5 * cos(rectPrism[i].angle) + 0.5);
        
    }

    
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer

    /* now, swap the front/back buffers, to set the front buffer location */
    SBitSync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer


    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();

        // Draw things we want here
        for(i = 0; i < NUM_PRISMS; i++){
            drawAPrism(rectPrism[i]);
        }

        // Update length of each prism to make it oscillate
        for(i = 0; i < NUM_PRISMS; i++){
            
            // Control oscillation speed here
            rectPrism[i].angle += oscillationRate;

            // Prevent overflow
            if(rectPrism[i].angle > 2*PI){
                rectPrism[i].angle -= 2*PI;
            }

            rectPrism[i].height = INITIAL_HEIGHT * (0.5 * cos(rectPrism[i].angle) + 0.5);
        }

        SBitSync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // reflect the address of new back buffer
    }
}


void drawAPrism(RectPrism rectPrism){
    int i, j, yMiddle, xOffset;
    int topStartingYPoint, bottomStartingYPoint;

    /* Draw top side */
    topStartingYPoint = rectPrism.yloc - rectPrism.height - 15;
    bottomStartingYPoint = rectPrism.yloc - rectPrism.height - 2;
    xOffset = 0;

    /* Draw upper half of top side */
    for(i = topStartingYPoint; i < topStartingYPoint + 7; i++){
        for(j = rectPrism.xloc - xOffset; j <= rectPrism.xloc + xOffset; j++){
            plot_pixel(j, i, DARK_LIME);
        }
        xOffset++;
    }

    /* Draw lower half of top side */
    xOffset = 0;
    for(i = bottomStartingYPoint; i > bottomStartingYPoint - 7; i--){
        for(j = rectPrism.xloc - xOffset; j <= rectPrism.xloc + xOffset; j++){
            plot_pixel(j, i, DARK_LIME);
        }
        xOffset++;
    }

    /* Draw boundary between top and shaded side */
    // DARK_LIME_TO_SHADE_1
    int topToShadeX = rectPrism.xloc - 1;
    int topToShadeY = rectPrism.yloc - rectPrism.height - 1;
    for(i = topToShadeX; i >= topToShadeX - 6; i--, topToShadeY--){
        plot_pixel(i, topToShadeY, DARK_LIME_TO_SHADE_1);
    }

    // DARK_LIME_TO_SHADE_2
    topToShadeX = rectPrism.xloc - 1;
    topToShadeY = rectPrism.yloc - rectPrism.height - 2;
    for(i = topToShadeX; i >= topToShadeX - 6; i--, topToShadeY--){
        plot_pixel(i, topToShadeY, DARK_LIME_TO_SHADE_2);
    }


    /* Draw boundary between top and bright side */
    // DARK_LIEM_TO_LIME_1
    int topToBrightX = rectPrism.xloc + 1;
    int topToBrightY = rectPrism.yloc - rectPrism.height - 1;
    for(i = topToBrightX; i <= topToBrightX + 6; i++, topToBrightY--){
        plot_pixel(i, topToBrightY, DARK_LIME_TO_LIME_1);
    }

    // DARK_LIEM_TO_LIME_2
    topToBrightX = rectPrism.xloc + 1;
    topToBrightY = rectPrism.yloc - rectPrism.height - 2;
    for(i = topToBrightX; i <= topToBrightX + 6; i++, topToBrightY--){
        plot_pixel(i, topToBrightY, DARK_LIME_TO_LIME_2);
    }

    /* Draw shaded side */
    yMiddle = rectPrism.yloc - 1;
    xOffset = 2;
    for(i = 0; i < 6; i++){

        for(j = yMiddle - rectPrism.height; j <= yMiddle + rectPrism.height; j++){
            plot_pixel(rectPrism.xloc - xOffset, j, SHADE);
        }
        xOffset++;
        yMiddle--;

    }


    /* Draw boundary between shaded and lime side - 1 */
    for(i = rectPrism.yloc - rectPrism.height; i <= rectPrism.yloc + rectPrism.height; i++){
        plot_pixel(rectPrism.xloc - 1, i, SHADE_TO_LIME_1);
    }

    /* Draw a pixel between middle part of side and top */
    plot_pixel(rectPrism.xloc, rectPrism.yloc - rectPrism.height - 1, MIDDLE_TO_TOP);



    /* Draw boundary between shaded and lime side - 2 */
    for(i = rectPrism.yloc - rectPrism.height; i <= rectPrism.yloc + rectPrism.height + 1; i++){
        plot_pixel(rectPrism.xloc, i, SHADE_TO_LIME_2);
    }

    /* Draw boundary between shaded and lime side - 3 */
    for(i = rectPrism.yloc - rectPrism.height; i <= rectPrism.yloc + rectPrism.height; i++){
        plot_pixel(rectPrism.xloc + 1, i, SHADE_TO_LIME_3);
    }



    /* Draw bright side */ 
    yMiddle = rectPrism.yloc - 1;
    xOffset = 2;
    for(i = 0; i < 6; i++){

        for(j = yMiddle - rectPrism.height; j <= yMiddle + rectPrism.height; j++){
            plot_pixel(rectPrism.xloc + xOffset, j, LIME);
        }
        xOffset++;
        yMiddle--;

    }

}


void SBitSync()
{	
   *pixel_ctrl_ptr = 1;
    
   while(((*sFlag) & 0x1) != 0){}
}

void draw_line(short int x0, short int y0, short int x1, short int y1, short int color)
{
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);

    if(is_steep){
        swap(&x0, &y0); 
        swap(&x1, &y1);
    }
    if(x0 > x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0); 
    int error = -(deltax / 2);
    int y = y0; 
    
    int y_step;

    if(y0 < y1){
        y_step = 1;
    }else{
        y_step = -1;
    }

    int x;
    for(x = x0; x <= x1; x++){
        
        if(is_steep){
            plot_pixel(y,x, color);
        }
        else{
            plot_pixel(x,y, color);
        }
        
        error = error + deltay;
        if(error >= 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}


void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void swap(short int* e1, short int* e2)
{
    int temp = *e1;
    *e1 = *e2;
    *e2 = temp;
}

void clear_screen()
{
    int x;
    int y;
    
    for(x = 0; x < 320; x++)
    {
        for(y = 0; y < 240; y++)
        {
            plot_pixel(x, y, IVORY);
        }
    }
}

