#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input.jpg>\n", argv[0]);
    return 1;
  }

  const char* filename = argv[1];
  FILE* infile = fopen(filename, "rb");

  if (!infile) {
    perror("Error opening file");
    return 1;
  }

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPARRAY buffer;
  int row_stride;

  // Set up error handling
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  // Specify the data source
  jpeg_stdio_src(&cinfo, infile);

  // Read the header
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;

  buffer = (*cinfo.mem->alloc_sarray)(
      (j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

  printf("Image dimensions: %d x %d\n", cinfo.output_width, cinfo.output_height);
  printf("Components per pixel: %d\n", cinfo.output_components);
  printf("Pixel values (row by row):\n");

  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);
    for (unsigned int i = 0; i < row_stride; i += cinfo.output_components) {
      for (int c = 0; c < cinfo.output_components; ++c) {
        printf("%3d ", buffer[0][i + c]);  // RGB or grayscale
      }
      printf("| ");
    }
    //printf("\n");
  }

  // Clean up
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  return 0;
}
