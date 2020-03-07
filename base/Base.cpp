#include <windows.h>
#include "Setting.h"
#include "Random.h"
#include "Reader.h"
#include <cstdlib>
#include <process.h>


extern "C"
void setInPath(char *path);

extern "C"
void setOutPath(char *path);

extern "C"
void setWorkThreads(INT threads);


extern "C"
INT getWorkThreads();

extern "C"
INT getEntityTotal();

extern "C"
INT getRelationTotal();

extern "C"
INT getTupleTotal();

extern "C"
INT getTrainTotal();

extern "C"
INT getTestTotal();

extern "C"
INT getValidTotal();

extern "C"
void randReset();

extern "C"
INT getTrainHourTotal();

extern "C"
void importTrainFiles();

struct Parameter {
	INT id;    		// 线程id 
	INT *batch_id; 	// 是哪个核心区域 
	REAL *batch_x;  // 一维代替二维数组指针，batch_size*dim_x 
	REAL *batch_y; 	// y值 batch_size 
	INT hour;
	INT batchSize; 	// 批大小 
	INT area_id; 	// 选用指定id的数据。
	INT test_flag; 	// 是否生成测试集 
};

// 
unsigned int __stdcall getBatch(void* con) {
	
	Parameter *para = (Parameter *)(con);
	INT id = para -> id;
	INT *batch_id = para -> batch_id;
	REAL *batch_x = para -> batch_x;
	REAL *batch_y = para -> batch_y;
	INT hour = para -> hour;
	INT batchSize = para -> batchSize;
	INT area_id = para -> area_id;
	INT test_flag = para -> test_flag;

	INT lef, rig;

	//分配每个线程的任务 
	if (batchSize % workThreads == 0) {
		lef = id * (batchSize / workThreads); //从 0 开始 
		rig = (id + 1) * (batchSize / workThreads);
	} else {
		lef = id * (batchSize / workThreads + 1); // 每各线程，多做一个，以防不足。  
		rig = (id + 1) * (batchSize / workThreads + 1);
		if (rig > batchSize) rig = batchSize;
	}
	for (INT batch = lef; batch < rig; batch++) { 
		INT fict_i = rand_max(id, trainHourTotal); // 随机得到一个 测试样例 的 编号[0,trainTotal) 
		INT res1 = area_data_shift+(fict_i%area_data_vaild)*24+hour; // 区域内偏移 
		INT res2 = area_data_sum*(fict_i/area_data_vaild); //区域偏移 
		INT true_i =  res1 + res2 ;
//		printf("true_i time: %lld\n", trainList[true_i].date_hour);
		for(INT i=0; i<x_dim; i++){
			if(true_i < shift_list[i]) continue;
			batch_x[batch*x_dim + i]=trainList[true_i-shift_list[i]].index;
		} 
		batch_y[batch] = trainList[true_i].index;  // 标明是正例 
		batch_id[batch] = trainList[true_i].id;
	}
	_endthreadex(0);
	return 0;
}


// 启用多线程 生成 一批数据 
extern "C"
void sampling(
		INT *batch_id, 
		REAL *batch_x, 
		REAL *batch_y, 
		INT hour,
		INT batchSize, 
		INT area_id,
		INT test_flag = false
) {
	HANDLE  *pt = (HANDLE  *)calloc(workThreads , sizeof(HANDLE ));
	Parameter *para = (Parameter *)calloc(workThreads , sizeof(Parameter));
	for (INT threads = 0; threads < workThreads; threads++) {
		para[threads].id = threads;
		para[threads].batch_id = batch_id;
		para[threads].batch_x = batch_x;
		para[threads].batch_y = batch_y;
		para[threads].hour = hour;
		para[threads].batchSize = batchSize;
		para[threads].area_id = area_id;
		para[threads].test_flag = test_flag;
		pt[threads] = (HANDLE)_beginthreadex(NULL,0,getBatch,(void*)(para+threads),0,NULL);
	}
	WaitForMultipleObjects(workThreads, pt, TRUE, INFINITE);
	free(pt);
	free(para);
}

int main() {
	// test sampling function 
//	setInPath("D:/py_workspace/machine_learning/contests/2020重点区域人群密度预测/algorithm/data/database/area_passenger_index.csv");
//	randReset();
//	setWorkThreads(2);
//	printf("%s\n",inPath);
//	importTrainFiles();
//	printf("finish importTrainFiles\n");
//	INT  id[20];
//	REAL y[20],x[20*x_dim];
//	sampling(id,x,y,1,10,0,0);
//	for(INT i=0; i<10 ;i++){
//		printf("%.1f:",y[i]);
//		for(INT j=0; j<31; j++){
//			printf("%.1f ",x[i*x_dim + j ]);
//		}
//		
//		printf("\n");
//	}
	
	
//	scanf("%d",&pos);

	// test importTestFiles function 
//	// needed envirement: setInPath、randReset、importTestFiles 
//	setInPath("D:/py_workspace/machine_learning/experiment/paper/OpenKE/benchmarks/FB15K237/");
//	randReset();
//	importTestFiles();
//	importTypeFiles();
	
	return 0;
}
