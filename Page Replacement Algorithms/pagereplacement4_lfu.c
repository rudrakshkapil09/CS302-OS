#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int findLFU(int freq[], int n)
{
    int i, minimum = freq[0], pos = 0;

    for (i = 1; i < n; ++i)
    {
        if(freq[i] < minimum)
        {
            minimum = freq[i];
            pos = i;
        }
    }
    
    return pos;
}

int main()
{
    int no_of_frames, no_of_pages, frames[10], pages[30], freq[10], flag1, flag2, i, j, pos, faults = 0;
    printf("Enter number of frames: ");
    scanf("%d", &no_of_frames);
    printf("Enter number of pages: ");
    scanf("%d", &no_of_pages);
    printf("Enter pages (as string): ");
    for (i = 0; i < no_of_pages; ++i)
        scanf("%d", &pages[i]);
    
    for (i = 0; i < no_of_frames; ++i) {
        frames[i] = -1;
        freq[i] = 0;
    }
    
    for (i = 0; i < no_of_pages; ++i)
    {
        flag1 = flag2 = 0;
        
        //page already in a frame
        for (j = 0; j < no_of_frames; ++j)
        {
            if (frames[j] == pages[i])
            {
                freq[j]++;
                flag1 = flag2 = 1;
                break;
            }
        }
        
        if (flag1 == 0)
        {
            //any empty frames present - no need to find lfu
            for (j = 0; j < no_of_frames; ++j)
            {
                if (frames[j] == -1)
                {
                    faults++;
                    frames[j] = pages[i];
                    freq[j] = 1;
                    flag2 = 1;
                    break;
                }
            }
        }
        
        //must replace
        if (flag2 == 0)
        {
            pos = findLFU(freq, no_of_frames);
            faults++;
            frames[pos] = pages[i];
            freq[pos] = 1;
        }
        
        printf("\n");
    
        for (j = 0; j < no_of_frames; ++j)
            printf("%d\t", frames[j]);
    }
    
    printf("\n\nTotal Page Faults = %d", faults);
    return 0;
}
