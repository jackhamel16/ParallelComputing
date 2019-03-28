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
    if(i < nx*ny) {
        if(r<ny-1 && r>0 && c<nx-1 && c>0){
            ax = (d_z[i+nx]+d_z[i-nx]-2.0*d_z[i])*dx2inv;
            ay = (d_z[i+1]+d_z[i-1]-2.0*d_z[i])*dy2inv;
            d_a[i] = (ax+ay)/2;
        }
        else
            d_a[i] = 0.0;
    }
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
}
    
int main(int argc, char ** argv) {
    int nx = 500;
    int ny = 500;
    int N = nx * ny;
    int nt = 10000; 
    int frame=0;
    //int nt = 1000000;
    int i,it;
    double dx,dy,dt;
    double max,min;
    double tmax;
    double dx2inv, dy2inv;
    char filename[sizeof "./images/cuda00000.png"];
    double *d_z, *d_v, *d_a, *d_output;

    image_size_t sz; 
    sz.width=nx;
    sz.height=ny;
    int mem_size = nx * ny * sizeof(double);
 
    //make mesh
    double * h_z = (double *) malloc(mem_size);
    //Velocity
    double * h_v = (double *) malloc(mem_size);
    //Accelleration
    double * h_a = (double *) malloc(mem_size);
    //output image
    unsigned char * o_img = (unsigned char *) malloc(sz.width*sz.height*sizeof(unsigned char));

    max=10.0;
    min=0.0;
    dx = (max-min)/(double)(nx-1);
    dy = (max-min)/(double)(ny-1);
    
    tmax=20.0;
    dt= (tmax-0.0)/(double)(nt-1);

    CUDA_CALL(cudaMalloc((void **)&d_z, mem_size));
    CUDA_CALL(cudaMalloc((void **)&d_v, mem_size));
    CUDA_CALL(cudaMalloc((void **)&d_a, mem_size));
    CUDA_CALL(cudaMalloc((void **)&d_output, sz.width*sz.height*sizeof(unsigned char)));

    int block_size = BLOCKDIM;
    int num_blocks = N / block_size + 1;
    dim3 dimBlock(block_size, 1, 1);
    dim3 dimGrid(num_blocks, 1, 1);

    array_setup<<<dimGrid, dimBlock>>>(nx, ny, dx, dy, d_z, d_v, d_a);

    dx2inv=1.0/(dx*dx);
    dy2inv=1.0/(dy*dy);
    for(it=0;it<nt-1;it++) {
        accel_update<<<dimGrid, dimBlock>>>(nx, ny, dx2inv, dy2inv, d_z, d_a);
        pos_update<<<dimGrid, dimBlock>>>(nx, ny, dt, d_z, d_v, d_a);    
	if (it % 100 ==0)
	{
            CUDA_CALL(cudaMemcpy(h_z, d_z, mem_size, cudaMemcpyDeviceToHost));
            double mx,mn;
            mx = -999999;
            mn = 999999;
            for(i=0; i<N; ++i) {
                mx = max(mx, h_z[i]);
                mn = min(mn, h_z[i]);
            }
            for(i=0; i<N; ++i) {
                o_img[i] = (char) round((h_z[i]-mn)/(mx-mn)*255);
            }

    	    sprintf(filename, "./images/cuda%05d.png", frame);
            printf("Writing %s\n",filename);    
            write_png_file(filename,o_img,sz);
            frame+=1;
        }

    }

    CUDA_CALL(cudaMemcpy(h_z, d_z, mem_size, cudaMemcpyDeviceToHost));
    
    double mx,mn;
    mx = -999999;
    mn = 999999;
    for(i=0; i<N; ++i) {
	 mx = max(mx, h_z[i]);
	 mn = min(mn, h_z[i]);
    }
        
    for(i=0; i<N; ++i) {
        o_img[i] = (char) round((h_z[i]-mn)/(mx-mn)*255);  
    }

    sprintf(filename, "./images/cuda%05d.png", it);
    printf("Writing %s\n",filename);    
    //Write out output image using 1D serial pointer
    write_png_file(filename,o_img,sz);
    return 0;
}
