#include "gst_stream.h"
#include <stdio.h>

static GstElement *pipeline = NULL;
static GMainLoop *loop = NULL;

void gst_stream_init(void) {
    gst_init(NULL, NULL);
}

int gst_stream_start(const char *device, const char *host, int port,
                     int width, int height, int bitrate) {
    if (pipeline != NULL) {
        fprintf(stderr, "Streaming already started!\n");
        return -1;
    }

    // Build pipeline string dynamically with resolution and bitrate
    char pipeline_str[512];
    int use_libcamera = 1;
    snprintf(pipeline_str, sizeof(pipeline_str),
        "%s ! video/x-raw,width=%d,height=%d,framerate=30/1 ! "
        "videoconvert ! x264enc tune=zerolatency bitrate=%d speed-preset=ultrafast ! "
        "rtph264pay ! gdppay ! tcpserversink host=%s port=%d",
        use_libcamera ? "libcamerasrc" : "v4l2src device=/dev/video0",
        width, height, bitrate, host, port);


    // Parse pipeline
    GError *error = NULL;
    pipeline = gst_parse_launch(pipeline_str, &error);
    if (!pipeline) {
        fprintf(stderr, "Failed to create pipeline: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    // Set pipeline to PLAYING
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        fprintf(stderr, "Failed to start streaming pipeline\n");
        gst_object_unref(pipeline);
        pipeline = NULL;
        return -1;
    }

    // Create and run main loop
    loop = g_main_loop_new(NULL, FALSE);
    g_print("Streaming started on %s:%d (Resolution: %dx%d, Bitrate: %d kbps)...\n",
            host, port, width, height, bitrate);
    g_main_loop_run(loop);

    return 0;
}

void gst_stream_stop(void) {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = NULL;
    }
    if (loop) {
        g_main_loop_quit(loop);
        g_main_loop_unref(loop);
        loop = NULL;
    }
    g_print("Streaming stopped.\n");
}