# Phase Shifting in Sinusoidal Coded Light

There are various types of Coded Light, one is the previously done called Gray Code pattern, and the other is the present: Sinusoidal Code pattern. This pattern is similar to the past in that it is only a grayscale image. But now, instead of opaque stripes we have **sinusoidal** stripes, which resemble a sine wave seen from above, or in a 2D plane.

## Overview

| Expression | Frequency |
| ---- | ---- |
| sin(t) | 1 wave in 2PI (6.28) |
| sin(5t) | 5 waves in 2PI (6.28) |
| sin(t / 306) | 1 wave in 1920 |
| sin(t / (1920 / 2PI)) | 1 wave in 1920 |
| sin(t / (1920 / 2PI) * N) | N waves in 1920 |

This how we arrive at the conclusion that, if we have 1920 pixels, 

> 1920 / 2PI = 306,

which means that 

> sin(t / 306) or sin(t / (1920 / 2PI))

produces one wave in a 1920 x 1080 image:

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/1-wave.png?raw=true)

Therefore, 

> sin(t / (1920 / 2PI) * N)

produces N waves in the mentioned image. If we want to have a pixel-based configuration in the code (the user only enters the pixels that she/he wants per wave), we would just have to compute from the pixel input:

> N = 1920 / pixels

For example, a 10-pixel wave looks:

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/10-pixel-waves.PNG?raw=true)

And a 15-pixel wave generates:

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/15-pixel-waves.PNG?raw=true)

Where we confirm that a full wave measures the amount of pixels specified.

## Tests

Source: https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/pattern/pattern.cpp

- Shift: 0

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/zero.png?raw=true)

- Shift: + 2PI / 3

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/2PI3.png?raw=true)

- Shift: - 2PI / 3

![img](https://github.com/the-other-mariana/vision/blob/master/phase-shifting-sinusoidal-code/pattern/neg2PI3.png?raw=true)
