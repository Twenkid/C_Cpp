MIMCHO, За Мимчо с C270

https://stackoverflow.com/questions/4491649/how-to-convert-yuy2-to-a-bitmap-in-c


```c++
for (int i = 0;  i < width/2;  ++i)
{
    int y0 = ptrIn[0];
    int u0 = ptrIn[1];
    int y1 = ptrIn[2];
    int v0 = ptrIn[3];
    ptrIn += 4;
    int c = y0 - 16;
    int d = u0 - 128;
    int e = v0 - 128;
    ptrOut[0] = clip(( 298 * c + 516 * d + 128) >> 8); // blue
    ptrOut[1] = clip(( 298 * c - 100 * d - 208 * e + 128) >> 8); // green
    ptrOut[2] = clip(( 298 * c + 409 * e + 128) >> 8); // red
    c = y1 - 16;
    ptrOut[3] = clip(( 298 * c + 516 * d + 128) >> 8); // blue
    ptrOut[4] = clip(( 298 * c - 100 * d - 208 * e + 128) >> 8); // green
    ptrOut[5] = clip(( 298 * c + 409 * e + 128) >> 8); // red
    ptrOut += 6;
}
```
