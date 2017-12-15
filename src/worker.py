import tensorflow as tf
from network import AC_Network
import sunfish
import numpy as np
import util
import chess

class Agent():
    def __init__(self, s_size, numberOfActions, trainer, name):
        self.game_rewards = []
        self.game_lengths = []
        self.game_mean_values = []

        self.game_buffer = []
        self.game_values = []
        self.game_reward = 0
        self.game_step_count = 0

        #Create the local copy of the network and the tensorflow op to copy global parameters to local network
        self.local_AC = AC_Network(s_size,numberOfActions,name,trainer)
        self.update_local_ops = util.update_target_graph('global',name)

        self.actions = self.actions = np.identity(numberOfActions,dtype=bool).tolist()

    def train(self,rollout,sess,gamma,bootstrap_value):
        network = self.local_AC
        rollout = np.array(rollout)
        observations = rollout[:,0]
        actions = rollout[:,1]
        rewards = rollout[:,2]
        next_observations = rollout[:,3]
        values = rollout[:,5]

        print("REWARDS:", rewards)

        # Here we take the rewards and values from the rollout, and use them to
        # generate the advantage and discounted returns.
        # The advantage function uses "Generalized Advantage Estimation"
        self.rewards_plus = np.asarray(rewards.tolist() + [bootstrap_value])
        discounted_rewards = util.discount(self.rewards_plus,gamma)[:-1]
        self.value_plus = np.asarray(values.tolist() + [bootstrap_value])
        advantages = rewards + gamma * self.value_plus[1:] - self.value_plus[:-1]
        advantages = util.discount(advantages,gamma)

        # Update the global network using gradients from loss
        # Generate network statistics to periodically save
        feed_dict = {network.target_v:discounted_rewards,
                     network.inputs:np.vstack(observations),
                     network.actions:actions,
                     network.advantages:advantages,
                     network.state_in[0]:self.batch_rnn_state[0],
                     network.state_in[1]:self.batch_rnn_state[1]}
        v_l,p_l,e_l,g_n,v_n, self.batch_rnn_state,_ = sess.run([network.value_loss,
                                                                network.policy_loss,
                                                                network.entropy,
                                                                network.grad_norms,
                                                                network.var_norms,
                                                                network.state_out,
                                                                network.apply_grads],
                                                               feed_dict=feed_dict)
        return v_l / len(rollout),p_l / len(rollout),e_l / len(rollout), g_n,v_n

    def game_start(self, sess):
        self.rnn_state = self.local_AC.state_init
        self.batch_rnn_state = self.rnn_state
        sess.run(self.update_local_ops)

    def iterate(self, sess, state, board, gamma):
        network = self.local_AC
        #Take an action using probabilities from policy network output.
        a_dist,v,rnn_state = sess.run([network.policy, network.value, network.state_out],
                                      feed_dict={network.inputs:[state],
                                                 network.state_in[0]:self.rnn_state[0],
                                                 network.state_in[1]:self.rnn_state[1]})
        #fromDist = a_dist[0][:64]
        #toDist = a_dist[0][64:]
        a = np.random.choice(a_dist[0],p=a_dist[0])
        a = np.argmax(a_dist == a)
        #a2 = np.random.choice(toDist,p=toDist)
        #a = np.argmax(fromDist == a)
        #a2 = np.argmax(toDist == a2)

        state, r = util.make_action(board, self.actions[a])
        print("Reward:", r)
        d = util.is_finished(board)
        if d == False:
            s1 = util.process_position(board)
        else:
            s1 = state

        self.game_buffer.append([s1,a,r,s1,d,v[0,0]])
        self.game_values.append(v[0,0])

        self.game_reward += r
        s = s1
        self.game_step_count += 1

        # If the episode hasn't ended, but the experience buffer is full, then we
        # make an update step using that experience rollout.
        if len(self.game_buffer) == 30 and d != True:
            # Since we don't know what the true final return is, we "bootstrap" from our current
            # value estimation.
            v1 = sess.run(network.value,
                          feed_dict={network.inputs:[s],
                                     network.state_in[0]:rnn_state[0],
                                     network.state_in[1]:rnn_state[1]})[0,0]
            v_l,p_l,e_l,g_n,v_n = self.train(self.game_buffer,sess,gamma,v1)
            game_buffer = []
            sess.run(self.update_local_ops)
        if r <= 0:
            return False
        return True

    def game_ended(self, saver, sess, game_count, gamma):
        self.game_rewards.append(self.game_reward)
        self.game_lengths.append(self.game_step_count)
        self.game_mean_values.append(np.mean(self.game_values))

        # Update the network using the episode buffer at the end of the episode.
        if len(self.game_buffer) != 0:
            v_l,p_l,e_l,g_n,v_n = self.train(self.game_buffer,sess,gamma,0.0)


        # Periodically save model parameters, and summary statistics.
        if game_count % 5 == 0 and game_count != 0:
            self.summarise(saver, sess, game_count, v_l, p_l, e_l, g_n, v_n)
        if self.name == 'worker_0':
            sess.run(self.increment)
        game_count += 1


    def summarise(self, saver, sess, game_count, v_l, p_l, e_l, g_n, v_n):
            if game_count % 250 == 0 and self.name == 'worker_0':
                saver.save(sess,self.model_path+'/model-'+str(game_count)+'.cptk')
                print ("Saved Model")

            mean_reward = np.mean(self.game_rewards[-5:])
            mean_length = np.mean(self.game_lengths[-5:])
            mean_value = np.mean(self.game_mean_values[-5:])
            summary = tf.Summary()
            summary.value.add(tag='Perf/Reward', simple_value=float(mean_reward))
            summary.value.add(tag='Perf/Length', simple_value=float(mean_length))
            summary.value.add(tag='Perf/Value', simple_value=float(mean_value))
            summary.value.add(tag='Losses/Value Loss', simple_value=float(v_l))
            summary.value.add(tag='Losses/Policy Loss', simple_value=float(p_l))
            summary.value.add(tag='Losses/Entropy', simple_value=float(e_l))
            summary.value.add(tag='Losses/Grad Norm', simple_value=float(g_n))
            summary.value.add(tag='Losses/Var Norm', simple_value=float(v_n))
            self.summary_writer.add_summary(summary, game_count)

            self.summary_writer.flush()

