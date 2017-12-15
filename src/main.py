
import tensorflow as tf
import numpy as np

import os
import multiprocessing
import threading
from time import sleep
from worker import Worker
from network import AC_Network

max_episode_length = 300
gamma = .99 # discount rate for advantage estimation and reward discounting
s_size = 64 * 6 * 2 # Observations are chess game, 64 squares, 6 characters, 2 colours
a_size = 64 * 64 # Agent can move a piece in any location to another location
load_model = False
model_path = './model'
tf.reset_default_graph()

if not os.path.exists(model_path):
    os.makedirs(model_path)

workers = []

with tf.device("/cpu:0"):
    global_games = tf.Variable(0,dtype=tf.int32,name='global_games',trainable=False)
    trainer = tf.train.AdamOptimizer(learning_rate=1e-4)
    master_network = AC_Network(s_size,a_size,'global',None) # Generate global network
    num_workers = multiprocessing.cpu_count() # Set workers to number of available CPU threads
    print("Workers:", num_workers)
    # Create worker classes
    for i in range(num_workers):
        workers.append(Worker(i,s_size,a_size,trainer,model_path,global_games))
    saver = tf.train.Saver(max_to_keep=5)

with tf.Session() as sess:
    coord = tf.train.Coordinator()
if load_model == True:
    print ('Loading Model...')
    ckpt = tf.train.get_checkpoint_state(model_path)
    saver.restore(sess,ckpt.model_checkpoint_path)
else:
    sess.run(tf.global_variables_initializer())

# This is where the asynchronous magic happens.
# Start the "work" process for each worker in a separate threat.
worker_threads = []
for worker in workers:
    worker_work = lambda: worker.work(max_episode_length,gamma,sess,coord,saver)
    t = threading.Thread(target=(worker_work))
    t.start()
    sleep(0.5)
    worker_threads.append(t)
coord.join(worker_threads)

