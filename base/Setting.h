#ifndef SETTING_H
#define SETTING_H
#define INT long long
#define REAL float
#include <string.h>
#include <stdio.h>
//#include <string>
char inPath[1000] = "../data/FB15K/";
char outPath[1000] = "../data/FB15K/";
INT area_data_shift; //每个区域前几天的数据是不能使用的 
INT area_data_vaild; //每个区域有效数据条数 
INT area_data_sum; 	// 每个区域共有多少条记录。
INT area_hour_sum; //每个区域按小时有多少条记录
INT area_hour_vailf;//每个区域按小时有多少记录
INT areaTotal = 0;
INT tupleTotal = 0;
INT testTotal = 0;
INT trainTotal = 0;
INT trainHourTotal = 0; 
INT validTotal = 0;
const INT x_dim=23 + 6 +2;			// x的维度 
INT* shift_list;


extern "C"
void setInPath(char *path) {
	INT len = strlen(path);
	inPath[0] = '\0';
	strcat(inPath, path);
	printf("Input Files Path : %s\n", inPath);
}

extern "C"
void setOutPath(char *path) {
	INT len = strlen(path);
	outPath[0] = '\0';
	strcat(outPath, path);
	printf("Output Files Path : %s\n", outPath);
}

/*
============================================================
*/

INT workThreads = 1;

extern "C"
void setWorkThreads(INT threads) {
	workThreads = threads;
}

extern "C"
INT getWorkThreads() {
	return workThreads;
}

/*
============================================================
*/




extern "C"
INT getareaTotal() {
	return areaTotal;
}


extern "C"
INT getTupleTotal() {
	return tupleTotal;
}

extern "C"
INT getTrainTotal() {
	return trainTotal;
}

extern "C"
INT getTrainHourTotal() {
	return trainHourTotal;
}

extern "C"
INT getTestTotal() {
	return testTotal;
}



extern "C"
INT getValidTotal() {
	return validTotal;
}


#endif
