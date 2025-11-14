#ifndef GST_STREAM_H
#define GST_STREAM_H

#include <gst/gst.h>

// Initialize GStreamer (call once in your app)
void gst_stream_init(void);


/** 
* @brief Start streaming from camera device to TCP
* Parameters:
*   @param[in] device : camera device path (e.g., "/dev/video0")
*   @param[in] host: IP address to bind (e.g., "0.0.0.0")
*   @param[in] port: TCP port (e.g., 5000)
*   @param[in] width, height: resolution (e.g., 640x480)
*   @param[in] bitrate: encoder bitrate in kbps (e.g., 1000)
*/
int gst_stream_start(const char *device, const char *host, int port,
                     int width, int height, int bitrate);


// Stop streaming and cleanup
void gst_stream_stop(void);

#endif