#include <stdio.h>
#include <stdbool.h>

int polyCorners = 4;                                           // how many corners the polygon has (no repeats)
float polyX[4] = {-3.73935, -3.73894, -3.7391, -3.73945};      // horizontal coordinates of corners
float polyY[4] = {-38.47359, -38.47351, -38.47296, -38.47304}; // vertical coordinates of corners
float x = -3.73921, y = -38.4734;                              // point to be tested
float constant[4] = {0};                                       // storage for precalculated constants (same size as polyX)
float multiple[4] = {0};                                       // storage for precalculated multipliers (same size as polyX)

void precalc_values()
{

    int i, j = polyCorners - 1;

    for (i = 0; i < polyCorners; i++)
    {

        if (polyY[j] == polyY[i])
        {
            constant[i] = polyX[i];
            multiple[i] = 0;
        }
        else
        {
            constant[i] = polyX[i] - (polyY[i] * polyX[j]) / (polyY[j] - polyY[i]) + (polyY[i] * polyX[i]) / (polyY[j] - polyY[i]);
            multiple[i] = (polyX[j] - polyX[i]) / (polyY[j] - polyY[i]);
        }
        j = i;
    }
}

bool pointInPolygon()
{

    int i = 0, j = polyCorners - 1;
    bool oddNodes = false;

    for (i = 0; i < polyCorners; i++)
    {

        if ((polyY[i] < y && polyY[j] >= y) || (polyY[j] < y && polyY[i] >= y))
        {
            oddNodes = oddNodes ^ (y * multiple[i] + constant[i] < x);
        }
        j = i;
    }

    return oddNodes;
}

// bool pointInPolygon() {

//     bool oddNodes=false, current=polyY[polyCorners-1]>y, previous;
//     for (int i=0; i<polyCorners; i++) {
//         previous=current;
//         current=polyY[i]>y;

//         if (current!=previous)
//             oddNodes^=y*multiple[i]+constant[i]<x;
//     }

//   return oddNodes;
// }

int main()
{

    precalc_values();

    printf("\nO ponto está dentro da cerca: %s \n\n", pointInPolygon() ? "dentro" : "fora");

    return 0;
}
