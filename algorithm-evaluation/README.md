# Pattern Detection Algorithm Evaluation

## Test Renders using Unity 3D

The test renders of the pattern in centered, front tilt (45°) and side tilt (45°) were done in Unity 3D, using a script that would get a frame of 1920x1080 resolution from a camera placed within a 1.5 meters distance accross the pattern, with a FOV of 84° that simulated the human eye.

Source: https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/renders/Assets/SnapshotCamera.cs

### Renders

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/res/renders-preview.png?raw=true)

## Error Plot

If we compute the difference between both detected centroids in each of the three **considered** (blobs, regions and ellipse) approaches, we will get an estimated error. Now, if we plot an histogram of these continuous error values by dividing the range in intervals and recording the frecuency of these errors inside the intervals, we would get the following plots. In this way, we can observe the distributions of the errors within the three 'equivalent' approaches.

- Center Pattern

### Grid Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/grid-center.png?raw=true)

### Single Plot Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-center.png?raw=true)

- Front-tilt Pattern

### Grid Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/grid-front-tilt.png?raw=true)

### Single Plot Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-front-tilt.png?raw=true)

- Side-tilt Pattern

### Grid Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/grid-side-tilt.png?raw=true)

### Single Plot Form

![img](https://github.com/the-other-mariana/vision/blob/master/algorithm-evaluation/histograms/out/condensed-side-tilt.png?raw=true)

## Interpretation

The **Center Pattern** error plots show that the errors fall into the same interval, due to the small error in a perfectly centered pattern render.

As we check further pattern renders, we can conclude that most of the errors for the blob technique are located in the smaller intervals, while the errors using the other two techniques are spread accross bigger intervals, towards the right side of the horizontal axis.

Therefore, it seems like a blob approach for pattern detection is the one that would yield the smallest errors.

## Handy Links

- [FOV Eye](https://petapixel.com/2012/11/17/the-camera-versus-the-human-eye/)

- [FOV for Games](https://forum.unity.com/threads/camera-setting-for-field-of-view.355424/)