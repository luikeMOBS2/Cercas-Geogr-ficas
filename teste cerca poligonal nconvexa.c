#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//  Globals which should be set before calling these functions:
//  The following global arrays should be allocated before calling these functions:
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//  USAGE:
//  Call precalc_values() to initialize the constant[] and multiple[] arrays,
//  then call pointInPolygon(x, y) to determine if the point is in the polygon.
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

int    polyCorners[4];  //=  how many corners the polygon has (no repeats)
float  polyX[4];      //=  horizontal coordinates of corners
float  polyY[4];      //=  vertical coordinates of corners
float  x, y;        //=  point to be tested

float constant[4]; //= storage for precalculated constants (same size as polyX)
float multiple[4]; //= storage for precalculated multipliers (same size as polyX)

typedef struct position{
  float lat; //x
  float lon; //y
} position;

position testepoint;

void precalc_values() {

  int i, j=polyCorners-1;

  for(i=0; i<polyCorners; i++) {
    if(polyY[j]==polyY[i]) {
      constant[i]=polyX[i];
      multiple[i]=0; }
    else {
      constant[i]=polyX[i]-(polyY[i]*polyX[j])/(polyY[j]-polyY[i])+(polyY[i]*polyX[i])/(polyY[j]-polyY[i]);
      multiple[i]=(polyX[j]-polyX[i])/(polyY[j]-polyY[i]); }
    j=i; }}

bool pointInPolygon() {
    int   i,j;
    bool  oddNodes=0;

    for (i=0; i<polyCorners; i++) {
    if ((polyY[i]< y && polyY[j]>=y
    ||   polyY[j]< y && polyY[i]>=y)) {
      oddNodes^=(y*multiple[i]+constant[i]<x); }
    j=i; }

  return oddNodes; }


int main()
{
    
    int n, i;
    
    polyX[0] = -3.73935;
    polyY[0] = -38.47359;
    polyX[1] = -3.73894;
    polyY[1] = -38.47351;
    polyX[2] = -3.7391;
    polyY[2] = -38.47296;
    polyX[3] = -3.73945;
    polyY[3] = -38.47304;

   while(1)
    {
        printf("\ninforme o ponto para ser testado: ");
        printf("\nlatitute : ");
        scanf("%f",&testepoint.lat);
        printf("\nlongetude: ");
        scanf("%f",&testepoint.lon);
    //   testepoint.lat = -3.73926;
    //   testepoint.lon = -38.47346;

        pointInPolygon(testepoint,polyX,polyY,n);

  	printf("\nO ponto está dentro da cerca: %s \n\n", pointInPolygon(testepoint, polyX,polyY, polyCorners) ? "dentro" : "fora");
    }
}