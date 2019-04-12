#include <stdio.h>          
#include <stdlib.h>
#include <math.h>
#include <png.h>
#include <mpi.h>
#include <sys/time.h>

void writeworld(char * filename, char ** my_world, int sz_x, int sz_y) {

   int width = sz_x+2;
   int height = sz_y+2;

   FILE *fp = fopen(filename, "wb");
   if(!fp) abort();

   png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (!png) abort();

   png_infop info = png_create_info_struct(png);
   if (!info) abort();

   if (setjmp(png_jmpbuf(png))) abort();

   png_init_io(png, fp);

   png_set_IHDR(
     png,
     info,
     width, height,
     8,
     PNG_COLOR_TYPE_RGB,
     PNG_INTERLACE_NONE,
     PNG_COMPRESSION_TYPE_DEFAULT,
     PNG_FILTER_TYPE_DEFAULT
   );
   png_write_info(png, info);

   png_bytep row = (png_bytep) malloc(3 * width * sizeof(png_byte));
//   printf("writing RGB %d, %d\n",width, height);

   int full_size_with_borders = (sz_x+2)*(sz_y+2);

   for(int r=0;r <height;r++) {
     for(int i=0;i < width;i++){
        switch (my_world[r][i]){
  	 case 1:
              row[(i*3)+0] = 255;
              row[(i*3)+1] = 255;
              row[(i*3)+2] = 255; 
              break;
	 case 2:
              row[(i*3)+0] = 255;
              row[(i*3)+1] = 0;
              row[(i*3)+2] = 0;
              break;
         case 3:
              row[(i*3)+0] = 0;
              row[(i*3)+1] = 255;
              row[(i*3)+2] = 0;
              break;
         case 4:
              row[(i*3)+0] = 0;
              row[(i*3)+1] = 0;
              row[(i*3)+2] = 255;
              break;
         case 5:
              row[(i*3)+0] = 255;
              row[(i*3)+1] = 255;
              row[(i*3)+2] = 0;
              break;
         case 6:
              row[(i*3)+0] = 255;
              row[(i*3)+1] = 0;
              row[(i*3)+2] = 255;
              break;
         case 7:
              row[(i*3)+0] = 0;
              row[(i*3)+1] = 255;
              row[(i*3)+2] = 255;
              break;
         case 8:
              row[(i*3)+0] = 233;
              row[(i*3)+1] = 131;
              row[(i*3)+2] = 0;
              break;
	 default:
              row[(i*3)+0] = 0;
              row[(i*3)+1] = 0;
              row[(i*3)+2] = 0;
              break;
      }
    }
    png_write_row(png, row);
   }
  png_write_end(png, NULL);
//  printf("done writing file\n");

  free(row);
  fclose(fp);
}

