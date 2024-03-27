#ifndef  __MANDELBROT_SET_H__
#define  __MANDELBROT_SET_H__

struct CoordData
{
    float shiftX;
    float shiftY;
    float zoomMult;
};


const int WindowWidth  = 1400;
const int WindowHeight = 1000;

const int MaxPointN    = 256;


const float dx         = 1.f / WindowWidth;
const float dy         = 1.f / WindowHeight;

const float Radius2Max = 100.f;

const float CenterX    =  -1.325f;
const float CenterY    =  0.f;



int runMandelbrotSet(void);


#endif //__MANDELBROT_SET_H__
