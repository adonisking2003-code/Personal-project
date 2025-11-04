App (V4L2 API)
   ↓
Kernel Camera Driver
   ↓
I²C (sensor control) + MIPI CSI (image data)
   ↓
Camera Sensor

Yes, if you use a Raspberry Pi Camera Module (or any CSI camera), you need both MIPI CSI and I²C configured, but they serve different purposes:

✅ MIPI CSI

Role: High-speed data link for image data.
The camera sensor sends pixel data to the SoC over MIPI CSI lanes.
This is handled by the Pi’s hardware and kernel driver (you don’t configure this manually in your app).
When you enable the camera in Raspberry Pi OS (raspi-config or device tree), the CSI interface is activated.


✅ I²C

Role: Control channel for camera configuration.
Used to send commands to the camera sensor (e.g., set resolution, exposure, gain).
The Pi camera driver uses I²C internally to talk to the sensor.
You usually don’t need to manually send I²C commands unless you’re writing a custom driver or doing low-level sensor tuning.


✅ How V4L2 fits in

V4L2 operates above these hardware layers.
When you call VIDIOC_S_FMT or VIDIOC_REQBUFS, the kernel driver translates that into:

I²C commands → configure sensor registers.
CSI setup → stream image data.



So your application only uses V4L2, not raw MIPI or I²C.