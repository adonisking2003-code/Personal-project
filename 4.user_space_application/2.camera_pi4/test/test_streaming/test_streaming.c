
#include "streaming/gst_stream.h"

int main() {
    gst_stream_init();
    // Start streaming with custom resolution and bitrate
    gst_stream_start("/dev/video0", "0.0.0.0", 5000, 1280, 720, 1500);
    return 0;
}
