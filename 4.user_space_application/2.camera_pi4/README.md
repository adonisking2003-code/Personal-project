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

##############################################################
##############################################################

✅ 1. Capture Local (V4L2 hoặc libcamera API)

Mục tiêu: lấy frame về RAM để xử lý trực tiếp trong ứng dụng.
Dùng cho:

Computer Vision (OpenCV, AI inference).
Lưu ảnh/video vào file.


Bạn có toàn quyền áp dụng các xử lý ảnh như:

AWB (Auto White Balance).
Scaling / Resize.
Color correction, denoise, crop.


Thường thực hiện bằng OpenCV hoặc pipeline xử lý trong code.


✅ 2. Streaming ra Laptop (GStreamer pipeline)

Mục tiêu: gửi video qua mạng (TCP/UDP/RTSP).
Dùng cho:

Hiển thị từ xa.
Truyền dữ liệu cho client.


Xử lý ảnh có thể thực hiện:

Trong pipeline GStreamer (thêm phần tử như videobalance cho AWB, videoscale cho resize).
Hoặc trước khi đẩy vào pipeline (nếu dùng appsrc để stream frame từ code).


################################  
################################
✅ Chi tiết:

Cảm biến OV và IMX xuất dữ liệu theo chuẩn giao tiếp (MIPI CSI, DVP, v.v.).
Dữ liệu này thường là:

RAW Bayer (ví dụ: 10-bit, 12-bit).
Hoặc YUV nếu ISP (Image Signal Processor) đã xử lý.


Nếu bạn dùng trực tiếp sensor qua CSI mà không có ISP, bạn sẽ nhận raw Bayer.
Nếu dùng camera module với ISP tích hợp (như Pi Camera với libcamera), bạn có thể yêu cầu:

JPEG (nén sẵn).
YUV/RGB (đã xử lý màu).
RAW Bayer (cho xử lý ảnh nâng cao).




✅ Vậy khác nhau ở đâu?

OV và IMX đều trả raw nếu bạn lấy trực tiếp từ sensor.
Nếu có ISP (trên SoC hoặc trong module), bạn có thể nhận ảnh đã xử lý (YUV, RGB, JPEG).


📌 Tóm lại:

Capture bằng V4L2 hoặc libcamera → bạn chọn format (RAW, YUV, RGB, JPEG).
Streaming hoặc hiển thị → cần encode hoặc convert (ví dụ H.264, MJPEG).


#######################################
#######################################
Đúng rồi, khi bạn dùng libcamera, nó không chỉ đọc raw từ sensor mà còn có thể đi qua ISP (Image Signal Processor) nếu bạn yêu cầu output ở dạng đã xử lý.

✅ Cách hoạt động của libcamera:

Sensor → ISP → libcamera → ứng dụng.
ISP thực hiện:

Demosaicing (chuyển từ RAW Bayer sang RGB/YUV).
AWB (Auto White Balance).
Noise reduction, sharpening.
Scaling.


Bạn có thể yêu cầu libcamera trả về:

RAW Bayer (bỏ qua ISP).
Processed YUV/RGB (qua ISP).
JPEG/H.264 (nếu pipeline có encoder).



