#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "png_util.h"
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#define CUDA_CALL(x) {cudaError_t cuda_error__ = (x); if (cuda_error__) printf("CUDA error: " #x " returned \"%s\"\n", cudaGetErrorString(cuda_error__));}

#define BLOCKDIM 1024

__global__ void array_setup(int nx, int ny, double dx, double dy,
                       double* d_z, double* d_v, double* d_a) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if(i < nx * ny) {
        int row = i / nx;
        int col = i % nx;
	double x = (double)col*dx;
	double y = (double)row*dy;
        d_z[i] = exp(-(sqrt((x-5.0)*(x-5.0)+(y-5.0)*(y-5.0))));
        d_v[i] = 0.0;
	d_a[i] = 0.0;
    }
}

__global__ void accel_update(int nx, int ny, double dx2inv, double dy2inv,
                             double* d_z, double* d_a) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;    
    double ax, ay;
    int r = i / nx;
    int c = i % nx;
    if(r<ny-1 && r>0 && c<nx-1 && c>0){
        ax = (d_z[i+nx]+d_z[i-nx]-2.0*d_z[i])*dx2inv;
        ay = (d_z[i+1]+d_z[i-1]-2.0*d_z[i])*dy2inv;
        d_a[i] = (ax+ay)/2;
    }
//    if(i<(nx*(ny-1)) && i>nx && i%(nx-1)==0 && i%(nx)==0){
//        ax = (d_z[i+nx]+d_z[i-nx]-2.0*d_z[i])*dx2inv;
//        ay = (d_z[i+1]+d_z[i-1]-2.0*d_z[i])*dy2inv;
//        d_a[i] = (ax+ay)/2;
//    }
}

__global__ void pos_update(int nx, int ny, double dt, double* d_z,
                           double* d_v, double* d_a) {    
    int i = blockDim.x * blockIdx.x + threadIdx.x;    
    int r = i / nx;
    int c = i % nx;
    if(r<ny-1 && r>0 && c<nx-1 && c>0){
        d_v[i] = d_v[i] + dt*d_a[i];
        d_z[i] = d_z[i] + dt*d_v[i];
    }
//    if(i<(nx*(ny-1)) && i>nx && i%(nx-1)==0 && i%(nx)==0) {
//        d_v[i] = d_v[i] + dt*d_a[i];
//        d_z[i] = d_z[i] + dt*d_v[i];
//    }
}

