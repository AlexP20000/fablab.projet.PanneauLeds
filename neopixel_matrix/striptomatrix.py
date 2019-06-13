# TODO:
# - Being able to define starting corner and if wiring of lines is straight
#   or zigzag.
from itertools import chain

def striptomatrix_zigzag_topright(strip, x):
    a = listto2d(strip, x)
    reverselistin2dlist_even(a)
    return a

def matrixtostrip_zigzag_topright(matrix):
    a = matrix.copy()
    reverselistin2dlist_even(a)
    return list(chain.from_iterable(a))

def listto2d(l, n):
    """l: list, n: size of the inside list
    Takes a list and returns a 2 dimensional list."""
    return [l[i:i+n] for i in range(0, len(l), n)]

def reverselistin2dlist_even(matrix):
    """Reverse a list inside a list if it's of even index.
    ex: a[0], a[2], a[4]..."""
    for i in range(len(matrix)):
        if i % 2 == 0:
            matrix[i] = list(reversed(matrix[i]))
