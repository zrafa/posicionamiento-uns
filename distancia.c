
/* Calcula la distancia Spearman */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* datos :
 *        MAC (ID). Ej: 38:6B:1C:27:7C:DA
 *        
 */
# define ERs_FILE "ERs.txt"
# define GPSs_ERs_FILE "gps-er.txt"
# define MAC_LEN	17	/* 17 letras tiene una MAC de ER */
# define LAT_LONG_LEN	80	/* len de la cadena latitud,longitud */
# define LINE_LEN	80	/* 17 letras tiene una MAC de ER */
# define Nc	9	/* 30 ERs maximo en la matriz */
# define POS_N	40	/* maxima cantidad de puntos. Ej: 5 puntos por cuadra, 4 cuadras */
typedef struct v_st {
	int ID;
	int pos;
} v_st;

typedef struct fing_st {
	char mac[MAC_LEN];
	int rss;
} fing_st;

// v_st Vt[Nc];
// v_st Vr[Nc];

int tf_ex[5][2] = { {100, 62}, {110, 60}, {2, 54}, {5, 43}, {99, 40} };

v_st vt[] = { {2,3}, {5,4}, {10,9}, {99,5}, {100,1}, {110,2}, {111,9}, {200,9}, {201,9}};
v_st vr[] = { {2,9}, {5,2}, {10,5}, {99,9}, {100,1}, {110,3}, {111,4}, {200,6}, {201,7}};


char ERs[Nc][MAC_LEN];
fing_st RFINGs[POS_N][Nc]; /* hay que guardar la MAC (17 letras) y la potencia (un int) en cada RFING */
char GPSs[Nc][LAT_LONG_LEN];

int ERs_load(void)
{
	int f, i, n;
	char mac[MAC_LEN+1];

	f = open(ERs_FILE, O_RDONLY);
	i=0;
	
	while (read(f, mac, MAC_LEN+1)) {
		memcpy(ERs[i], mac, MAC_LEN);
		// printf("%s\n", ERs[i]);
		i++;
	}
	close(f);

	for (i=0; i<Nc; i++) {
		memcpy(mac, ERs[i], MAC_LEN);
		mac[MAC_LEN] = '\0';
		printf("%s\n", mac);
	};

	return n;
}


int GPSs_ERs_load(void)
{
	int i, j, n;
	char mac[LINE_LEN];
	char l[LINE_LEN];
	char *p;
	FILE *f;
	ssize_t r;

	/* falta inicializar las estructuras de datos en 0 */

	f = fopen(GPSs_ERs_FILE, "r");
	i=-1; j=0; n-0;
	while (fgets(l, sizeof(l), f)) {
		if (strchr(l, ',') == NULL) {		/* es una MAC y potencia */
			memcpy(RFINGs[i][j].mac, l, MAC_LEN);
			p = strchr(l, ' ');
			RFINGs[i][j].rss = atoi(p);
			j++;
		} else {
			i++;
			strncpy(GPSs[i], l, LAT_LONG_LEN);
		}
	}
	n = i;
	fclose(f);

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

void main(void)
{
	GPSs_ERs_load();
	// ERs_load();
	
	float p = p_calc(vt, vr);
	printf("c: %f\n", p);
	printf("d=1-p = %f\n", (float)1-p);

}
