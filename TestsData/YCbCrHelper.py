import numpy as np

def rgb_to_ycbcr_matrix(rgb_matrix):
    """
    Convert an 8x8 RGB matrix (values 0-255) to YCbCr using the given formulas.
    Returns Y, Cb, Cr matrices separately.
    """
    Y  = np.zeros((8, 8), dtype=int)
    Cb = np.zeros((8, 8), dtype=int)
    Cr = np.zeros((8, 8), dtype=int)

    for i in range(8):
        for j in range(8):
            R, G, B = rgb_matrix[i, j]

            Y[i, j]  = min(max(0, round(0.299*R + 0.587*G + 0.114*B)), 255)
            Cb[i, j] = min(max(0, round(-0.1687*R - 0.3313*G + 0.5*B + 128)), 255)
            Cr[i, j] = min(max(0, round(0.5*R - 0.4187*G - 0.0813*B + 128)), 255)

    return Y, Cb, Cr


def print_matrix(matrix, name):
    """Print 8x8 matrix with only commas."""
    print(f"\n{name}:")
    for row in matrix:
        print(",".join(str(x) for x in row) + ",")


def test_rgb_to_ycbcr():
    # Example test cases
    test_cases = {
        "All zeros (black)": np.zeros((8, 8, 3), dtype=int),
        "All 255 (white)": np.full((8, 8, 3), 255, dtype=int),
        "Red only": np.full((8, 8, 3), [255, 0, 0], dtype=int),
        "Green only": np.full((8, 8, 3), [0, 255, 0], dtype=int),
        "Blue only": np.full((8, 8, 3), [0, 0, 255], dtype=int),
        "Gradient": np.array([[(i*32, j*32, (i+j)*16) for j in range(8)] for i in range(8)], dtype=int),
        "Random": np.random.randint(0, 256, (8, 8, 3), dtype=int)
    }

    for name, rgb_matrix in test_cases.items():
        print(f"\n=== Test Case: {name} ===")

        # Split RGB channels
        R = rgb_matrix[:, :, 0]
        G = rgb_matrix[:, :, 1]
        B = rgb_matrix[:, :, 2]

        # Convert to YCbCr
        Y, Cb, Cr = rgb_to_ycbcr_matrix(rgb_matrix)

        # Print matrices
        print_matrix(R, "R")
        print_matrix(G, "G")
        print_matrix(B, "B")
        print_matrix(Y, "Y")
        print_matrix(Cb, "Cb")
        print_matrix(Cr, "Cr")


if __name__ == "__main__":
    test_rgb_to_ycbcr()
