#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H

// Device path for Pi camera (using V4L2 interface)
#define CAMERA_DEVICE_PATH "/dev/video0"
// Camera resolution settings
#define CAMERA_RESOLUTION_WIDTH 640
#define CAMERA_RESOLUTION_HEIGHT 480
// Camera frame rate
#define CAMERA_FRAME_RATE 30
// Camera pixel format (commonly YUV or MJPEG for V4L2)
#define CAMERA_PIXEL_FORMAT "YUYV"
// Buffer settings
#define CAMERA_BUFFER_COUNT 4
// Timeout for camera operations in milliseconds
#define CAMERA_OPERATION_TIMEOUT_MS 2000
// Camera brightness, contrast, saturation setting (0-255)
#define CAMERA_BRIGHTNESS 128
#define CAMERA_CONTRAST 128
#define CAMERA_SATURATION 128

// Enable/Disable camera auto exposure
#define CAMERA_AUTO_EXPOSURE_ENABLED 1
// Enable/Disable camera auto white balance
#define CAMERA_AUTO_WHITE_BALANCE_ENABLED 1

// End of camera configuration settings

#endif // CAMERA_CONFIG_H