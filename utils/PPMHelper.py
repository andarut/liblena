import numpy as np

def read_ppm(filename):
	"""
	Read a PPM (P3 or P6) image and return it as a numpy array (H, W, 3).
	Values are in 0–255.
	"""
	with open(filename, "rb") as f:
		header = f.readline().decode().strip()
	if header not in ("P3", "P6"):
		raise ValueError("Unsupported PPM format: " + header)

	# Skip comments
	line = f.readline().decode()
	while line.startswith("#"):
		line = f.readline().decode()

	# Read width, height
	width, height = map(int, line.split())

	# Max color value
	maxval = int(f.readline().decode())
	if maxval != 255:
		raise ValueError("Only 8-bit PPM supported (maxval=255).")

	if header == "P6":  # Binary data
		raw_data = np.frombuffer(f.read(), dtype=np.uint8)
		img = raw_data.reshape((height, width, 3))
	else:  # P3 ASCII data
		data = []
		while len(data) < width * height * 3:
			line = f.readline().decode()
			if not line:
				break
			data.extend(map(int, line.split()))
		img = np.array(data, dtype=np.uint8).reshape((height, width, 3))

	return img

def print_matrix(matrix, name):
	"""Print a matrix with only commas."""
	print(f"\n{name}:")
	for row in matrix:
		print(",".join(str(x) for x in row) + ",")

def main():
	filename = "8x8.ppm"
	rgb_image = read_ppm(filename)

	R = rgb_image[:, :, 0]
	G = rgb_image[:, :, 1]
	B = rgb_image[:, :, 2]

	print_matrix(R, "R")
	print_matrix(G, "G")
	print_matrix(B, "B")

if __name__ == __main__:
	main()
