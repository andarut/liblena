import numpy as np
import math

def alpha(k):
    """Normalization factor α(k)."""
    return 1 / math.sqrt(2) if k == 0 else 1

def fdct_8x8(block):
    """
    Perform Forward Discrete Cosine Transform (FDCT) on an 8x8 block.
    Matches JPEG implementation with recentering around zero.
    """
    # Step 1: Recenter around zero
    block = block - 128

    # Step 2: Apply FDCT
    G = np.zeros((8, 8), dtype=int)

    for u in range(8):
        for v in range(8):
            sum_val = 0.0
            for x in range(8):
                for y in range(8):
                    sum_val += block[x, y] * \
                               math.cos(((2*x+1) * u * math.pi) / 16) * \
                               math.cos(((2*y+1) * v * math.pi) / 16)

            G[u, v] = round(0.25 * alpha(u) * alpha(v) * sum_val)

    return G

def print_matrix(matrix, name):
    """Print 8x8 matrix with only commas."""
    print(f"\n{name}:")
    for row in matrix:
        print(",".join(str(x) for x in row))

def test_fdct():
    test_cases = {
        "All zeros": np.zeros((8, 8), dtype=int),
        "All 255": np.full((8, 8), 255, dtype=int),
        "Gradient": np.array([[i*32 + j*4 for j in range(8)] for i in range(8)], dtype=int),
        "Random": np.random.randint(0, 256, (8, 8), dtype=int)
    }

    for name, block in test_cases.items():
        print(f"\n=== Test Case: {name} ===")
        print_matrix(block, "Input Block")
        dct_matrix = fdct_8x8(block)
        print_matrix(dct_matrix, "DCT Coefficients (rounded)")

if __name__ == "__main__":
    test_fdct()
