import os
import random
import string

TARGET_SIZE_MB = 60
TARGET_SIZE_BYTES = TARGET_SIZE_MB * 1024 * 1024
OUTPUT_FILE = 'random_lines.txt'

def random_line(min_length=10, max_length=200):
    length = random.randint(min_length, max_length)
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length)) + '\n'

with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
    total_bytes = 0
    while total_bytes < TARGET_SIZE_BYTES:
        line = random_line()
        f.write(line)
        total_bytes += len(line.encode('utf-8'))  # точный размер в байтах

