#ifndef READER_H
#define READER_H
#include "Setting.h"
#include "Tuple.h"
#include <cstdlib>
#include <unistd.h>
#include <algorithm>
//#include <iostream> //ctype 不支持 
#include <cmath>
#include <typeinfo.h >
Tuple *trainList; // 保存所有的正例 

void set_area_data_shift(){
	area_data_shift = 24*7*2;
//	x_dim = 23 + 6 +2;
	area_data_vaild = area_data_sum - area_data_shift;
	shift_list = (INT *)calloc(tupleTotal, sizeof(INT));
	for(INT i=0 ; i<23; i++){
		shift_list[i] = i+1;
	}
	for(INT i=0 ; i<6; i++){
		shift_list[23+i] = 24*i+24;
	}
	for(INT i=0; i<2; i++){
		shift_list[23+6+i] = 24*7*i+24*7;
	}
	trainTotal =  areaTotal * area_data_vaild;
	area_hour_sum = area_data_sum/24;
	area_hour_vailf = area_data_vaild/24;
	trainHourTotal = areaTotal*area_hour_vailf;

}

extern "C"
void importTrainFiles() {
	printf("the path is %s\n",inPath);
	printf("The toolkit is importing datasets.\n");
	
	FILE *fin;
	INT tmp1,tmp2,tmp3;
	fin = fopen(inPath, "r");
	
	tupleTotal = 0;
	while(fscanf(fin, "%lld,%lld,%llfd", &tmp1,&tmp2,&tmp3)!=EOF){
		tupleTotal++;
	}
	fclose(fin);
	printf("tuple Total:%lld\n",tupleTotal);
	fin = fopen(inPath, "r");
	trainList = (Tuple *)calloc(tupleTotal, sizeof(Tuple));
	areaTotal = 0;
	for(INT i=0; i<tupleTotal ;i++){
		fscanf(fin, "%lld,%lld,%f", &trainList[i].id,&trainList[i].date_hour,&trainList[i].index);
		if(trainList[i].id > areaTotal ){
			areaTotal = trainList[i].id;
		}
	}
	fclose(fin);
	std::sort(trainList, trainList + tupleTotal, Tuple::cmp);
	area_data_sum=1;
	for(INT i=1; i<tupleTotal; i++){
		if(trainList[i].id !=trainList[i-1].id) break;
		area_data_sum++;
	}
	set_area_data_shift();

	printf("areaTotal:%lld\n",areaTotal);
	printf("trainTotal:%lld\n",trainTotal);
	printf("area_data_shift:%lld\n",area_data_shift);
	printf("area_data_vaild:%lld\n",area_data_vaild);
	printf("area_data_sum:%lld\n",area_data_sum);
	printf("trainHourTotal:%lld\n",trainHourTotal);
}





#endif
