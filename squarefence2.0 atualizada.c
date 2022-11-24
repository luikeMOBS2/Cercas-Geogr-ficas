#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//formulas:

float theta;
float rearth;
float pi;



float lat,lon,nlat,nlon;

float dy,dx;

rearth = 6378;

theta = int lat;

pi = 3.14;

dy = (pi/180) * rearth * cos(theta*pi/180);

dx = (pi/180) * rearth = 111 / degree;

nlat  = lat  + (dy / r_earth) * (180 / pi);

nlon = lon  + (dx / r_earth) * (180 / pi) / cos(lat * pi/180);



//conversor Lat/Lon para grau/min/seg

            // latitude recebe valor grau decimal ex:-3.73935
            // 3 será o grau
            // 0.73935 x 60 será o minuto(44,361 minutos), (44' minutos)
            // O resto desta multiplicação x 60 será o segundo (exs:0.361 x 60 = 21,66), 22'' segundos 
            //arredonde o valor do segundo se o decimal do valor tiver acima de 5, assim como no exemplo.
int main()
{
    printf ("Latitude: %f \t Longitude: %f\n", lat,lon);
    scanf ("%f, %f",&lat,%lon);
    printf("nova latitude %f \t nova longitude %f", nlat,nlon);




    // void geocord_converter(float decimal_degrees, char *coord){
    
    // float lat = decimal_degrees;
    // float lat_aux = (lat > 0) ? lat : -1*lat;
    
    // int grau = lat;
    // int min = (lat_aux - abs(grau))*60;
    // int seg = ((lat_aux - abs(grau))*60 - min)*60;
    // sprintf(coord, "%dº%d'%d''", grau, min, seg);
    // }

    // float lat = -3.73935;
    // float lat_aux = (lat > 0) ? lat : -1*lat;
    
    // int grau = lat;
    // int min = (lat_aux - abs(grau))*60;
    // int seg = ((lat_aux - abs(grau))*60 - min)*60;
    
    // printf("Grau: %d | Min: %d | Seg: %d\n", grau, min, seg);
    // printf("Latitude: %f => %dº%d'%d''", lat, grau, min, seg);
    
return 0;


}
 




