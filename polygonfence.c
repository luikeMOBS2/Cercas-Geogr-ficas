#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>



typedef struct position{
  float lat; //x
  float lon; //y
} position;

// typedef struct datetime{
//   uint32_t time;
//   uint32_t date;
// } datetime;

typedef struct fenceProperty{
  int sideOutside; 
  float distance;
  float bearing;
} fenceProperty;


position testepoint;
int polyCorners;

        float distance(struct position v, struct position w);
        bool pointInPolygon(struct position testepoint, struct position fencePt[], int Corners);
        fenceProperty geofence(struct position testepoint, struct position fencePt[], int Corners);
        float distBehind(struct position testepoint, struct position w, struct position v);
        float degrees2radians(float degrees);
        float sqr(float x);
        position findProjection(struct position testepoint, struct position v, struct position w);
        float dist2segment(struct position p, struct position v, struct position w, struct position * projection);
        float bearing2fence(struct position testepoint, struct position projection);


//Macros
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))




float degrees2radians(float degrees)
{
    return degrees*M_PI/180;
}
float sqr(float x)
{
    return x*x;
}


//Step 1: Equirectangular Approximation Distance Calculation Function
float distance(struct position v, struct position w)
{   //Equirectangular Approximation
    float p1 = degrees2radians(v.lon - w.lon)* cos( 0.5*degrees2radians(v.lat+w.lat) ) ;
    float p2 = degrees2radians(v.lat - w.lat);
    return 6371000 * sqrt( p1*p1 + p2*p2);
} 


//Step 2: Test if the point is within the polygon of points ;; Testar se o ponto está dentro do polígono de pontos
bool pointInPolygon(struct position testepoint, struct position fencePt[], int Corners)
{
    //oddNodes = 1 means within the polygon, oddNodes = 0 outside the polygon.
    int i, j=Corners-1;
    bool  oddNodes=0;
    float temp=0;
    int sidebehind;
    struct position projection, projectionmax;

    for(i=0; i<Corners; i++)
    {		
        if(((fencePt[i].lat< testepoint.lat && fencePt[j].lat>=testepoint.lat)  
        || (fencePt[j].lat< testepoint.lat && fencePt[i].lat>=testepoint.lat))  
        &&  (fencePt[i].lon<=testepoint.lon || fencePt[j].lon<=testepoint.lon)) 
        {
        oddNodes^=(fencePt[i].lon+(testepoint.lat-fencePt[i].lat) / 
        (fencePt[j].lat-fencePt[i].lat)*(fencePt[j].lon-fencePt[i].lon)<testepoint.lon);
        }
    j=i;
    }

    return oddNodes; //True Point is within the boundary, False then outside
}

//Step 3: Find which sides of the boundary we are outside;;Descubra de quais lados da borda estamos fora.
float distBehind(struct position testepoint, struct position w, struct position v)
{
    //Returns a negative if outside that boundary;; Retorna negativo se estiver fora dos limites.
    float Fplat = w.lat;
    float Fplon = w.lon;
    //Calculate the unit length normal vector: Fn ;; calculando vetor comprimento nominal do vetor: Fn
    float Fnlat =  w.lon-v.lon;       // y' 
    float Fnlon = -(w.lat-v.lat);    //-x'
    float mag = sqrt(sqr(Fnlat)+sqr(Fnlon));
      Fnlat /= mag;           //Unit length
      Fnlon /= mag;
     //testepoint-Fp
      Fplat = testepoint.lat- Fplat;   //Reuse variables
      Fplon = testepoint.lon - Fplon;
      //Return the dot product
    return Fplat*Fnlat + Fplon*Fnlon;
}

position findProjection(struct position testepoint, struct position v, struct position w)
{
    position projection;
     //Check if the two side points are in the same location (avoid dividing by zero later)
    float l = distance(v,w);
    if(l==0) return v;
    //Find the max and min x and y points
    float minx = MIN(v.lat, w.lat);
    float maxx = MAX(v.lat, w.lat);
    float miny = MIN(v.lon, w.lon);
    float maxy = MAX(v.lon, w.lon);
    
    //struct position projection;
    if(testepoint.lat<minx && testepoint.lon<miny)
    {//testepoint does not fall between the two points and is closest to the lower corner
        projection.lat = minx; 
        projection.lon = miny; 
    }   
    else if(testepoint.lat>maxx && testepoint.lon>maxy)
    {//testepoint does not fall between the two points and is closest to the lower corner
        projection.lat = maxx; 
        projection.lon = maxy; 
    }
    else
    {//testepoint does fall between the two points, project point onto the line
        float x1=v.lat, y1=v.lon, x2=w.lat, y2=w.lon, x3=testepoint.lat, y3=testepoint.lon;
        float px = x2-x1, py = y2-y1, dAB = px*px + py*py;
        float u = ((x3 - x1) * px + (y3 - y1) * py) / dAB;
        float x = x1 + u * px, y = y1 + u * py;
        projection.lat = x; 
        projection.lon = y; 
    }
    
    return projection;
}


