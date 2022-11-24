#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//structs:
typedef struct position{
float lat; //x
float lon; //y
}position;
position testepoint;
position center;
float distance(struct position v, struct position w);
  
//Macros
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


// formulas:

    

float degrees2radians(float degrees)
{
    return degrees*M_PI/180;
}
float sqr(float x)
{
    return x*x;
} 
//Equirectangular Approximation Distance Calculation Function:

float distance(struct position v, struct position w)
{   //Equirectangular Approximation
    float p1 = degrees2radians(v.lon - w.lon)* cos( 0.5*degrees2radians(v.lat+w.lat) ) ;
    float p2 = degrees2radians(v.lat - w.lat);
    return 6371000 * sqrt( p1*p1 + p2*p2);
} 


int main()
{

int i;

    position center;
    printf("Informe o centro da sua cerca: \n");
    printf("\nlatitute : ");
    scanf("%f",&center.lat);
    printf("\nlongetude: ");
    scanf("%f",&center.lon);
    
    float r;
    printf("Informe o Raio da sua cera em metros: ");
    scanf ("%f",&r);
     
      //Area da cerca:
    
    float pi = 3.14;
    float A;
     A = pi*r*r;
    
    printf("\nSua cerca é de %f km²\n", A);
   
    while(1)
    {
        printf("\ninforme o ponto para ser testado: ");
        printf("\nlatitute : ");
        scanf("%f",&testepoint.lat);
        printf("\nlongetude: ");
        scanf("%f",&testepoint.lon);
        
        float d = distance(center, testepoint);
        
        if(d < r)
            printf("\nO ponto está dentro da cerca por %f", d-r);
        else
            printf("\nO ponto está fora da cerca por  %f",d-r);
    }
}

