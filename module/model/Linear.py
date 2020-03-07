import torch
import torch.nn as nn
import torch.nn.functional as F
from .Model import Model


# 父类 是nn.Module 因为用到了forward 的特殊用法
class Linear(Model):

    def __init__(self, x_dim, hour, area = 0, fix_area = False ):
        super(Linear, self).__init__(x_dim, hour)
        self.hour = hour  # embedding dim
        self.area = area  #
        self.hour =hour
        self.fix_area = fix_area
        self.linear = nn.Linear(x_dim, 1)

    def forward(self, data):
        x = data["batch_x"]
        out = self.linear(x)
        return out