int main(int argc, char **argv)
{
   int rank, size;
   MPI_Status Status;
   MPI_Request Request;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   struct timeval time1, time2;
   double setup_t = 0, run_t = 0, output_t = 0;

   gettimeofday(&time1, NULL);

   srand(rank);
   
   //Simulation Parameters
   int global_sz_x = 1000;
   int global_sz_y = 1000;
   if(argc == 2)
     global_sz_x = atoi(argv[1]);
   else if (argc > 2) {
    global_sz_x = atoi(argv[1]);
    global_sz_y = atoi(argv[2]);
   }

   int sz_x = global_sz_x; 
   int sz_y = ceil(global_sz_y / size);

   printf("sizes: %d %d \n", sz_x, sz_y);
   char filename[sizeof "./images/file00000.png"];
   int img_count = 0;

   float pop_prob = 0.75;
   float rumor_prob = 0.25;
   int num_loops = 1000;
   int NUM_RUMORS = 7;

   //Allocate space for my world
   int full_size_with_borders = (sz_x+2)*(sz_y+2);
   char * world_mem = (char *) malloc(2*full_size_with_borders*sizeof(char));
   for (int i=0; i<full_size_with_borders; i++)
   	world_mem[i] = 0; 

   char ** my_world[2];
   my_world[0] = (char **) malloc((sz_y+2) * sizeof(char*));
   my_world[1] = (char **) malloc((sz_y+2) * sizeof(char*));
   for (int which=0; which < 2; which++) {
      for (int r=0; r<(sz_y+2); r++)
         my_world[which][r] = &world_mem[(r*(sz_x+2))+which*full_size_with_borders];
   }
//   printf("After Allocate\n");
   int which = 0;
   
   //Inicialize Random World
   for (int r=0; r<sz_y+2; r++)
   	for (int c=0; c<sz_x+2; c++)
	{
	   float rd = ((float) rand())/(float)RAND_MAX;
           if (rd < pop_prob) 
        	my_world[which][r][c] = 1;
 	}
//   printf("After Inicialize\n");
   
   //Pick Rumor Starting location
   for (int u; u < NUM_RUMORS; u++) {
      int r = (int) ((float)rand())/(float)RAND_MAX*(sz_y+2);
      int c = (int) ((float)rand())/(float)RAND_MAX*(sz_x+2);
      my_world[which][r][c] = u+2; 
//      printf("Starting a Rumor %d, %d = %d\n",r,c,u+2); 
   } 
   //writeworld("start.png", my_world[which], sz_x, sz_y);
//   printf("After Start location picked \n");

   gettimeofday(&time2, NULL);
   setup_t = (double)time2.tv_sec - (double)time1.tv_sec + 
             ((double)time2.tv_usec - (double)time1.tv_usec)/1e6;

   //Main Time loop
   for(int t=0; t<num_loops;t++) {
     gettimeofday(&time1, NULL);
//     printf("Before Allocating Boundaries\n");
     //Communicate Edges
     int* ubound_out = (int *) malloc((sz_x+2) * sizeof(int));
     int* lbound_out = (int *) malloc((sz_x+2) * sizeof(int));
//     printf("Before Copying\n");
     for(int c=0; c<sz_x+2; ++c) {
//       printf("Copying %d\n",c);
       ubound_out[c] = my_world[0][sz_y][c];
       lbound_out[c] = my_world[0][1][c];
     }
     int ubound_in[sz_x+2];
     int lbound_in[sz_x+2];
//     printf("Before Communicating %d\n", rank);
     if(rank==0) {
       MPI_Isend(&lbound_out, sz_x+2, MPI_INT, size-1, 1, MPI_COMM_WORLD, &Request);
       MPI_Isend(&ubound_out, sz_x+2, MPI_INT, (rank+1)%size, 2, MPI_COMM_WORLD, &Request);
       MPI_Barrier(MPI_COMM_WORLD);
       MPI_Irecv(&lbound_in, sz_x+2, MPI_INT, size-1, 2, MPI_COMM_WORLD, &Request);
       MPI_Irecv(&ubound_in, sz_x+2, MPI_INT, (rank+1)%size, 1, MPI_COMM_WORLD, &Request);
     }
     else if(rank==size-1) {
       MPI_Isend(&lbound_out, sz_x+2, MPI_INT, (rank-1)%size, 1, MPI_COMM_WORLD, &Request);
       MPI_Isend(&ubound_out, sz_x+2, MPI_INT, 0, 2, MPI_COMM_WORLD, &Request);
       MPI_Barrier(MPI_COMM_WORLD);
       MPI_Irecv(&lbound_in, sz_x+2, MPI_INT, (rank-1)%size, 2, MPI_COMM_WORLD, &Request);
       MPI_Irecv(&ubound_in, sz_x+2, MPI_INT, 0, 1, MPI_COMM_WORLD, &Request);
     }
     else {
       MPI_Isend(&lbound_out, sz_x+2, MPI_INT, (rank-1)%size, 1, MPI_COMM_WORLD, &Request);
       MPI_Isend(&ubound_out, sz_x+2, MPI_INT, (rank+1)%size, 2, MPI_COMM_WORLD, &Request);
       MPI_Barrier(MPI_COMM_WORLD);
       MPI_Irecv(&lbound_in, sz_x+2, MPI_INT, (rank-1)%size, 2, MPI_COMM_WORLD, &Request);
       MPI_Irecv(&ubound_in, sz_x+2, MPI_INT, (rank+1)%size, 1, MPI_COMM_WORLD, &Request);
     }
     for(int c=0; c<sz_x+2; ++c) {
       my_world[0][sz_y+1][c] = ubound_in[c];
       my_world[0][0][c] = lbound_in[c];
     }
 
     //Loop Edges
//   for (int c=1; c<sz_x+1; c++){
//      my_world[which][0][c] = my_world[which][sz_y][c];
//      my_world[which][sz_y+1][c] = my_world[which][1][c];
//   }
     for (int r=1; r<sz_y+1; r++){
        my_world[which][r][0] = my_world[which][r][sz_x];
        my_world[which][r][sz_x+1] = my_world[which][r][1];
     }
          
//   printf("Step %d\n",t);
     int rumor_counts[NUM_RUMORS+2];
     for (int r=1; r<sz_y+1; r++) {
        for (int c=1; c<sz_x+1; c++)
        { 
          my_world[!which][r][c] = my_world[which][r][c];
          if (my_world[which][r][c] >0) { 
     	for(int n=0;n<NUM_RUMORS+2;n++)
                rumor_counts[n] = 0;
             rumor_counts[my_world[which][r-1][c]]++; 
             rumor_counts[my_world[which][r+1][c]]++; 
             rumor_counts[my_world[which][r][c-1]]++; 
             rumor_counts[my_world[which][r][c+1]]++; 

     	float rd = ((float) rand())/(float)RAND_MAX; 
     	float my_prob = 0;
     	for(int n=2;n<NUM_RUMORS+2;n++) {
     	   if (rumor_counts[n] > 0) {
     		my_prob += rumor_prob*rumor_counts[n]; 
                	if(rd <= my_prob) {
           			//printf("."); 
     	      		my_world[!which][r][c] = n;
     	      		break;
                	}
     	   } 
     	}
          }
        }
     }

     gettimeofday(&time2, NULL);
     run_t += (double)time2.tv_sec - (double)time1.tv_sec + 
              ((double)time2.tv_usec - (double)time1.tv_usec)/1e6;
     	
     which = !which;
     if(t%10==0) {   
       if(rank==0) {    
         //Send everything back to master for saving.
         gettimeofday(&time1, NULL);

         //Allocate space for global world
         int global_size_with_borders = (global_sz_x+2)*(global_sz_y+2);
         char * global_world_mem = (char *) malloc(2*global_size_with_borders*sizeof(char));
         for (int i=0; i<global_size_with_borders; i++)
         	global_world_mem[i] = 0; 
     
         char ** global_world[2];
         global_world[0] = (char **) malloc((global_sz_y+2) * sizeof(char*));
         global_world[1] = (char **) malloc((global_sz_y+2) * sizeof(char*));
         for (int which=0; which < 2; which++) {
            for (int r=0; r<(global_sz_y+2); r++)
               global_world[which][r] = &global_world_mem[(r*(sz_x+2))+which*global_size_with_borders];
         }
         // Fill global world
         // fill with master world
         for(int r=1; r<sz_y+1; ++r) {
           for(int c=1; c<sz_x+1; ++c) {
             global_world[0][r][c] = my_world[0][r][c];
           }
         }
//       for(int c=0; c<sz_x+2; ++c)
//         printf("%d ",global_world[0][10][c]);
//       printf("\n");
//       for(int r=1; r<sz_y+1; ++r) {
//         for(int c=1; c<sz_x+1; ++c) {
//           global_world[which][r][c] = my_world[which][r][c];
//         }
//       }
         // fill with all other procs' worlds

         int in_row[sz_x+2];
         for(int proc=1; proc<size; ++proc) {
           for(int r=1; r<sz_y+1; ++r) {
             MPI_Recv(&in_row, sz_x+2, MPI_INT, proc, r, MPI_COMM_WORLD, &Status); 
             for(int c=1; c<sz_x+1; ++c) {
              // if(t==10)printf("r=%d c=%d \n",proc*sz_y+r, c);
               global_world[0][proc*sz_y+r][c] = in_row[c];
             }
           }
         }
//         printf("%d\n",global_world[0][100][999]);
         sprintf(filename, "./images/file%05d.png", img_count);
         writeworld(filename, global_world[0], global_sz_x, global_sz_y);
         img_count++;
         gettimeofday(&time2, NULL);
         output_t += (double)time2.tv_sec - (double)time1.tv_sec + 
                     ((double)time2.tv_usec - (double)time1.tv_usec)/1e6;
       }
       else {
         for(int r=0; r<sz_y+2; ++r) {
           int * out_row = (int *) malloc((sz_x+2)*sizeof(int));
           for(int c=0; c<sz_x+2; ++c)
             out_row[c] = my_world[0][r][c];
           MPI_Send(&out_row, sz_x+2, MPI_INT, 0, r, MPI_COMM_WORLD);
         }
       }
     }
   } 

   //Write out output image using 1D serial pointer
   //writeworld("end.png", world_mem, sz_x, sz_y);
//   printf("After Loop\n");
   free(my_world[0]);
   free(my_world[1]);
//   printf("After Clean up\n");
//   printf("Setup Time = %f\nRun Time = %f\nOutput Time = %f\n",
//          setup_t, run_t, output_t);

   MPI_Finalize();
   return(0);
}

