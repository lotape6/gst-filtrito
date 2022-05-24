project('gst-filtrito', 'c', 'cpp', version : '0.0.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

gst_version = meson.project_version()

api_version = '1.0'

gst_dep = dependency('gstreamer-1.0',
    fallback : ['gstreamer', 'gst_dep'])

gst_bad_dep = dependency('gstreamer-plugins-bad-1.0',
    fallback : ['gstreamer', 'gst_dep'])

gst_video_dep = dependency('gstreamer-video-1.0',
    fallback : ['gstreamer', 'gst_video_dep'])

opencv_dep = dependency('opencv45',  required : false)

plugin_cpp_args = ['-DHAVE_CONFIG_H']

gstaudio_dep = dependency('gstreamer-audio-1.0',
    fallback: ['gst-plugins-base', 'audio_dep'])

cppdata = configuration_data()
cppdata.set_quoted('PACKAGE_VERSION', gst_version)
cppdata.set_quoted('PACKAGE', 'gst-filtrito')
cppdata.set_quoted('GST_LICENSE', 'LGPL')
cppdata.set_quoted('GST_API_VERSION', api_version)
cppdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer Hello World Plug-ins')
cppdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cppdata)

# Plugin (filter example)
filtrito_sources = [
  'src/gstfiltrito.cpp',
  ]

gstfiltrito = library('gstfiltrito',
  filtrito_sources,
  cpp_args : plugin_cpp_args + [ '-DGST_USE_UNSTABLE_API' ],
  dependencies : [gst_dep, gstaudio_dep, gst_video_dep, gst_bad_dep, opencv_dep],
  install : true,
  install_dir : plugins_install_dir,
)
