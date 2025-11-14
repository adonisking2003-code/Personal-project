
#!/bin/bash
# Usage: ./laptop_view.sh <RPI_IP> <PORT>
RPI_IP=$1
PORT=$2

gst-launch-1.0 tcpclientsrc host=$RPI_IP port=$PORT ! \
gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink

# ./laptop_view.sh 192.168.1.100 5000