# References
# https://qiita.com/naoppy/items/8a2178383859178e6d7e
# https://pyimagesearch.com/2015/03/30/accessing-the-raspberry-pi-camera-with-opencv-and-python/

import cv2
import numpy
from picamera import PiCamera
from picamera.array import PiRGBArray

resolution = (160, 128)
framerate = 5

# Perspective Transformation
camera_x, camera_y = resolution
src_points = numpy.float32([
    [camera_x * 0.25, camera_y * 0.75],
    [camera_x * 0.75, camera_y * 0.75],
    [camera_x, camera_y],
    [0, camera_y]
])
dst_points = numpy.float32([
    [0, 0],
    [100, 0],
    [100, 100],
    [0, 100]
])
mat = cv2.getPerspectiveTransform(src_points, dst_points)

camera = PiCamera()
camera.resolution = resolution
camera.framerate = framerate
raw_capture = PiRGBArray(camera, size=resolution)

for frame in camera.capture_continuous(raw_capture, format="bgr", use_video_port=True):
    src_image = frame.array
    dst_image = cv2.warpPerspective(src_image, mat, (100, 100))
    hsv_image = cv2.cvtColor(dst_image, cv2.COLOR_BGR2HSV_FULL)
    
    points = [(65, 25), (65, 50), (65, 75),
              (80, 25), (80, 50), (80, 75),
              (95, 25), (95, 50), (95, 75)]
    pixel_values = [hsv_image[y, x][2] for (y, x) in points]
    
    print(" ".join(map(str, pixel_values)), flush=True)
    
    raw_capture.truncate(0)
