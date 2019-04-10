#include <node_api.h>
#include <stdlib.h>
#include <string.h>

#include "rgb565_bmp.h"

/****************************
      RGB加上头部信息转换成BMP
      参数說明：
      rgb_buffer        :RGB数据文件中的信息, 包含前54字节
      nWidth            :图像宽度的像素数
      nHeight           :图像高度的像素数
*****************************/
int RGB2BMP(unsigned char* rgb_buffer, int nWidth, int nHeight) {      
  
  rgb_buffer[0] = 0x42;
  rgb_buffer[1] = 0x4D;
  rgb_buffer[2] = 0x36;
  rgb_buffer[3] = 0x84;
  rgb_buffer[4] = 0x03;
  rgb_buffer[5] = 0x00;
  rgb_buffer[6] = 0x00;
  rgb_buffer[7] = 0x00;
  
  rgb_buffer[8] = 0x00;
  rgb_buffer[9] = 0x00;
  rgb_buffer[10] = 0x36;
  rgb_buffer[11] = 0x00;
  rgb_buffer[12] = 0x00;
  rgb_buffer[13] = 0x00;
  rgb_buffer[14] = 0x28;
  rgb_buffer[15] = 0x00;
  
  rgb_buffer[16] = 0x00;
  rgb_buffer[17] = 0x00;
  rgb_buffer[18] = 0x40;
  rgb_buffer[19] = 0x01;
  rgb_buffer[20] = 0x00;
  rgb_buffer[21] = 0x00;
  rgb_buffer[22] = 0xF0;
  rgb_buffer[23] = 0x00;
  
  rgb_buffer[24] = 0x00;
  rgb_buffer[25] = 0x00;
  rgb_buffer[26] = 0x01;
  rgb_buffer[27] = 0x00;
  rgb_buffer[28] = 0x18;
  rgb_buffer[29] = 0x00;
  rgb_buffer[30] = 0x00;
  rgb_buffer[31] = 0x00;
  
  rgb_buffer[32] = 0x00;
  rgb_buffer[33] = 0x00;
  rgb_buffer[34] = 0x00;
  rgb_buffer[35] = 0x84;
  rgb_buffer[36] = 0x03;
  rgb_buffer[37] = 0x00;
  rgb_buffer[38] = 0x00;
  rgb_buffer[39] = 0x00;
  
  rgb_buffer[40] = 0x00;
  rgb_buffer[41] = 0x00;
  rgb_buffer[42] = 0x00;
  rgb_buffer[43] = 0x00;
  rgb_buffer[44] = 0x00;
  rgb_buffer[45] = 0x00;
  rgb_buffer[46] = 0x00;
  rgb_buffer[47] = 0x00;
  
  rgb_buffer[48] = 0x00;
  rgb_buffer[49] = 0x00;
  rgb_buffer[50] = 0x00;
  rgb_buffer[51] = 0x00;
  rgb_buffer[52] = 0x00;
  rgb_buffer[53] = 0x00;
  rgb_buffer[54] = 0x78;
  
  return 0;
}

napi_value rgb565_bmp(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 3;
  napi_value argv[3];
  
  status = napi_get_cb_info(env, info, &argc, argv, 0, 0);
  if(status != napi_ok || argc < 3) {
    napi_throw_type_error(env, "EBADARGS", "Wrong number of arguments");
    return 0;
  }
  napi_value rgbBuffer = argv[0];
  size_t rgbLength;
  unsigned short* rgb_buffer;
  status = napi_get_buffer_info(env, rgbBuffer, (void **)&rgb_buffer, &rgbLength);
  if(status != napi_ok) {
    napi_throw_type_error(env, "EBADAPI", "napi_get_buffer_info");
    return 0;
  }
  int nWidth = 0, nHeight = 0;
  napi_value nWidth_value = argv[1];
  status = napi_get_value_int32(env, nWidth_value, &nWidth);
  if(status != napi_ok) {
    napi_throw_type_error(env, "EBADAPI", "napi_get_value_int32 nWidth");
    return 0;
  }
  napi_value nHeight_value = argv[2];
  status = napi_get_value_int32(env, nHeight_value, &nHeight);
  if(status != napi_ok) {
    napi_throw_type_error(env, "EBADAPI", "napi_get_value_int32 nHeight");
    return 0;
  }
  
  unsigned long nData = nWidth * nHeight;
  unsigned long total = 54 + (nData * 3);
  unsigned char* bmp_buffer;
  napi_value result;
  status = napi_create_buffer(env, total, (void **)&bmp_buffer, &result);
  if(status != napi_ok) {
    napi_throw_type_error(env, "EBADAPI", "napi_create_buffer");
    return 0;
  }
  unsigned char* pVisit = bmp_buffer + 54;
  unsigned long i = 0;
  unsigned char R,G,B;

  while(i < nData) {
		R = *rgb_buffer & 0x1f;
		G = (*rgb_buffer >> 5) & 0x3f;
		B = (*rgb_buffer >> 11) & 0x1f;
		B = B << 3;
		G = G << 2;
		R = R << 3;
    *pVisit=R;
    pVisit++;
    *pVisit=G;
    pVisit++;
    *pVisit=B;
    pVisit++;
    rgb_buffer++;
    i++;
  }
  
  RGB2BMP(bmp_buffer, nWidth, nHeight);
  return result;
}

static napi_value Init(napi_env env, napi_value exports) {
  //napi_create_string_utf8(env, "world", NAPI_AUTO_LENGTH, &world);
  // napi_property_descriptor 为结构体，作用是描述扩展暴露的 属性/方法 的描述
  napi_property_descriptor desc = { "rgb565_bmp", 0, rgb565_bmp, 0, 0, 0, napi_default, 0 };
  napi_define_properties(env, exports, 1, &desc);  // 定义暴露的方法
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)