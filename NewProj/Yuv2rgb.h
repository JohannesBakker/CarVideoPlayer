#ifndef __YUV2RGB_H_
#define __YUV2RGB_H_

void yuv2rgb_24(
	unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, 
	unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, 
	int width_y, int height_y,
	unsigned int stride_out);
void yuv2rgb_32(
	unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, 
	int width_y, int height_y,
	unsigned int stride_out);
void yuv2rgb_555(
	unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, 
	unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, 
	int width_y, int height_y,
	unsigned int stride_out);
void yuv2rgb_565(
	unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, 
	unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, 
	int width_y, int height_y,
	unsigned int stride_out);

void yuv12_out(unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, int width_y, int height_y,
	unsigned int stride_out);
void yuy2_out(unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, int width_y, int height_y,
	unsigned int stride_out); 
void uyvy_out(unsigned char *puc_y, int stride_y, 
	unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
	unsigned char *puc_out, int width_y, int height_y,
	unsigned int stride_out);

#endif
