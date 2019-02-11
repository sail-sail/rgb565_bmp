### rgb565_bmp
rgb565 image cover to bmp image
```typescript
npm install --save rgb565_bmp
```
```typescript
import { readFile, writeFile } from "fs-extra";
import { rgb565_bmp } from "rgb565_bmp";

(async () => {
  const rgbBuffer = await readFile(`${__dirname}/test`);
  const bmpBuffer = await rgb565_bmp(rgbBuffer, 320, 240);
  await writeFile(`${__dirname}/test.bmp`);
})();
```
