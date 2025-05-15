from PIL import Image
import numpy as np

def print_rgb_matrices(image_path):
    img = Image.open(image_path).convert('RGB')
    arr = np.array(img)
    R = arr[:, :, 0]
    G = arr[:, :, 1]
    B = arr[:, :, 2]
    
    print(R)
    print(G)
    print(B)

if __name__ == "__main__":
    # print_rgb_matrices('test.jpg')
    print_rgb_matrices('test.ppm')
    # print_rgb_matrices('8x8.ppm')
