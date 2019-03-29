#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "png_util.h"
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

int main(int argc, char ** argv) {
    int nx = 500;
    int ny = 500;
    int nt = 10000; 
    int frame=0;
    //int nt = 1000000;
    int r,c,it;
    double dx,dy,dt;
    double max,min;
    double tmax;
    double dx2inv, dy2inv;
    char filename[sizeof "./images/file00000.png"];

    if(argc >= 2) {
      nx = atoi(argv[1]);
      ny = nx;
    }
    else if(argc >= 3)
    {
      nx = atoi(argv[1]);
      ny = nx;
      nt = atoi(argv[2]);
    }

    image_size_t sz; 
    sz.width=nx;
    sz.height=ny;
    struct timeval stime1, stime2, rtime1, rtime2, otime1, otime2;
    double setup_t = 0, run_t = 0, output_t = 0, elapsed_t;

    gettimeofday(&stime1, NULL);

    //make mesh
    double * h_z = (double *) malloc(nx*ny*sizeof(double));    
    double ** z = malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
    	z[r] = &h_z[r*nx];

    //Velocity
    double * h_v = (double *) malloc(nx*ny*sizeof(double));
    double ** v = malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
        v[r] = &h_v[r*nx];

    //Accelleration
    double * h_a = (double *) malloc(nx*ny*sizeof(double));
    double ** a = malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
        a[r] = &h_a[r*nx];

    //output image
    char * o_img = (char *) malloc(sz.width*sz.height*sizeof(char));
    char **output = malloc(sz.height * sizeof(char*));
    for (int r=0; r<sz.height; r++)
        output[r] = &o_img[r*sz.width];

    max=10.0;
    min=0.0;
    dx = (max-min)/(double)(nx-1);
    dy = (max-min)/(double)(ny-1);
    
    tmax=20.0;
    dt= (tmax-0.0)/(double)(nt-1);

    double x,y; 
    for (r=0;r<ny;r++)  {
    	for (c=0;c<nx;c++)  {
		x = min+(double)c*dx;
		y = min+(double)r*dy;
        	z[r][c] = exp(-(sqrt((x-5.0)*(x-5.0)+(y-5.0)*(y-5.0))));
        	v[r][c] = 0.0;
	        a[r][c] = 0.0;
    	}
    }
   
    gettimeofday(&stime2, NULL);
    setup_t = 1000000 * ((double)stime2.tv_sec - (double)stime1.tv_sec) + (double)stime2.tv_usec - (double)stime1.tv_usec;

    dx2inv=1.0/(dx*dx);
    dy2inv=1.0/(dy*dy);

    for(it=0;it<nt-1;it++) {
	//printf("%d\n",it);
	gettimeofday(&rtime1, NULL);
        for (r=1;r<ny-1;r++)  
    	    for (c=1;c<nx-1;c++)  {
		double ax = (z[r+1][c]+z[r-1][c]-2.0*z[r][c])*dx2inv;
		double ay = (z[r][c+1]+z[r][c-1]-2.0*z[r][c])*dy2inv;
		a[r][c] = (ax+ay)/2;
	    }
        for (r=1;r<ny-1;r++)  
    	    for (c=1;c<nx-1;c++)  {
               v[r][c] = v[r][c] + dt*a[r][c];
               z[r][c] = z[r][c] + dt*v[r][c];
            }
        gettimeofday(&rtime2, NULL);
        run_t += 1000000 * ((double)rtime2.tv_sec - (double)rtime1.tv_sec) + 
                 (double)rtime2.tv_usec - (double)rtime1.tv_usec;

	if (it % 100 ==0)
	{
            gettimeofday(&otime1, NULL);
    	    double mx,mn;
    	    mx = -999999;
            mn = 999999;
            for(r=0;r<ny;r++)
                for(c=0;c<nx;c++){
           	    mx = max(mx, z[r][c]);
           	    mn = min(mn, z[r][c]);
        	}
    	    for(r=0;r<ny;r++)
                for(c=0;c<nx;c++)
                    output[r][c] = (char) round((z[r][c]-mn)/(mx-mn)*255);

    	    sprintf(filename, "./images/file%05d.png", frame);
//            printf("Writing %s\n",filename);    
    	    write_png_file(filename,o_img,sz);
	    frame+=1;
            
            gettimeofday(&otime2, NULL);
            output_t += 1000000 * ((double)otime2.tv_sec - (double)otime1.tv_sec)
                        + (double)otime2.tv_usec - (double)otime1.tv_usec;
        }

    }
    gettimeofday(&otime1, NULL);
    
    double mx,mn;
    mx = -999999;
    mn = 999999;
    for(r=0;r<ny;r++)
        for(c=0;c<nx;c++){
	   mx = max(mx, z[r][c]);
	   mn = min(mn, z[r][c]);
        }

//    printf("%f, %f\n", mn,mx);

    for(r=0;r<ny;r++)
        for(c=0;c<nx;c++){  
	   output[r][c] = (char) round((z[r][c]-mn)/(mx-mn)*255);  
	}

    sprintf(filename, "./images/file%05d.png", it);
//    printf("Writing %s\n",filename);    
    //Write out output image using 1D serial pointer
    write_png_file(filename,o_img,sz);
    
    gettimeofday(&otime2, NULL);
    output_t += (double)otime2.tv_usec - (double)otime1.tv_usec;    

    printf("Setup time: %f \nRun time: %f \nOutput time: %f \n", \
           setup_t / 1e6, run_t / 1e6, output_t / 1e6);
    
    return 0;
}
