# CV-ROS
## A cv program based on ROS
* language:cpp
* function:detect the object on the ground and return the location and angle of the object to the moving part of our drone.
## algorithm:
* getimage.cpp gets image from the usb camera and deliver the message using ROS topic communication.
* processimage.cpp gets image from ROS topic communication and convert the image into a binary image.Then it finds the largest contour of the image and calculuates its minimum enclosing rectangle.Finally it calculuates the x,y and angle of the object.
![This is the image of detecting a smartphone](https://github.com/ZhangTianxi05/CV-ROS/blob/main/test.jpg)
