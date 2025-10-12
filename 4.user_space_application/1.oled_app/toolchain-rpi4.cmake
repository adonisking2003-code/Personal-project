set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)  # hoặc arm nếu 32 bit

# Thay bằng đường dẫn thực tế đến sysroot của SDK Yocto
set(CMAKE_SYSROOT /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/x86_64-pokysdk-linux)

# Đường dẫn tới cross compiler trong SDK Yocto
set(CMAKE_C_COMPILER /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gcc)
set(CMAKE_CXX_COMPILER /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#run by command: cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi4.cmake ..