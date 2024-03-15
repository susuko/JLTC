# References
# https://qiita.com/naoppy/items/8a2178383859178e6d7e
# https://pyimagesearch.com/2015/03/30/accessing-the-raspberry-pi-camera-with-opencv-and-python/

import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray

camera = PiCamera()
camera.resolution = (160, 128)
camera.framerate = 5
rawCapture = PiRGBArray(camera, size=(160, 128))

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    hsvImage = cv2.cvtColor(image, cv2.COLOR_BGR2HSV_FULL)
    points = [(112, 55), (112, 80), (112, 105),
              (120, 50), (120, 80), (120, 110),
              (127, 45), (127, 80), (127, 115)]
    pixelValues = [hsvImage[y, x][2] for (y, x) in points]
    print(" ".join(map(str, pixelValues)), flush=True)
    rawCapture.truncate(0)
