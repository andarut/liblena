#!./../.venv/bin/python3

import sys
import numpy as np
from PIL import Image
import math

def calculate_mse_psnr(image_path1, image_path2):
    """
    Calculate Mean Squared Error (MSE) and Peak Signal-to-Noise Ratio (PSNR) 
    between two JPEG images.
    
    Args:
        image_path1: Path to first JPEG image
        image_path2: Path to second JPEG image
    
    Returns:
        tuple: (mse, psnr) values as floats
    """
    # Load images
    img1 = Image.open(image_path1)
    img2 = Image.open(image_path2)
    
    # Convert to RGB to ensure same format
    img1 = img1.convert('RGB')
    img2 = img2.convert('RGB')
    
    # Resize to same dimensions if needed (use smaller dimensions)
    if img1.size != img2.size:
        min_width = min(img1.width, img2.width)
        min_height = min(img1.height, img2.height)
        img1 = img1.resize((min_width, min_height))
        img2 = img2.resize((min_width, min_height))
        print(f"Warning: Images resized to {min_width}x{min_height} for comparison")
    
    # Convert to numpy arrays
    arr1 = np.array(img1, dtype=np.float64)
    arr2 = np.array(img2, dtype=np.float64)
    
    # Calculate MSE
    mse = np.mean((arr1 - arr2) ** 2)
    
    # Calculate PSNR
    # For 8-bit images, max pixel value is 255
    if mse == 0:
        psnr = float('inf')  # Images are identical
    else:
        max_pixel = 255.0
        psnr = 20 * math.log10(max_pixel / math.sqrt(mse))
    
    return mse, psnr

def main():
    if len(sys.argv) != 3:
        print("Usage: python mse_psnr.py <image1.jpg> <image2.jpg>")
        sys.exit(1)
    
    image_path1 = sys.argv[1]
    image_path2 = sys.argv[2]
    
    try:
        mse, psnr = calculate_mse_psnr(image_path1, image_path2)
        print(f"Image 1: {image_path1}")
        print(f"Image 2: {image_path2}")
        print(f"MSE:  {mse:.4f}")
        if psnr == float('inf'):
            print(f"PSNR: ∞ (identical images)")
        else:
            print(f"PSNR: {psnr:.4f} dB")
    except FileNotFoundError as e:
        print(f"Error: File not found - {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
