#!./../.venv/bin/python3

import os
import sys

Q = 50

def encode(input_path: str, encoded_path: str):
    encode_cmd = f"cjpeg -quality {Q} -baseline -dct float -sample 1x1,1x1,1x1 -outfile {encoded_path} {input_path}"
    print(f"ENCODE {encode_cmd}")
    os.system(encode_cmd)
             
def decode(encoded_path: str, decoded_path: str):
    decode_cmd = f"djpeg -dct float -dither none -nosmooth -rgb -onepass -outfile {decoded_path} {encoded_path}"
    print(f"DECODE {decode_cmd}")
    os.system(decode_cmd)

TEST_CASES = [
  "8x8",
  "9x9",
  "1",
  "2",
  "3",
  "4",
  "16x16",
  "128",
  "129",
  "lenna",
]

for i in range(4096):
    TEST_CASES.append(f"block_{i}")

for case in TEST_CASES:
    input_path = case + ".ppm"
    output_path = case + ".jpeg"
    decoded_path = "decoded_" + input_path
    encode(input_path, output_path);
    decode(output_path, decoded_path);
