/* Inclusões */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
/* Fim das inclusões */

/* File paths */
#define ADC_POT_1_VALUE "/sys/devices/ocp.3/helper.15/AIN5"
#define ADC_POT_0_VALUE "/sys/devices/ocp.3/helper.15/AIN1"
/* File paths */

#define SETUP "echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots"


/* Funções de manipulação */
void read_file(char *file_path, char *value){
	FILE *file = fopen(file_path, "r");
	fscanf(file, "%s", value);
	fclose(file);
}
/* Fim das funções de manipulação */