float bearing2fence(struct position testepoint, struct position projection)
{
    float y = sin(testepoint.lon-projection.lon) * cos(testepoint.lat);
    float x = cos(projection.lat)*sin(testepoint.lat) - sin(projection.lat)*cos(testepoint.lat)*cos(testepoint.lon-projection.lon);
    float brng = atan2(y, x)*180/M_PI;
    return brng;
}

fenceProperty geofence(struct position testepoint, struct position fencePt[], int Corners)
{
    bool inside = pointInPolygon(testepoint, fencePt, Corners);
    float temp=0;
    position projection, tempprojection;
    fenceProperty result;
    result.distance=0; result.sideOutside=0; result.bearing=0;

    if(!inside)
    {
    if( distBehind(testepoint,fencePt[Corners-1],fencePt[0]) < 0) 
        {
            projection=findProjection(testepoint, fencePt[Corners-1],fencePt[0]);
            result.distance=distance(testepoint, projection);
            result.sideOutside=0;
        }
    for(int i=1; i<Corners; i++)
    { 
        if( distBehind(testepoint,fencePt[i-1],fencePt[i]) < 0 ) 
        {
            tempprojection=findProjection(testepoint, fencePt[i-1],fencePt[i]); 
            temp=distance(testepoint, tempprojection);
        if (temp > result.distance) 
            {
            result.distance = temp; //if further away from side update the max distance behind
            result.sideOutside=i;
            projection=tempprojection;
            }
        }
    }

  //Determine left or right hand side alert
    result.bearing = bearing2fence(testepoint, projection);
  //Get Compass bearing, determine left of right
    } 
    return result;
}


int main()
{
    
    int n, i;
    position testepoint;
    // printf(" informe o número de pontos: ");
    // scanf ("%d",&n);
    // polyCorners = n;
    // position points[n];
    
    // for(i=0; i<n ; i++)
    // {
    //   printf("\ncoordenadas do ponto %d", i);
    //     printf("\nlatitute : ");
    //     scanf("%f",&points[i].lat);
    //     printf("\nlongetude: ");
    //     scanf("%f",&points[i].lon);
    // }
   
    polyCorners = 10;
    position points[10];
    
    points[0].lat = -3.73935;
    points[0].lon = -38.47359;
    points[1].lat = -3.73894;
    points[1].lon = -38.47351;
    points[2].lat = -3.7391;
    points[2].lon = -38.47296;
    points[3].lat = -3.73945;
    points[3].lon = -38.47304;

   while(1)
    {
        printf("\ninforme o ponto para ser testado: ");
        printf("\nlatitute : ");
        scanf("%f",&testepoint.lat);
        printf("\nlongetude: ");
        scanf("%f",&testepoint.lon);
    //   testepoint.lat = -3.73926;
    //   testepoint.lon = -38.47346;

        pointInPolygon(testepoint,points,n);
        
    //printf("\nDistance \t %.5G \n",distance(points[i],testepoint));

  	printf("\nO ponto está dentro da cerca: %s \n\n", pointInPolygon(testepoint, points, polyCorners) ? "dentro" : "fora");


	//Step 3 and 4: Which sides are we outside and by how far? ;; De que lados estamos do lado de fora e a que distância?
   	printf ("tamanho da cerca\n");
   	printf("Side \t Step 3 \t Step 4 \t\n");

  	for( i=0; i<polyCorners; i++){
  	    float dist = distance(points[i],testepoint);
        printf("%i<->%i \t %G \t %G\n",polyCorners-1,i, distBehind(testepoint,points[i],points[i])*100000, dist);
  	}

  	printf("\n");

  	fenceProperty result = geofence(testepoint, points, polyCorners);
  	
  	printf("\nDistance \t %.2G \n",result.distance);
  	printf("\nSide Outside \t %i \n",result.sideOutside);
  	printf("\nBearing \t %.2G \n",result.bearing);
    }

}



