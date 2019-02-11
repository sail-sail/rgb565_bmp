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
  BmpHead m_BMPHeader;        
  m_BMPHeader.imageSize=3*nWidth*nHeight+54;
  m_BMPHeader.blank=0;
  m_BMPHeader.startPosition=54;
  
  rgb_buffer[0] = 0x42;
  rgb_buffer[1] = 0x4D;
  rgb_buffer += 2;
  
  memcpy(rgb_buffer, &m_BMPHeader.imageSize, sizeof(m_BMPHeader.imageSize));
  rgb_buffer += sizeof(m_BMPHeader.imageSize);
  memcpy(rgb_buffer, &m_BMPHeader.blank, sizeof(m_BMPHeader.blank));
  rgb_buffer += sizeof(m_BMPHeader.blank);
  memcpy(rgb_buffer, &m_BMPHeader.startPosition, sizeof(m_BMPHeader.startPosition));
  rgb_buffer += sizeof(m_BMPHeader.startPosition);
  
  InfoHead  m_BMPInfoHeader;
  m_BMPInfoHeader.Length=40; 
  m_BMPInfoHeader.width=nWidth;
  m_BMPInfoHeader.height=nHeight;
  m_BMPInfoHeader.colorPlane=1;
  m_BMPInfoHeader.bitColor=24;
  m_BMPInfoHeader.zipFormat=0;
  m_BMPInfoHeader.realSize=3*nWidth*nHeight;
  m_BMPInfoHeader.xPels=0;
  m_BMPInfoHeader.yPels=0;
  m_BMPInfoHeader.colorUse=0;
  m_BMPInfoHeader.colorImportant=0;

  memcpy(rgb_buffer, &m_BMPInfoHeader.Length, sizeof(m_BMPInfoHeader.Length));
  rgb_buffer += sizeof(m_BMPInfoHeader.width);
  memcpy(rgb_buffer, &m_BMPInfoHeader.width, sizeof(m_BMPInfoHeader.width));
  rgb_buffer += sizeof(m_BMPInfoHeader.width);
  memcpy(rgb_buffer, &m_BMPInfoHeader.height, sizeof(m_BMPInfoHeader.height));
  rgb_buffer += sizeof(m_BMPInfoHeader.height);
  memcpy(rgb_buffer, &m_BMPInfoHeader.colorPlane, sizeof(m_BMPInfoHeader.colorPlane));
  rgb_buffer += sizeof(m_BMPInfoHeader.colorPlane);
  memcpy(rgb_buffer, &m_BMPInfoHeader.bitColor, sizeof(m_BMPInfoHeader.bitColor));
  rgb_buffer += sizeof(m_BMPInfoHeader.bitColor);
  memcpy(rgb_buffer, &m_BMPInfoHeader.zipFormat, sizeof(m_BMPInfoHeader.zipFormat));
  rgb_buffer += sizeof(m_BMPInfoHeader.zipFormat);
  memcpy(rgb_buffer, &m_BMPInfoHeader.realSize, sizeof(m_BMPInfoHeader.realSize));
  rgb_buffer += sizeof(m_BMPInfoHeader.realSize);
  memcpy(rgb_buffer, &m_BMPInfoHeader.xPels, sizeof(m_BMPInfoHeader.xPels));
  rgb_buffer += sizeof(m_BMPInfoHeader.xPels);
  memcpy(rgb_buffer, &m_BMPInfoHeader.yPels, sizeof(m_BMPInfoHeader.yPels));
  rgb_buffer += sizeof(m_BMPInfoHeader.yPels);
  memcpy(rgb_buffer, &m_BMPInfoHeader.colorUse, sizeof(m_BMPInfoHeader.colorUse));
  rgb_buffer += sizeof(m_BMPInfoHeader.colorUse);
  memcpy(rgb_buffer, &m_BMPInfoHeader.colorImportant, sizeof(m_BMPInfoHeader.colorImportant));
  rgb_buffer += sizeof(m_BMPInfoHeader.colorImportant);
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