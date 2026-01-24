import numpy as np
from pathlib import Path

def load_ppm(filename):
    """Load a P6 (binary) PPM image into a numpy array"""
    with open(filename, 'rb') as f:
        header = f.readline().strip()
        if header != b'P6':
            raise ValueError("Only binary PPM (P6) supported")
        
        # Skip comments
        while True:
            line = f.readline()
            if line.startswith(b'#'):
                continue
            else:
                break
        
        width, height = map(int, line.split())
        maxval = int(f.readline())
        assert maxval <= 255, "Only 8-bit PPM supported"

        # Read image data
        data = np.frombuffer(f.read(), dtype=np.uint8)
        img = data.reshape((height, width, 3))
        return img

def save_ppm(filename, img):
    """Save a numpy array as P6 PPM"""
    h, w, c = img.shape
    assert c == 3, "Only RGB images supported"
    with open(filename, 'wb') as f:
        f.write(b'P6\n')
        f.write(f"{w} {h}\n255\n".encode())
        f.write(img.tobytes())

def split_and_save_blocks(img, out_dir, block_size=8):
    """Split image into 8x8 blocks and save each as a separate PPM"""
    h, w, _ = img.shape
    assert h % block_size == 0 and w % block_size == 0, "Image size must be multiple of block size"
    
    Path(out_dir).mkdir(parents=True, exist_ok=True)
    
    blocks_per_row = w // block_size
    block_index = 0
    
    for by in range(0, h, block_size):       # top → bottom
        for bx in range(0, w, block_size):   # left → right
            block = img[by:by+block_size, bx:bx+block_size, :]
            filename = Path(out_dir) / f"block_{block_index}.ppm"
            save_ppm(filename, block)
            block_index += 1

    print(f"Saved {block_index} blocks to {out_dir}")

# --------------------
# Example usage
input_file = "lenna.ppm"
output_folder = "MCUs"

image = load_ppm(input_file)
split_and_save_blocks(image, output_folder)

num_blocks = 4096

for i in range(num_blocks):
    print(f'RETURN_IF_ERROR(basicTestCase("block_{i}"), "Test {i} failed\\n");')

