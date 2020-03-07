# coding:utf-8
import os
import ctypes  #python 和 C 混合调用的库
import numpy as np
import platform

# 每个epoch 是一个 TrainDataSampler ，nbatches次调用 datasampler，每次调用产生一个batch的数据
class TrainDataSampler(object):
	# 第二个参数返回 调用 产生一个batches 的函数
	def __init__(self, nbatches, datasampler):
		self.nbatches = nbatches
		self.datasampler = datasampler
		self.batch = 0

	def __iter__(self):
		return self

	def __next__(self):
		self.batch += 1 
		if self.batch > self.nbatches:
			raise StopIteration()
		return self.datasampler()

	def __len__(self):
		return self.nbatches

#用于加载 训练数据，低层实现是调用C++的DLL，上层主要实现，batch数据迭代
class TrainDataLoader(object):

	def __init__(self, in_path = "./",x_dim = 31,hour = 0 ,batch_size = None, nbatches = None, threads = 8):
		#连接 动态链接库
		if platform.system() == 'Windows':
			base_file = os.path.abspath(os.path.join(os.path.dirname(__file__), "../release/Base.dll"))
			self.lib = ctypes.cdll.LoadLibrary(base_file)
		elif platform.system() == 'Linux':
			base_file = os.path.abspath(os.path.join(os.path.dirname(__file__), "../release/Base.so"))
			self.lib = ctypes.cdll.LoadLibrary(base_file)
		self.x_dim = x_dim
		self.hour = hour
		"""argtypes"""
		self.lib.sampling.argtypes = [
			ctypes.c_void_p, #  batch_id
			ctypes.c_void_p, # batch_x
			ctypes.c_void_p, # batch_y
			ctypes.c_int64,  # hour
			ctypes.c_int64,  # batch_size
			ctypes.c_int64,  # area_id
			ctypes.c_int64   #test_flag
		]
		"""set essential parameters"""
		self.in_path = os.path.abspath(os.path.join(os.path.dirname(__file__), in_path))    # 训练数据存放位置
		print(self.in_path)
		self.work_threads = threads # C代码所用线程数目
		self.nbatches = nbatches # 每个epoch 有多少个batches ，因为 batches 是随机抽取，而不是枚举
		self.batch_size = batch_size # 每个batch的样例数
		self.read()

	def read(self):
		self.lib.setInPath(ctypes.create_string_buffer(self.in_path.encode(), len(self.in_path) * 2)) #设置路径
		self.lib.setWorkThreads(self.work_threads) # 设定 线程数目
		self.lib.randReset() # 设置随机种子
		self.lib.importTrainFiles() # 加载测试集
		self.areaTotal = self.lib.getareaTotal() # 获取 区域数
		self.trainHourTotal = self.lib.getTrainHourTotal() # 获取 训练集数

		if self.batch_size == None:
			self.batch_size = self.trainHourTotal // self.nbatches # 整数除法
		if self.nbatches == None:
			self.nbatches = self.trainHourTotal // self.batch_size
		print("trainHourTotal",self.trainHourTotal)
		print('nbatches',self.nbatches)
		print("batch_size",self.batch_size)
		print("areaTotal",self.areaTotal)
		self.batch_seq_size = self.batch_size  #batch 缓冲区大小

		self.batch_id = np.zeros(self.batch_seq_size, dtype=np.int64)
		self.batch_x = np.zeros(self.batch_seq_size * self.x_dim, dtype=np.float32)
		self.batch_y = np.zeros(self.batch_seq_size, dtype=np.float32)
		self.batch_id_addr = self.batch_id.__array_interface__["data"][0]
		self.batch_x_addr = self.batch_x.__array_interface__["data"][0]
		self.batch_y_addr = self.batch_y.__array_interface__["data"][0]

	def sampling(self):
		self.lib.sampling(
			self.batch_id_addr,
			self.batch_x_addr,
			self.batch_y_addr,
			self.hour,
			self.batch_size,
			0,
			0
		)
		return {
			"batch_id": self.batch_id,
			"batch_x": self.batch_x,
			"batch_y": self.batch_y,
		}


	"""interfaces to set essential parameters"""

	def set_work_threads(self, work_threads):
		self.work_threads = work_threads

	def set_in_path(self, in_path):
		self.in_path = in_path

	def set_nbatches(self, nbatches):
		self.nbatches = nbatches

	def set_batch_size(self, batch_size):
		self.batch_size = batch_size
		self.nbatches = self.trainHourTotal // self.batch_size

	def set_x_dim(self,x_dim):
		self.x_dim = x_dim

	def set_hour(self,hour):
		self.hour = hour


	"""interfaces to get essential parameters"""

	def get_batch_size(self):
		return self.batch_size

	def get_x_dim(self):
		return self.x_dim

	def get_hour(self):
		return self.hour

	def get_area_tot(self):
		return self.areaTotal

	def get_train_tot(self):
		return self.trainHourTotal

	def __iter__(self):
		print("in TrainDataLoader",self.nbatches)
		return TrainDataSampler(self.nbatches, self.sampling)

	def __len__(self):
		return self.nbatches