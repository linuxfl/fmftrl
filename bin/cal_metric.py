#!/usr/bin/env python 
import sys
import math
import operator
from math import log

def help():
	print "python predict_out_file"
	print "format: predict_score label(0|1) +"
	print "qauc is not implement"
	sys.exit(1)

if len(sys.argv) < 2:
	print help

predict_list = []
predict_file = open(sys.argv[1], 'r')

seg_size = 5000
q_dist = [0] * (seg_size + 1)
qq_plot = [[0 for x in range(2)] for y in range(seg_size + 1)]

total = 0
pos_num = 0
neg_num = 0
mae = 0.0
mse = 0.0

p_click = 0.0
t_click = 0.0
print "Begin to load data and sort to cal AUC and MSE LOGLOSS"

for line in predict_file:
	fleds = line.strip().split(" ")
	label = int(fleds[1])
	pre_score = float(fleds[0])
	p_click = p_click + pre_score
	slot = int(pre_score * seg_size)
	q_dist[slot] = q_dist[slot] + 1
	qq_plot[slot][0] = qq_plot[slot][0] + 1
	if label != 1:
		label = 0
		neg_num = neg_num + 1
		mae = mae + pre_score
		mse = mse + pre_score * pre_score
	else:
		pos_num = pos_num + 1
		t_click = t_click + 1
		mae = mae + 1 - pre_score
		mse = mse + (1 - pre_score) * (1 - pre_score)
		qq_plot[slot][1] = qq_plot[slot][1] + 1
	predict_list.append((pre_score, label))
predict_file.close()

total = neg_num + pos_num
pos_rate = (0.0 + pos_num) / total
mae = mae / total
mse = mse /total
copc = t_click / p_click

print("Valid data's pos_rate is %f" % pos_rate)
print("COPC is %f" % copc)
print("End load data, %d pos %d neg" % (pos_num, neg_num))
print("Valid data's pos_rate is %f" % pos_rate)
print("Valid data's MAE is %f, MSE is %f" % (mae, mse))
print "write q_dist to q_dist.csv"

q_file = open("q_dist.csv", 'w')

q_file.write(','.join(map(str, range(0, seg_size + 1))))
q_file.write('\n')
q_file.write(','.join(map(str, q_dist)))
q_file.close()

qq_list = [0.0] * (seg_size + 1)
q_show = [0] * (seg_size + 1)
q_click = [0] * (seg_size + 1)
for index in range(0, seg_size + 1):
	q_show[index] = qq_plot[index][0]
	q_click[index] = qq_plot[index][1]
	if qq_plot[index][0] != 0:
		qq_list[index] = qq_plot[index][1] / (qq_plot[index][0] + 0.0)

print "write q_dist to qq_plot.csv"""
qq_file = open("qq_plot.csv", 'w')

qq_file.write(','.join(map(str, range(0, seg_size + 1))))
qq_file.write('\n')
qq_file.write(','.join(map(str, qq_list)))
qq_file.write('\n')
qq_file.write(','.join(map(str, q_show)))
qq_file.write('\n')
qq_file.write(','.join(map(str, q_click )))
qq_file.close()

print "Begin to cal AUC.."

predict_list.sort(key=operator.itemgetter(0), reverse=True)
index = 0
total_score = 0
for item in predict_list:
	if item[1] == 1:
		total_score = total_score + total - index
	index = index + 1
total_score = total_score - pos_num * (pos_num + 1.0) / 2.0
auc = total_score / (pos_num * neg_num)

print("AUC is %f" % auc)

print "Begin to cal Logloss.."

logloss = 0

for item in predict_list:
	if item[1] == 1:
		logloss = logloss + math.log(item[0])
	else:		
		logloss = logloss + math.log(1 - item[0])
logloss = -logloss / total

print("Logloss is %f" % logloss)

backgroundLoss = pos_rate*log(pos_rate) + (1-pos_rate)*log(1-pos_rate)

NE = -logloss/backgroundLoss

print("NE is %f" % NE)

