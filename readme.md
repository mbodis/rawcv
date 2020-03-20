
**NOTES for build 2020-03-20**
https://docs.opencv.org/master/dd/dd5/tutorial_py_setup_in_fedora.html

We need CMake to configure the installation, GCC for compilation, Python-devel and Numpy for creating Python extensions etc.
`sudo dnf install -y cmake`
`sudo dnf install -y gcc gcc-c++`

Next we need GTK support for GUI features, Camera support (libdc1394, v4l), Media Support (ffmpeg, gstreamer) etc.
`sudo dnf install -y opencv`
`sudo dnf install -y opencv-devel`

Optional 
OpenCV comes with supporting files for image formats like PNG, JPEG, JPEG2000, TIFF, WebP etc. But it may be a little old. If you want to get latest libraries, you can install development files for these formats.
`sudo dnf install -y libpng-devel`
`sudo dnf install -y libjpeg-turbo-devel`
`sudo dnf install -y jasper-devel`
`sudo dnf install -y openexr-devel`
`sudo dnf install -y libtiff-devel`
`sudo dnf install -y libwebp-devel`

svb: problem with gdal lib - some listed below works :D
`sudo dnf install -y gdal` 
`sudo dnf install -y gdal-devel`
`sudo dnf install -y gdal-libs`
`sudo dnf install -y gdal23-devel gdal30-devel`
`sudo dnf install -y gdalcpp-devel`

