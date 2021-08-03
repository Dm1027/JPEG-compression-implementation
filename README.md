# JPEG-compression-implementation

Implemented JPEG compression algorithm on .ppm image. 

Compile and run program with 1 arg of the P6 .ppm image location

Steps:
Read Photo in to 3 arrays, 1 for each color channel
Process Discrete Cosine Transformation(DCT) on each 8x8 block 
Quantize the data with a quality 10 (change q10 to q90 within the code to change compression rate on ln. 282, 283)
Inverse Quantization
Inverse DCT
Generate new p6 .ppm image to show the difference in compression rates


Note: This program is not compressing the image, but performing all the functions so that a before and after may be seen for different compression qualities 
