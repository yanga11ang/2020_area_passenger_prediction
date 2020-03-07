
from config import Trainer, Tester
from module.model import Linear
from module.loss import RMSELoss
from module.strategy import NoStrategy
from data import TrainDataLoader, TestDataLoader


# dataloader for training
train_dataloader = TrainDataLoader(
	in_path = "../data/database/area_passenger_index.csv",
	x_dim = 31,
	hour = 0,
	batch_size = 126,
	threads = 8
)

# dataloader for test
# test_dataloader = TestDataLoader("./benchmarks/WN18RR/", "link")

# define the model
linear = Linear(
	x_dim = train_dataloader.get_x_dim(),
	hour = train_dataloader.get_hour()
)


# define the loss function
model = NoStrategy(
	model = linear,
	loss = RMSELoss(),
	batch_size = train_dataloader.get_batch_size(),
	regul_rate = 0.0
)

# train the model
trainer = Trainer(model = model, data_loader = train_dataloader, train_times = 1, alpha = 2e-5, use_gpu = False, opt_method = "adam")
trainer.run()
linear.save_checkpoint('./chekpoint/linear_first_try.ckpt')

print("up to cheeck point 4 is ok\n")
exit(0)
# test the model
# transe.load_checkpoint('./checkpoint/transe_2.ckpt')
# tester = Tester(model = transe, data_loader = test_dataloader, use_gpu = True)
# tester.run_link_prediction(type_constrain = False)
