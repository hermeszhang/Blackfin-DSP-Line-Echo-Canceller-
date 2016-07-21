#include <stdio.h>
#include <stdlib.h>
#include <fract.h>
//#include "proc_c.h"
#include <cycles.h>

#define NSAMPLES       161



FILE *file ,*coeficientes, *file_out;

  
extern short proc_FIR(short*, short*, short);
extern void  proc_LEC(short* ,short*,short,short);
//extern void proc_SHIFT(short*, short, short);

int main()
{
    short rd, cf, k;
    int i, aux;
	short aux_file_read, aux_coef_read,saida, aux_erro, aux_mult ;
	
	short passo = 3300;
	short W[NSAMPLES] = {0};
	short Y[NSAMPLES] ;
	
	
	cycle_stats_t stats;
  	
  	CYCLES_INIT(stats);
/////////////////////////////Nome do Arquivo de Entrada///////////////////////////////////////////////////
    file = fopen("..\\FAR_DIGITAL.PCM","rb");
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	coeficientes = fopen("..\\NEAR_DIGITAL.PCM", "rb");
    
	if(file == NULL){
        printf("\n\nErro ao abrir arquivo de Amostras\n\n");
        exit(1);
    }
    if(coeficientes == NULL){
          printf("\n\nErro ao abrir arquivo de Coeficientes\n\n");
          exit(1);
    }

    file_out= fopen("saida_LEC_ASM.pcm", "wb");
    
    
    i=0;
    while(!feof(file)&& fread(&aux_file_read,sizeof(short),1,file)){
      	CYCLES_START(stats);
		
      	Y[0] = aux_file_read;
      	fread(&aux_coef_read,sizeof(short),1,coeficientes);
    
    	
    	saida =  proc_FIR(Y, W, NSAMPLES);
        
        aux_erro =  aux_coef_read-saida;
        aux_mult = high_of_fr2x16(mult_fr1x32(aux_erro,passo));
        
        proc_LEC(Y,W,NSAMPLES,aux_mult);
    	
        //proc_SHIFT(Y, NSAMPLES, ((NSAMPLES*2)-2));
        for(k=161;k>=1;k--){
        	Y[k] = Y[k-1];
    	}
       
    	fwrite(&saida,sizeof(short),1,file_out);
        
    	CYCLES_STOP(stats);
    }
    CYCLES_PRINT(stats);
    printf("\nSinal Filtrado\n");
    fclose(file);
    fclose(file_out);

}
