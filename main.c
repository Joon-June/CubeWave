#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PRISMS 169
#define PI 3.14159265359
#define MAX_DISTANCE 100
#define INITIAL_HEIGHT 20
#define RAD_TO_DEG 57.2957795131

double cosArray[] = {1, 
0.999848, 0.999391, 0.99863, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 
0.981627, 0.978148, 0.97437, 0.970296, 0.965926, 0.961262, 0.956305, 0.951056, 0.945519, 0.939693, 
0.93358, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891006, 0.882947, 0.87462, 0.866025, 
0.857167, 0.848048, 0.83867, 0.829037, 0.819152, 0.809017, 0.798635, 0.788011, 0.777146, 0.766044, 
0.754709, 0.743145, 0.731353, 0.71934, 0.707107, 0.694658, 0.681998, 0.66913, 0.656059, 0.642787, 
0.62932, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.5, 
0.484809, 0.469471, 0.45399, 0.438371, 0.422618, 0.406736, 0.390731, 0.374606, 0.358367, 0.34202, 
0.325568, 0.309016, 0.292371, 0.275637, 0.258819, 0.241921, 0.22495, 0.207911, 0.190808, 0.173648, 
0.156434, 0.139172, 0.121869, 0.104528, 0.0871551, 0.0697558, 0.0523353, 0.0348988, 0.0174517, 6.73205e-07, 
0.0174531, 0.0349002, 0.0523367, 0.0697572, 0.0871565, 0.104529, 0.12187, 0.139174, 0.156435, 0.173649, 
0.19081, 0.207912, 0.224952, 0.241923, 0.25882, 0.275638, 0.292372, 0.309018, 0.325569, 0.342021, 
0.358369, 0.374607, 0.390732, 0.406737, 0.422619, 0.438372, 0.453991, 0.469472, 0.48481, 0.500001, 
0.515039, 0.52992, 0.54464, 0.559194, 0.573577, 0.587786, 0.601816, 0.615662, 0.629321, 0.642788, 
0.65606, 0.669131, 0.681999, 0.694659, 0.707107, 0.719341, 0.731354, 0.743146, 0.75471, 0.766045, 
0.777147, 0.788011, 0.798636, 0.809018, 0.819153, 0.829038, 0.838671, 0.848049, 0.857168, 0.866026, 
0.87462, 0.882948, 0.891007, 0.898795, 0.906308, 0.913546, 0.920505, 0.927184, 0.933581, 0.939693, 
0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.97437, 0.978148, 0.981627, 0.984808, 
0.987689, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.99863, 0.999391, 0.999848, 1, 
0.999848, 0.999391, 0.998629, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 
0.981627, 0.978147, 0.97437, 0.970295, 0.965925, 0.961261, 0.956304, 0.951056, 0.945518, 0.939692, 
0.93358, 0.927183, 0.920504, 0.913545, 0.906307, 0.898793, 0.891006, 0.882947, 0.874619, 0.866025, 
0.857166, 0.848047, 0.83867, 0.829037, 0.819151, 0.809016, 0.798635, 0.78801, 0.777145, 0.766043, 
0.754708, 0.743144, 0.731353, 0.719339, 0.707106, 0.694657, 0.681997, 0.669129, 0.656058, 0.642786, 
0.629319, 0.61566, 0.601814, 0.587784, 0.573575, 0.559191, 0.544638, 0.529918, 0.515037, 0.499998, 
0.484808, 0.46947, 0.453989, 0.43837, 0.422617, 0.406735, 0.390729, 0.374605, 0.358366, 0.342018, 
0.325566, 0.309015, 0.29237, 0.275636, 0.258817, 0.24192, 0.224949, 0.20791, 0.190807, 0.173646, 
0.156433, 0.139171, 0.121867, 0.104526, 0.0871538, 0.0697545, 0.052334, 0.0348975, 0.0174504, 2.01962e-06, 
0.0174544, 0.0349015, 0.052338, 0.0697585, 0.0871578, 0.104531, 0.121871, 0.139175, 0.156437, 0.17365, 
0.190811, 0.207914, 0.224953, 0.241924, 0.258821, 0.275639, 0.292374, 0.309019, 0.32557, 0.342022, 
0.35837, 0.374609, 0.390733, 0.406739, 0.42262, 0.438373, 0.453992, 0.469474, 0.484812, 0.500002, 
0.51504, 0.529921, 0.544641, 0.559195, 0.573578, 0.587787, 0.601817, 0.615663, 0.629322, 0.642789, 
0.656061, 0.669132, 0.682, 0.69466, 0.707108, 0.719341, 0.731355, 0.743146, 0.754711, 0.766046, 
0.777147, 0.788012, 0.798637, 0.809018, 0.819153, 0.829039, 0.838672, 0.848049, 0.857169, 0.866027, 
0.874621, 0.882949, 0.891008, 0.898795, 0.906309, 0.913546, 0.920506, 0.927185, 0.933581, 0.939693, 
0.945519, 0.951057, 0.956306, 0.961262, 0.965926, 0.970296, 0.974371, 0.978148, 0.981628, 0.984808, 
0.987689, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.99863, 0.999391, 0.999848, 1, 
};


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
double cos(double x);
double pow(double x, int power);


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
            rectPrism[i].height = 20;

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
            rectPrism[i].height = 20;

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
        
        double ratio = distanceFromMiddle / MAX_DISTANCE;

        // Map the ratio into [0, 2*pi]
        rectPrism[i].angle = ratio * 2 * PI;

        rectPrism[i].height = rectPrism[i].height * cos(rectPrism[i].angle);
        
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
            
            rectPrism[i].angle += 0.1;

            if(rectPrism[i].angle > 2*PI){
                rectPrism[i].angle -= 2*PI;
            }

            rectPrism[i].height = INITIAL_HEIGHT * cos(rectPrism[i].angle);
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
double cos(double x){
    
    x = RAD_TO_DEG * x;
    
    if(x > 361.0){
        x = x-361.0;
    }

    int index = (int) x;

    return cosArray[index]; 
}

double pow(double x, int power){
    double val = 1;
    int i;
    for(i = 0; i < power; i++){
        val *= x;
    }

    return val;
}
