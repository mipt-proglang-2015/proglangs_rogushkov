import time
import random
import string
import lcs

def longest_common_substring(s1, s2):
    m = [[0] * (1 + len(s2)) for i in range(1 + len(s1))]
    longest, x_longest = 0, 0
    for x in range(1, 1 + len(s1)):
        for y in range(1, 1 + len(s2)):
            if s1[x - 1] == s2[y - 1]:
                m[x][y] = m[x - 1][y - 1] + 1
                if m[x][y] > longest:
                    longest = m[x][y]
                    x_longest = x
            else:
                m[x][y] = 0
    return s1[x_longest - longest: x_longest]

def long_substr(data):
    substr = ''
    if len(data) > 1 and len(data[0]) > 0:
        for i in range(len(data[0])):
            for j in range(len(data[0])-i+1):
                if j > len(substr) and is_substr(data[0][i:i+j], data):
                    substr = data[0][i:i+j]
    return substr

def is_substr(find, data):
    if len(data) < 1 and len(find) < 1:
        return False
    for i in range(len(data)):
        if find not in data[i]:
            return False
    return True

if __name__ == "__main__":
    a = [''.join(random.choice('asdbe') for _ in range(10000)) for __ in range(2)]
    print(a)
    start = time.process_time()
    print(lcs.lcs(a))
    end = time.process_time()
    delta = end - start
    print("Time when using vector: {:.4f} ms".format(delta*1000))

    start = time.process_time()
    print(lcs.lcs_map(a))
    end = time.process_time()
    delta = end - start
    print("Time with using map and string: {:.4f} ms".format(delta * 1000))

    start = time.process_time()
    print(longest_common_substring(a[0],a[1]))
    end = time.process_time()
    delta = end - start
    print("Time with using Python: {:.4f} ms".format(delta * 1000))

    start = time.process_time()
    print(long_substr(a))
    end = time.process_time()
    delta = end - start
    print("Time with using Python LCS_DYN: {:.4f} ms".format(delta * 1000))