# Wiring:
# straight_topleft:
# 1 2 3
# 4 5 6
# 7 8 9
# zigzag_topleft:
# 1 2 3
# 6 5 4
# 7 8 9
# straight_topright:
# 3 2 1
# 6 5 4
# 9 8 7
# zigzag_topright:
# 3 2 1
# 4 5 6
# 9 8 7
#
# TODO:
# - Being able to define starting corner and if wiring of lines is straight
#   or zigzag. Expose this option in the object initialization.


from itertools import chain


def listto2d(l, n):
    """Takes a list and returns a 2 dimensional list.
    l: list
    n: size of the inside list
    """
    return [l[i:i+n] for i in range(0, len(l), n)]


def reverselistin2dlist_even(matrix):
    """Reverse a list inside a list if it's of even index.
    ex: Reverse a[0], a[2], a[4]...
    """
    for i in range(len(matrix)):
        # Bitwise and to check odd/even
        if i and 1:
            matrix[i] = list(reversed(matrix[i]))


def reverselistin2dlist_odd(matrix):
    """Reverse a list inside a list if it's of odd index.
    ex: Reverse a[1], a[3], a[5]...
    """
    for i in range(len(matrix)):
        # Bitwise and to check odd/even
        if not i and 1:
            matrix[i] = list(reversed(matrix[i]))


def striptomatrix_zigzag_topright(strip, x):
    return reverselistin2dlist_even(listto2d(strip, x))


def matrixtostrip_zigzag_topright(matrix):
    a = matrix.copy()
    reverselistin2dlist_even(a)
    return list(chain.from_iterable(a))


def striptomatrix_straight_topright(strip, x):
    return listto2d(strip, x)


def matrixtostrip_straight_topright(matrix):
    a = matrix.copy()
    return list(chain.from_iterable(a))
