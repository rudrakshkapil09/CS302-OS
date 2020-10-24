#include <stdlib.h>
#include <stdio.h>

int main()
{
    int i, n, req[50], mov = 0, cp, curr_val = 0, sav_val, difference = 0;
    printf("Enter the current position: ");
    scanf("%d", &cp);
    printf("Enter the number of requests (<50): ");
    scanf("%d", &n);
    printf("Enter the request order: ");
    for (i = 0; i < n; i++)
        scanf("%d", &req[i]);
	
	for (i = cp+1; i < n; i++) {
		curr_val = req[i];
		difference = abs(sav_val - curr_val);
		mov += difference;
		sav_val = curr_val;
	}

	for (i = cp-1; i >= 0; i--) {
		curr_val = req[i];
		difference = abs(curr_val - sav_val);
		mov += difference;
		sav_val = curr_val;

	}	

	printf("Movements => %d", mov);
}