class Worker():
    def __init__(self, name, s_size, numberOfActions, trainer, model_path, global_games):
        self.name = "worker_" + str(name)
        self.number = name
        self.model_path = model_path
        self.trainer = trainer
        self.global_games = global_games
        self.increment = self.global_games.assign_add(1)
        self.summary_writer = tf.summary.FileWriter("train_"+str(self.number))
        self.agents = [Agent(s_size, numberOfActions, trainer, self.name + "0"), Agent(s_size, numberOfActions, trainer, self.name + "1")]

        #self.env = sunfish.Position(sunfish.initial, 0, (True,True), (True,True), 0, 0)
        self.env = chess.Board()



    def work(self,max_episode_length,gamma,sess,coord,saver):
        self.game_count = sess.run(self.global_games)
        v_l = p_l = e_l = g_n = v_n = -1
        print ("Starting worker " + str(self.number))
        with sess.as_default(), sess.graph.as_default():
            while not coord.should_stop():
                d = False

                state = util.process_position(self.env)
                for agent in self.agents:
                    agent.game_start(sess)
                x = 0
                while util.is_finished(self.env) == False:
                    agent = self.agents[x % len(self.agents)]
                    madeLegalMove = agent.iterate(sess, state, self.env, gamma)
                    if madeLegalMove:
                        x += 1
                    legalMovesCount = 0;
                    for move in self.env.legal_moves:
                        legalMovesCount += 1
                    print("Move", x, "Legal moves available:", legalMovesCount)
                    print(self.env)
                    print("Made legal move:", madeLegalMove, "Game finished", util.is_finished(self.env))

                print("Game finished")
                self.game_count += 1
                for agent in self.agents:
                    agent.game_ended(saver, sess, self.game_counte, gamma)
                print("Game ended")
                exit(1)
