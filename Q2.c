#include<mpi.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int Find(int arr[], int element, int proc, MPI_Request request_cancel)
{
    printf("\nProcess %d\n", proc);
    printf("Arr: ");
    for(int p=0; p<10; p++)
    {
        printf("%d ", arr[p]);
    }
    printf("\n");

    bool check = 0;
    for(int i=0; i<10; i++)
    {
        if(arr[i]==element)
        {
            check = 1;   
            break;
        }
        int checker = 0;
        MPI_Irecv(&checker, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request_cancel);
        
        int flag = 0;
        MPI_Status status;
        MPI_Test(&request_cancel, &flag, &status); 
        if(flag==1){
            printf("Process %d: Aborting Search!\n", proc);
            return false;
        }
    }
    if(check==0){
        printf("Process %d Ended!\n", proc); 
    }
    
    return check;
}

int main(int argc,char*argv[])
{
    int rank, nproc, irecv;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    MPI_Status status_cancel;
    MPI_Request request_cancel = MPI_REQUEST_NULL;

	if(rank==0)
    {
        int size = nproc*10;
        int A[size];
        printf("Arr: ");
        for(int i=0; i<size; i++)
        {
            A[i] = rand()%100+i*5+rank%4;
            printf("%d ", A[i]);
        }
        printf("\n");

        int search_no = 0;
        printf("Enter Number: ");
        scanf("%d", &search_no); 

        int dis[nproc], count[nproc], t=0;
        for(int y=0; y<nproc; y++){
            count[y] = 10;
            dis[y] = t;
            t+=10;
        }

        for(int p=1; p<nproc; p++)
        {
            MPI_Send(&search_no, 1, MPI_INT, p, 20, MPI_COMM_WORLD);
        }

        int rev[10];
        MPI_Scatterv(A, count, dis, MPI_INT, &rev, 10, MPI_INT, 0, MPI_COMM_WORLD);  
        int result = Find(rev, search_no, rank, request_cancel);

        if(result==1){
            printf("-------------------------------------------------------\n");
            printf("Process %d: I have Found the Number :-)\n\n", rank);
            printf("Master: Process %d has Found the number!\n", rank);
            printf("Master: Informing all Processes to Abort!\n");
            printf("-------------------------------------------------------\n");
            for(int p=1; p<nproc; p++)
            {
                int end = 1;
                MPI_Isend(&end, 1, MPI_INT, p, 10, MPI_COMM_WORLD, &request_cancel);
            }
        }else{
            int checker = 0;
            MPI_Irecv(&checker, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);
            
            printf("-------------------------------------------------------\n");
            printf("Process %d: I have Found the Number :-)\n\n", status.MPI_SOURCE);
            printf("Master: Process %d has Found the number!\n", status.MPI_SOURCE);
            printf("Master: Informing all Processes to Abort!\n");
            printf("-------------------------------------------------------\n");
            if(checker==1){
                for(int p=1; p<nproc; p++)
                {
                    int end = 1;
                    MPI_Isend(&end, 1, MPI_INT, p, 10, MPI_COMM_WORLD, &request_cancel);
                }
            }
        }
    }else{
        int item = 0;
        MPI_Recv(&item, 1, MPI_INT, 0, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int rev[10];
        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, rev, 10, MPI_INT, 0, MPI_COMM_WORLD);   
        int result = Find(rev, item, rank, request_cancel);

        if(result==1){
            int found = 1;
            MPI_Isend(&found, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &request);
            // printf("Process %d: I have Found the Number :-)\n", rank);
            // printf("Process %d Found :)\n", rank);
        }
    }

    return 0;
}