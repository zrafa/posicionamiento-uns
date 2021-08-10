
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
# define MAC_LEN	17	/* 17 letras tiene una MAC de ER */
# define LINE_LEN	80	/* 17 letras tiene una MAC de ER */
# define Nc	9	/* 30 ERs maximo en la matriz */
typedef struct v_st {
	int ID;
	int pos;
} v_st;

// v_st Vt[Nc];
// v_st Vr[Nc];

int tf_ex[5][2] = { {100, 62}, {110, 60}, {2, 54}, {5, 43}, {99, 40} };

v_st vt[] = { {2,3}, {5,4}, {10,9}, {99,5}, {100,1}, {110,2}, {111,9}, {200,9}, {201,9}};
v_st vr[] = { {2,9}, {5,2}, {10,5}, {99,9}, {100,1}, {110,3}, {111,4}, {200,6}, {201,7}};


char ERs[Nc][MAC_LEN];

int ERs_load(void)
{
	int f, i, n;
	char mac[MAC_LEN+1];

	f = open(ERs_FILE, O_RDONLY);
	i = 0;
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

int GPS_ERs_load(void)
{
	int i, n;
	char mac[LINE_LEN];
	char l[LINE_LEN];
	char *p;
	FILE *f;
	size_t n = LINE_LEN;

	f = fopen(ERs_FILE, "r");
	i = 0;
	//while (read(f, mac, MAC_LEN+1)) {
	while ((read = getline(l, %len, f)) != -1) {
		if (strchr(l, ',') == NULL) {		/* si es una MAC */
			memcpy(ERs[i], l, MAC_LEN);
			p = strchr(l, ' ');
			sprintf(
		} else
			memcpy(ERs[i], l, MAC_LEN);
	 printf("Retrieved line of length %zu:\n", read);
	 printf("%s", line);
	}
	while (getline(f, mac, MAC_LEN+1)) {
		memcpy(ERs[i], mac, MAC_LEN);
		// printf("%s\n", ERs[i]);
		i++;
	}
	n = i;
	fclose(f);

	for (i=0; i<Nc; i++) {
		memcpy(mac, ERs[i], MAC_LEN);
		mac[MAC_LEN] = '\0';
		printf("%s\n", mac);
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
	ERs_load();
	
	float p = p_calc(vt, vr);
	printf("c: %f\n", p);
	printf("d=1-p = %f\n", (float)1-p);

}
