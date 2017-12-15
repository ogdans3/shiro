import numpy as np
import tensorflow as tf
import scipy.signal
import sunfish
import chess

# Copies one set of variables to another.
# Used to set worker network parameters to those of global network.
def update_target_graph(from_scope,to_scope):
    from_vars = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, from_scope)
    to_vars = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, to_scope)

    op_holder = []
    for from_var,to_var in zip(from_vars,to_vars):
        op_holder.append(to_var.assign(from_var))
    return op_holder

# Discounting function used to calculate discounted returns.
def discount(x, gamma):
    return scipy.signal.lfilter([1], [1, -gamma], x[::-1], axis=0)[::-1]

#Used to initialize weights for policy and value output layers
def normalized_columns_initializer(std=1.0):
    def _initializer(shape, dtype=None, partition_info=None):
        out = np.random.randn(*shape).astype(np.float32)
        out *= std / np.sqrt(np.square(out).sum(axis=0, keepdims=True))
        return tf.constant(out)
    return _initializer

def getFirstPositionInBitMap(bits):
    bits = np.array(bits)
    arr = np.where(bits==1)[0]
    if len(arr) == 0:
        return -1
    index = int(arr[0])
    row = int(index / 8)
    col = int(index - row * 8)
    return row, col

def process_position(pos):
    pos = str(pos)
    whitePawnBitmap = np.zeros(64)
    whiteBishopBitmap = np.zeros(64)
    whiteRookBitmap = np.zeros(64)
    whiteKnightBitmap = np.zeros(64)
    whiteQueenBitmap = np.zeros(64)
    whiteKingBitmap = np.zeros(64)
    blackPawnBitmap = np.zeros(64)
    blackBishopBitmap = np.zeros(64)
    blackRookBitmap = np.zeros(64)
    blackKnightBitmap = np.zeros(64)
    blackQueenBitmap = np.zeros(64)
    blackKingBitmap = np.zeros(64)
    offset = 0
    for index, piece in enumerate(pos): #enumerate(pos.board[20:-20]):
        if piece == ' ':
            offset += 1
            continue
        index = index - offset
        index = index - int(index / 9)
        if(piece == 'p'):
            whitePawnBitmap[index] = 1
        if(piece == 'b'):
            whiteBishopBitmap[index] = 1
        if(piece == 'n'):
            whiteKnightBitmap[index] = 1
        if(piece == 'r'):
            whiteRookBitmap[index] = 1
        if(piece == 'q'):
            whiteQueenBitmap[index] = 1
        if(piece == 'k'):
            whiteKingBitmap[index] = 1

        if(piece == 'P'):
            blackPawnBitmap[index] = 1
        if(piece == 'B'):
            blackBishopBitmap[index] = 1
        if(piece == 'N'):
            blackKnightBitmap[index] = 1
        if(piece == 'R'):
            blackRookBitmap[index] = 1
        if(piece == 'Q'):
            blackQueenBitmap[index] = 1
        if(piece == 'K'):
            blackKingBitmap[index] = 1
    whitePiecesBitMap = np.concatenate((whitePawnBitmap, whiteBishopBitmap, whiteRookBitmap, whiteKnightBitmap, whiteQueenBitmap, whiteKingBitmap))
    blackPiecesBitMap = np.concatenate((blackPawnBitmap, blackBishopBitmap, blackRookBitmap, blackKnightBitmap, blackQueenBitmap, blackKingBitmap))
    return np.concatenate((whitePiecesBitMap, blackPiecesBitMap))

def make_action(pos, move):
    move = np.array(move)
    fromPosition = int(np.where(move==1)[0] / 64)
    toPosition = int(np.where(move==1)[0] % 64)

    zeros = np.zeros(64)
    zeros[fromPosition] = 1
    fromRow, fromCol = getFirstPositionInBitMap(zeros)

    zeros = np.zeros(64);
    zeros[toPosition] = 1
    toRow, toCol = getFirstPositionInBitMap(zeros)
    move = move_from_number(fromRow, fromCol, toRow, toCol)
    print("Move", move)
    move = chess.Move.from_uci(move)

    moves = pos.legal_moves
    reward = 0
    if move in moves:
        #We made legal move, award a small amount of points
        reward = 0.001
        pos.push(move)
        print("LEGAL MOVE")
    else:
        #We made an illegal move
        reward = -0.001

    #Draw, reward is 0.5 points
    if pos.is_stalemate():
        reward = 0.5
    if pos.is_insufficient_material():
        reward = 0.5
    if pos.is_fivefold_repetition():
        reward = 0.5
    if pos.is_seventyfive_moves():
        reward = 0.5
    #We won, reward is 1 point
    if pos.is_checkmate():
        reward = 1

    return pos, reward

def is_finished(pos):
    return pos.is_game_over()

def move_from_number(fromRow, fromCol, toRow, toCol):
    offset = ord('a')
    fromRow = chr(offset + fromRow)
    fromPos = fromRow + str(fromCol + 1)
    toRow = chr(offset + toRow)
    toPos = toRow + str(toCol + 1)
    return fromPos + toPos

#import sunfish
#pos = sunfish.Position(sunfish.initial, 0, (True,True), (True,True), 0, 0)
#print(process_position(pos))

#move = np.zeros(64 + 64);
#move[62] = 1
#move[64+62-8-8+1] = 1
#pos = sunfish.Position(sunfish.initial, 0, (True,True), (True,True), 0, 0)
#make_action(pos, move)

#move = np.zeros(64 + 64);
#move[64-8-7] = 1
#move[64+64-8-7-8-8] = 1
#pos = sunfish.Position(sunfish.initial, 0, (True,True), (True,True), 0, 0)
#make_action(pos, move)
#print(chess.Board())
#for move in chess.Board().legal_moves:
#    print(move)
#move = move_from_number(1,2,1,4)
#print(chess.Move.from_uci(move))