int main(int argc, char ** argv) {
    int nx = 500;
    int ny = 500;
    int N = nx * ny;
    int nt = 10000; 
    int frame=0;
    //int nt = 1000000;
    int r,c,it;
    double dx,dy,dt;
    double max,min;
    double tmax;
    double dx2inv, dy2inv;
    char filename[sizeof "./images/cuda00000.png"];
    double *d_z, *d_v, *d_a;

    image_size_t sz; 
    sz.width=nx;
    sz.height=ny;
    int mem_size = nx * ny * sizeof(double);
 
    //make mesh
    //double * h_z = (double *) malloc(mem_size);
    double * h_z = (double *) malloc(mem_size);
    double * h_z2 = (double *) malloc(mem_size);
    double ** z = (double **) malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
    	z[r] = &h_z2[r*nx];

    //Velocity
    double * h_v = (double *) malloc(mem_size);
    double * h_v2 = (double *) malloc(mem_size);
    double ** v = (double **) malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
        v[r] = &h_v2[r*nx];
    
    //Accelleration
    double * h_a = (double *) malloc(mem_size);
    double * h_a2 = (double *) malloc(mem_size);
    double ** a = (double **) malloc(ny * sizeof(double*));
    for (r=0; r<ny; r++)
        a[r] = &h_a2[r*nx];

    //output image
    unsigned char * o_img = (unsigned char *) malloc(sz.width*sz.height*sizeof(unsigned char));
    unsigned char **output = (unsigned char **) malloc(sz.height * sizeof(unsigned char*));
    for (int r=0; r<sz.height; r++)
        output[r] = &o_img[r*sz.width];

    max=10.0;
    min=0.0;
    dx = (max-min)/(double)(nx-1);
    dy = (max-min)/(double)(ny-1);
    
    tmax=20.0;
    dt= (tmax-0.0)/(double)(nt-1);

    CUDA_CALL(cudaMalloc((void **)&d_z, mem_size));
    CUDA_CALL(cudaMalloc((void **)&d_v, mem_size));
    CUDA_CALL(cudaMalloc((void **)&d_a, mem_size));

    CUDA_CALL(cudaMemcpy(d_z, h_z, mem_size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMemcpy(d_v, h_v, mem_size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMemcpy(d_a, h_a, mem_size, cudaMemcpyHostToDevice));

    int block_size=BLOCKDIM;
    int num_blocks = nx / block_size; // nx-2?? also won't this be too low since int division?
    dim3 dimBlock(block_size, 1, 1);
    dim3 dimGrid(num_blocks, 1, 1);
 //   array_setup<<<dimGrid, dimBlock>>>(nx, ny, dx, dy, d_z, d_v, d_a);

    int i;
    double x,y; 
    for (r=0;r<ny;r++)  {
    	for (c=0;c<nx;c++)  {
                i = r * nx + c;
		x = min+(double)c*dx;
		y = min+(double)r*dy;
        	z[r][c] = exp(-(sqrt((x-5.0)*(x-5.0)+(y-5.0)*(y-5.0))));
        	h_z[i] = exp(-(sqrt((x-5.0)*(x-5.0)+(y-5.0)*(y-5.0))));
        	v[r][c] = 0.0;
        	h_v[i] = 0.0;
	        a[r][c] = 0.0;
        	h_a[i] = 0.0;
                //printf("%f \n", z[r][c]); 
    	}
    }
 
    CUDA_CALL(cudaMemcpy(d_z, h_z, mem_size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMemcpy(d_v, h_v, mem_size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMemcpy(d_a, h_a, mem_size, cudaMemcpyHostToDevice));
    
//  CUDA_CALL(cudaMemcpy(h_z, d_z, mem_size, cudaMemcpyDeviceToHost));
//  CUDA_CALL(cudaMemcpy(h_v, d_v, mem_size, cudaMemcpyDeviceToHost));
//  CUDA_CALL(cudaMemcpy(h_a, d_a, mem_size, cudaMemcpyDeviceToHost));

//  for(int r=0; r<ny; ++r)
//      for(int c=0; c<nx; ++c) {
//          i = r * nx + c;
//          printf("%f %f \n", h_z[i], z[r][c]);
//      }
    dx2inv=1.0/(dx*dx);
    dy2inv=1.0/(dy*dy);

    for(it=0;it<nt-1;it++) {
	//printf("%d\n",it);
        for (r=1;r<ny-1;r++)  
    	    for (c=1;c<nx-1;c++)  {
		double ax = (z[r+1][c]+z[r-1][c]-2.0*z[r][c])*dx2inv;
		double ay = (z[r][c+1]+z[r][c-1]-2.0*z[r][c])*dy2inv;
		a[r][c] = (ax+ay)/2;
	    }
        accel_update<<<dimGrid, dimBlock>>>(nx, ny, dx2inv, dy2inv, d_z, d_a);
        for (r=1;r<ny-1;r++)  
    	    for (c=1;c<nx-1;c++)  {
               v[r][c] = v[r][c] + dt*a[r][c];
               z[r][c] = z[r][c] + dt*v[r][c];
            }
        pos_update<<<dimGrid, dimBlock>>>(nx, ny, dt, d_z, d_v, d_a);    

	if (it % 100 ==0)
	{
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
                    output[r][c] = (unsigned char) round((z[r][c]-mn)/(mx-mn)*255);

    	    sprintf(filename, "./images/cuda%05d.png", frame);
            printf("Writing %s\n",filename);    
    	    write_png_file(filename,o_img,sz);
	    frame+=1;
        }

    }

    CUDA_CALL(cudaMemcpy(h_z, d_z, mem_size, cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(h_v, d_v, mem_size, cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(h_a, d_a, mem_size, cudaMemcpyDeviceToHost));

    for(int r=0; r<ny; ++r)
        for(int c=0; c<nx; ++c) {
            i = r * nx + c;
            printf("%f %f \n", h_z[i], z[r][c]);
        }
    
    double mx,mn;
    mx = -999999;
    mn = 999999;
    for(r=0;r<ny;r++)
        for(c=0;c<nx;c++){
	   mx = max(mx, z[r][c]);
	   mn = min(mn, z[r][c]);
        }

    printf("%f, %f\n", mn,mx);

    for(r=0;r<ny;r++)
        for(c=0;c<nx;c++){  
	   output[r][c] = (char) round((z[r][c]-mn)/(mx-mn)*255);  
	}

    sprintf(filename, "./images/file%05d.png", it);
    printf("Writing %s\n",filename);    
    //Write out output image using 1D serial pointer
    write_png_file(filename,o_img,sz);
    return 0;
}
