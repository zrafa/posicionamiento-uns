
/* spearman.c: Maneja y prepara los datos de gps y ers.
 *	       Calcula la distancia Spearman.
 *	       (Spearman Rank Correlation Coefficient - SRC )
 *
 * Curso de posicionamiento y seguimiento.
 * Prof. Dr. Favio Mason
 * Alumno: Rafael Zurita
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spearman.h"

/* datos :
 *	  Latitud,Longitud. Ej: 3849.84448,06804.07505
 *        MAC (ID) y nivel. Ej: 38:6B:1C:27:7C:DA 60
 */

/* base de datos */
char GPSs[Nc][LAT_LONG_LEN]; /* listado de localizaciones (puntos gps) */
fing_st RFINGs[POS_N][Nc];   /* firmas: por cada loc. tenemos un listado de AP y nivel */

/* listado de estaciones de radio (access points) */
char ERs[Nc][MAC_LEN];


void v_init(v_st *v)
{
	int i;

	for (i=0; i<Nc; i++) {
		v[i].id = i;
		v[i].pos = Nc;
	}
}

/* fing st init */
void f_init(fing_st *f)
{
	int i;

	for (i=0; i<Nc; i++)
		f[i].rss = -1;

}

/* BD (gps y er/rfing) st init */
void bd_init(fing_st *f[])
{
	int i;

	for (i=0; i<POS_N; i++)
		f_init(RFINGs[i]);

}

/* paso de ordenacion: */ 
void v_ordenar(fing_st *f, v_st *v)
{
	int i, j;

	v_init(v);
	for (i=0; i<Nc; i++) {
		if (f[i].rss == -1)
			break;

		/* buscamos la ER en el listado gral. */
		for (j=0; j<Nc; j++) {

			/* si son iguales */
			if (strncmp(f[i].mac, ERs[j], MAC_LEN) == 0) {
				v[j].pos = i+1;	
				break;
			}
		}
	}

}


int ERs_load(void)
{
	int i, n;
	char mac[MAC_LEN+1];
	char l[LINE_LEN];
	FILE *f;

	f = fopen(ERs_FILE, "r");
	i=0;
	
	while (fgets(l, sizeof(l), f)) {
		memcpy(ERs[i], l, MAC_LEN);
		i++;
	}
	fclose(f);

	/* Ordenamos cada entrada de forma decreciente */
 	for (i=0; i<Nc; i++)
                qsort (RFINGs[i], Nc, sizeof(fing_st), compare);

	return n;
}


int compare (const void * a, const void * b)
{
  fing_st *data_1 = (fing_st *)a;
  fing_st *data_2 = (fing_st *)b;
  return ( data_2->rss - data_1->rss );
}


void print_datos(void)
{
	int i,j;
	char mac[LINE_LEN];
	for (i=0; i<Nc; i++) {
		printf("gps: %s\n", GPSs[i]);
		for (j=0; j<Nc; j++) {
			if (RFINGs[i][j].rss != 0) {
				memcpy(mac, ERs[i], MAC_LEN);
				mac[MAC_LEN] = '\0';
				printf("\tmac: %s  -  rss: %i\n",RFINGs[i][j].mac, RFINGs[i][j].rss);
			}
		}
	};
}

/* carga la base de datos (CDB) */
int GPSs_ERs_load(void)
{
	int i, j, n;
	char mac[LINE_LEN];
	char l[LINE_LEN];
	char *p;
	FILE *f;
	ssize_t r;

	/* inicializa las estructuras de datos en -1 */
	for (i=0; i<Nc; i++)
		f_init(RFINGs[i]);

	f = fopen(GPSs_ERs_FILE, "r");
	i=-1; j=0; n-0;
	while (fgets(l, sizeof(l), f)) {
		if (strchr(l, ',') == NULL) {		/* es una MAC y potencia */
			memcpy(RFINGs[i][j].mac, l, MAC_LEN);
			p = strchr(l, ' ');
			RFINGs[i][j].rss = atoi(p);
			j++;
		} else {				/* latitud y longitud */
			i++;
			j=0;
			strncpy(GPSs[i], l, LAT_LONG_LEN);
		}
	}
	n = i;
	fclose(f);
	
	return n;
}

float pt_pr_calc(v_st *v) 
{
	float p = 0;
	int i;

	for (i=0; i<Nc; i++)
		p = p + (float)(v[i].pos);

	p = p / Nc;
	return p;
}

/* 
 * p_calc: calcula el Spearman Rank Correlation Coefficient (SRC)
 */
float p_calc(v_st *vt, v_st *vr)
{
	int i;
	float p;
	float pt, pr;

	float t1, t2, t3;

	/*    t1 / (sqrt(t2 * t3))    */

	pt = pt_pr_calc(vt);
	pr = pt_pr_calc(vr);

	printf("pt= %f\n", pt);
	printf("pr= %f\n", pr);

	t1=0; t2=0; t3=0; p=0;

	for (i=0; i<Nc; i++) 	
		t1 = t1 + ((vt[i].pos - pt) * (vr[i].pos - pr));
	
	for (i=0; i<Nc; i++) 	
		t2 = t2 + powf((vt[i].pos - pt), 2);

	for (i=0; i<Nc; i++) 	
		t3 = t3 + powf((vr[i].pos - pr), 2);
	
	p = t1 / sqrtf(t2*t3);

	return p;
}
