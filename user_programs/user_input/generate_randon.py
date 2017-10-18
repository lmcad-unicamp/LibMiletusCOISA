#!/usr/bin/python

import random
import sys
import math

size = int(sys.argv[1]) if (len(sys.argv) > 1) else 10

# random.seed(1)
# result =  "["
# for i in xrange(0, size):
# 	result += str(random.uniform(0, 100)) + ", "
# result = result[:-2]
# result += "]"
# print result

import numpy
numpy.random.seed(size)


result = ""
resultvec="["
for i in xrange(0, size):
	value = "{0:.2f}".format(math.sin((i+1)/4.0) * numpy.random.normal(30,15))
	# value = "%0.2f" % float(value)
	result += value + " "
	resultvec += value + ", "
result = result[:-2]
resultvec = resultvec[:-3]
resultvec += "]"

# print result
# print "\n------------------------------\n"
# print resultvec

result_file_object  = open(str(size)+".in", "w")
result_file_object.write(result)
result_file_object.write("\n")
result_file_object.close()
result_vec_file_object  = open(str(size)+".vec", "w")
result_vec_file_object.write(resultvec)
result_vec_file_object.write("\n")
result_vec_file_object.close()


'''
result_file_object  = open(str(size)+".in", "w")
for i in xrange(0, size):
	value = "{0:.2f}".format(math.sin((i+1)/4.0) * numpy.random.normal(30,15))
	# value = "%0.2f" % float(value)
	result_file_object.write(value + " ")

# print result
# print "\n------------------------------\n"
# print resultvec

result_file_object.write("\n")
result_file_object.close()

'''