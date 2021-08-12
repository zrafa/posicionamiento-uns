
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
	int id;
	int pos;
} v_st;

typedef struct fing_st {
	char mac[MAC_LEN];
	int rss;
} fing_st;

// v_st Vt[Nc];
// v_st Vr[Nc];

int tf_ex[5][2] = { {100, 62}, {110, 60}, {2, 54}, {5, 43}, {99, 40} };

/* caso de Favio */
//v_st vt[] = { {2,3}, {5,4}, {10,9}, {99,5}, {100,1}, {110,2}, {111,9}, {200,9}, {201,9}};
//v_st vr[] = { {2,9}, {5,2}, {10,5}, {99,9}, {100,1}, {110,3}, {111,4}, {200,6}, {201,7}};

/* caso identico */
//v_st vt[] = { {0,3}, {1,4}, {2,9}, {3,5}, {4,1}, {5,2}, {6,9}, {7,9}, {8,9}};
//v_st vr[] = { {0,9}, {1,2}, {2,5}, {3,9}, {4,1}, {5,3}, {6,4}, {7,6}, {8,7}};

v_st vt[Nc];
v_st vr[Nc];

char ERs[Nc][MAC_LEN];
fing_st RFINGs[POS_N][Nc]; /* hay que guardar la MAC (17 letras) y la potencia (un int) en cada RFING */
char GPSs[Nc][LAT_LONG_LEN];

void v_init(v_st *v)
{
	int i;

	for (i=0; i<Nc; i++) {
		v[i].id = i;
		v[i].pos = Nc;
	}
}

fing_st tfing[] = { 

	{"38:6B:1C:27:7C:00", 62},
	{"38:6B:1C:27:7C:06", 60},
	{"38:6B:1C:27:7C:AA", 54},
	{"38:6B:1C:27:7C:05", 43},
	{"38:6B:1C:27:7C:AC", 40},
	{"38:6B:1C:27:7C:AC", -1}
	};

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
				v[j].pos = i;	
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

	for (i=0; i<Nc; i++) {
		memcpy(mac, ERs[i], MAC_LEN);
		mac[MAC_LEN] = '\0';
		printf("%s\n", mac);
	};

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









int GPSs_ERs_load(void)
{
	int i, j, n;
	char mac[LINE_LEN];
	char l[LINE_LEN];
	char *p;
	FILE *f;
	ssize_t r;

	/* falta inicializar las estructuras de datos en -1 */
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

void main(void)
{
	int i;


	GPSs_ERs_load();
	print_datos();
	ERs_load();
	for (i=0; i<Nc; i++)
		qsort (RFINGs[i], Nc, sizeof(fing_st), compare);
	print_datos();
	
	v_ordenar(RFINGs[2], vr);
	v_ordenar(tfing, vt);

	for (i=0; i<Nc; i++) {
		printf("vt %i id:%i pos:%i\n", i, vt[i].id, vt[i].pos); 
	}

	for (i=0; i<Nc; i++) {
		printf("vr %i id:%i pos:%i\n", i, vr[i].id, vr[i].pos); 
	}

	float p = p_calc(vt, vr);
	printf("c: %f\n", p);
	printf("d=1-p = %f\n", (float)1-p);

}
