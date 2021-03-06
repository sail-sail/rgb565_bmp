import { readFile, writeFile } from "fs-extra";
import { rgb565_bmp } from "../src/main";

test("rgb565 to bmp", async () => {
  const rgbBuffer = await readFile(`${__dirname}/test`);
  const bmpBuffer = await readFile(`${__dirname}/test.bmp`);
  const bmpBuffer2 = await rgb565_bmp(rgbBuffer, 320, 240);
  await writeFile("./bmpBuffer2.jpg", bmpBuffer2);
  expect(bmpBuffer).toEqual(bmpBuffer2);
});