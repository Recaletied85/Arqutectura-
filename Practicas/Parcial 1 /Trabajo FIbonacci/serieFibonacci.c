#include <stdio.h>
#include <stdlib.h>


int main(){
	int x=0, j=1,m=0;

	printf("-------------Generador serie fibonacci---------");
	printf("-------Dame un numero para la serie fibonacci: ");
	scanf("%d",&x);

	for (int i=0; i<x; i++){
		printf("%d, %d,  ",m,j);
		m += j;
		j += m;

	}

	return 0;

}
