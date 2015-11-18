import time
import random
import string
import lcs

if __name__ == "__main__":
    a = ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(100))
    b = ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(100))
    start = time.process_time()
    lcs.lcs(a,b)
    end = time.process_time()
    delta = end - start
    print("Time when using vector: {:.4f} ms".format(delta * 1000))

    start = time.process_time()
    lcs.lcs_map(a,b)
    end = time.process_time()
    delta = end - start
    print("Time with using map and string: {:.4f} ms".format(delta * 1000))