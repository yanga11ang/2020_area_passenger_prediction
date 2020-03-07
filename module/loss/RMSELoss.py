import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
import numpy as np
from .Loss import Loss


# 父类 是nn.Module 因为用到了forward 的特殊用法
class RMSELoss(Loss):

    def __init__(self ):
        super(RMSELoss, self).__init__()
        self.mse = nn.MSELoss()

    def forward(self, y, y_pre):  # p_score: positive score; n_score: negative score
        return torch.sqrt(self.mse(y_pre,y))

