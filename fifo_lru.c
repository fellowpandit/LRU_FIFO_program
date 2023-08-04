#include<stdio.h>
#include<time.h>

int checkFrame(int *frames, int page, int frameSize){
    for(int i=0; i<frameSize; i++){
        if(frames[i] == page)
            return 1;
    }
    return 0;
};

void displayFrame(int *frames, int frameSize){
    for(int i=0; i<frameSize; i++) {
        if(frames[i] != -1)
            printf("\t%d\t", frames[i]);
        else
            printf("\t-\t");
    }
};

void displayResults(int *Pages, int numOfPages ,double *timeTaken, int faultCount){
    printf("\n\nPages:\t");
    for(int i=0; i<numOfPages; i++){
        printf("%d\t\t",Pages[i]);
    }
    printf("\nTime:\t");
    for(int i=0; i<numOfPages; i++){
        printf("%f\t",timeTaken[i]);
    }
    printf("\nTotal Number of Faults: %d",faultCount);
};

int getPosition(int *frames, int x){
    int position=0;
    while(frames[position]!=x){
        position++;
    }
    return position;
};

int maxDistance(int *distance, int frameSize){
    int max = 0;
    for(int i=0; i<frameSize; i++){
        if(distance[max] < distance[i]){
            max = i;
        }
    }
    return max;
};

void increaseDistance(int *distance, int frameSize){
    for(int j=0; j<frameSize; j++){
        distance[j]++;
    }
};

void LRU(int numOfFrames, int numOfPages, int *Pages, int *frames){
    clock_t start_t, end_t;
    int positionToExchange, distance[numOfFrames], faultCount=0, state = 1;
    double timeTaken[numOfPages];

    for(int i=0; i<numOfFrames; i++){
        faultCount++;
        start_t = clock();
        frames[i] = Pages[i];
        for(int j=0; j<numOfFrames; j++){
            if(frames[j]!=-1){
                distance[j]++;
            }else{
                distance[j] = 0;
            }
        }
        state = 0;
        end_t = clock();
        timeTaken[i] = (double)(end_t-start_t)/CLOCKS_PER_SEC;
        printf("\n");
        displayFrame(frames, numOfFrames);
        printf("\tmiss");
    }

    for(int i=numOfFrames; i<numOfPages; i++){
        start_t = clock();
        state = checkFrame(frames, Pages[i], numOfFrames);
        if(state == 0){
            faultCount++;
            positionToExchange = maxDistance(distance, numOfFrames);
            frames[positionToExchange] = Pages[i];
        }else{
            positionToExchange = getPosition(frames, Pages[i]);
        }
        distance[positionToExchange] = 0;
        increaseDistance(distance, numOfFrames);
        end_t = clock();
        timeTaken[i] = (double)(end_t-start_t)/CLOCKS_PER_SEC;
        printf("\n");
        displayFrame(frames, numOfFrames);
        if(state == 1)
            printf("\thit");
        else if(state == 0)
            printf("\tmiss");
    }

    displayResults(Pages, numOfPages, timeTaken, faultCount);
};

void FIFO(int numOfFrames, int numOfPages, int *Pages, int *frames){
    clock_t start_t, end_t;
    int positionToExchange, faultCount=0, pointer=0, state = 1;
    double timeTaken[numOfPages];

    for(int i=0; i<numOfFrames; i++){
        faultCount++;
        start_t = clock();
        frames[i] = Pages[i];
        end_t = clock();
        timeTaken[i] = (double)(end_t-start_t)/CLOCKS_PER_SEC;
        printf("\n");
        displayFrame(frames, numOfFrames);
        printf("\tmiss");
    }

    for(int i=numOfFrames; i<numOfPages; i++){
        start_t = clock();
        state = checkFrame(frames, Pages[i], numOfFrames);
        if(state == 0){
            faultCount++;
            positionToExchange = (pointer++) % numOfFrames;
            frames[positionToExchange] = Pages[i];
        }
        end_t = clock();
        timeTaken[i] = (double)(end_t-start_t)/CLOCKS_PER_SEC;
        printf("\n");
        displayFrame(frames, numOfFrames);
        if(state == 1)
            printf("\thit");
        else if(state == 0)
            printf("\tmiss");
    }

    displayResults(Pages, numOfPages, timeTaken, faultCount);
};

void initFrame(int *frames, int numOfFrames){
    for(int i=0; i<numOfFrames; i++){
        frames[i] = -1;
    }
}

int main() {
    int numOfFrames,numOfPages;
    printf("Enter the number of Frames: ");
    scanf("%d", &numOfFrames);
    printf("Enter the number of Pages: ");
    scanf("%d", &numOfPages);

    int refString[numOfPages], frames[numOfFrames];
    printf("Enter the Pages: ");
    for (int i = 0; i < numOfPages; i++) {
        scanf("%d", &refString[i]);
    }

    clock_t start_t, end_t;
    double total_t;

    printf("\n\nLRU:");
    initFrame(frames, numOfFrames);
    start_t = clock();
    LRU(numOfFrames, numOfPages, refString, frames);
    end_t = clock();
    total_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;
    printf("\nThe time taken for LRU is: %f seconds.\n",total_t);

    printf("\n\nFIFO:");
    initFrame(frames, numOfFrames);
    start_t = clock();
    FIFO(numOfFrames, numOfPages, refString, frames);
    end_t = clock();
    total_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;
    printf("\nThe time taken for FIFO is: %f seconds.\n",total_t);
    return 0;
}
