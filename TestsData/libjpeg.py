#!./../.venv/bin/python3

import os
import sys

def encode(input_path: str, encoded_path: str):
    encode_cmd = f"cjpeg -quality 50 -baseline -dct float -sample 1x1,1x1,1x1 -rgb {input_path} > {encoded_path}"
    print(f"ENCODE {encode_cmd}")
    os.system(encode_cmd)

def decode(encoded_path: str, decoded_path: str):
    decode_cmd = f"djpeg -dct float -dither none -onepass {encoded_path} > {decoded_path}"
    print(f"DECODE {decode_cmd}")
    os.system(decode_cmd)

assert(len(sys.argv) > 3)

if __name__ == "__main__":
    encode(sys.argv[1], sys.argv[2])
    decode(sys.argv[2], sys.argv[3])