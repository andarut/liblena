from PIL import Image
import numpy as np

def print_rgb_matrices(image_path):
    # Open the image
    img = Image.open(image_path).convert('RGB')
    
    # Convert to NumPy array of shape (height, width, 3)
    arr = np.array(img)
    
    # Split into R, G, B matrices
    R = arr[:, :, 0]
    G = arr[:, :, 1]
    B = arr[:, :, 2]
    
    # Print shapes and contents
    print("R channel matrix (shape {}):".format(R.shape))
    print(R)
    print("\nG channel matrix (shape {}):".format(G.shape))
    print(G)
    print("\nB channel matrix (shape {}):".format(B.shape))
    print(B)

if __name__ == "__main__":
    # Replace 'your_image.jpg' with the path to your JPEG file
    print_rgb_matrices('test.jpg')
