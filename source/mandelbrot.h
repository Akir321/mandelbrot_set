#ifndef  __MANDELBROT_SET_H__
#define  __MANDELBROT_SET_H__

struct CoordData
{
    float shiftX;
    float shiftY;
    float zoomMult;
};


int runMandelbrotSet(void);


#endif //__MANDELBROT_SET_H__
