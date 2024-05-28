import sys


def parse_args(n) -> list:

    argv = sys.argv

    if len(argv) - 1 < n:
        print("ERROR: too few params")
        exit(1)

    if len(argv) - 1 > n:
        print("ERROR: too many params")
        exit(1)

    return argv

