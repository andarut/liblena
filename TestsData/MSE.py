import sys
import numpy as np
from PIL import Image

def calculate_mse(image_path1, image_path2):
    """
    Calculate Mean Squared Error (MSE) between two JPEG images.
    
    Args:
        image_path1: Path to first JPEG image
        image_path2: Path to second JPEG image
    
    Returns:
        MSE value as float
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
    
    return mse

def main():
    if len(sys.argv) != 3:
        print("Usage: python mse.py <image1.jpg> <image2.jpg>")
        sys.exit(1)
    
    image_path1 = sys.argv[1]
    image_path2 = sys.argv[2]
    
    try:
        mse = calculate_mse(image_path1, image_path2)
        print(f"MSE between '{image_path1}' and '{image_path2}': {mse:.4f}")
    except FileNotFoundError as e:
        print(f"Error: File not found - {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
