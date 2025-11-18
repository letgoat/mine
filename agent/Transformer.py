from sre_parse import POSSESSIVE_REPEAT
import torch
import torch.nn as nn
import match
import math

# --- 占位符模块，---

class PositionalEncoding(nn.Module):
    """
    位置编码模块
    """
    def forward(self, x):
        pass

class MutiHeadAttention(nn.Module):
    """
    多头注意力机制模块
    """
    def forward(self, query, key, value, mask):
        pass

class PositionWiseFeedForward(nn.Module):
    """
    位置前馈网络模块
    """
    def forward(self, x):
        pass

# --- 编码器模块 ---

class EncoderLayer(nn.Module):
    def __init__(self, d_model, num_heads, d_ff, dropout):
        super(EncoderLayer, self).__init__()
        self.self_attn = MutiHeadAttention() #待实现
        self.feed_forward = PositionWiseFeedForward() #待实现
        self.norm1 = nn.LayerNorm(d_model)
        self.norm2 = nn.LayerNorm(d_model)
        self.dropout = nn.Dropout(dropout)
    
    def forward(self, x, mask):
        # 残差连接与层归一化
        # 1. 多头注意力
        attn_output = self.self_attn(x, x, x, mask)
        x = self.norm1(x + self.dropout(attn_output))

        # 2. 前馈网络
        ff_output = self.feed_forward(x)
        x = self.norm2(x + self.dropout(ff_output))

        return x

# --- 解码器模块 ---
