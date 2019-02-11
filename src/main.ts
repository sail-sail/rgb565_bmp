import * as bindings from "bindings";
const addon = bindings("rgb565_bmp");

/**
 * rgb565编码格式转bmp格式
 * @param  {Buffer} rgbBuffer rgb565编码格式的文件
 * @param  {number} width 图片的宽度
 * @param  {number} height 图片的高度
 */
export async function rgb565_bmp(rgbBuffer: Buffer, width: number, height: number) :Promise<Buffer> {
  return addon.rgb565_bmp(rgbBuffer, width, height);
};