# XJCO3811 Computer Graphics CW1 Report

## Task Overview

1.1 Setting Pixels



## Task 1.1 Setting Pixels

In Task 1.1, I implemented essential functions for rendering pixels to visualize a particle field. Specifically, I created the `set_pixel_srgb` and `get_linear_index` functions, which enable me to position and color individual pixels within a defined window.



The positions of critical pixels in the window help define the rendering area:

- **(0, 0) - Top-Left Corner**: This is the origin of the window, located at the top-left.
- **(w−1, 0) - Top-Right Corner**: The top-right edge of the window, marking the horizontal boundary.
- **(0, h−1) - Bottom-Left Corner**: The bottom-left edge of the window, marking the vertical boundary.