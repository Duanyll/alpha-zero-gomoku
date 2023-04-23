# coding: utf-8
import sys
sys.path.append('..')
sys.path.append('../src')

import learner
import config

if __name__ == "__main__":
    if len(sys.argv) < 2 or sys.argv[1] not in ["train", "play", "test"]:
        print("[USAGE] python leaner_test.py train|play")
        exit(1)

    alpha_zero = learner.Leaner(config.config)

    if sys.argv[1] == "train":
        alpha_zero.learn()
    elif sys.argv[1] == "play":
        # Ask user who plays first
        human_first = input("Do you want to play first? (y/n): ")
        if human_first == "y":
            human_first = True
        else:
            human_first = False
        alpha_zero.play_with_human(human_first=human_first)
    elif sys.argv[1] == "test":
        from libzerogomoku import MCTS, Gomoku, NeuralNetwork
        cfg = config.config
        if len(sys.argv) < 3:
            print("[USAGE] python leaner_test.py test [num_contest]")
            exit(1)
        num_contest = int(sys.argv[2])
        libtorch_a = NeuralNetwork('./models/checkpoint.pt',
                                   cfg['libtorch_use_gpu'], cfg['num_mcts_threads'] * cfg['num_mcts_threads'] // 2)
        libtorch_b = NeuralNetwork('./models/best_checkpoint.pt',
                                   cfg['libtorch_use_gpu'], cfg['num_mcts_threads'] * cfg['num_mcts_threads'] // 2)

        one_won, two_won, draws = alpha_zero.contest(
            libtorch_a, libtorch_b, num_contest)
        print("A/B WINS : %d / %d ; DRAWS : %d" %
              (one_won, two_won, draws))
