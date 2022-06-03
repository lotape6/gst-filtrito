project('gst-filtrito', 'cpp', version : '0.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cpp = meson.get_compiler('cpp')

gst_version = meson.project_version()

api_version = '1.0'

gst_dep = dependency('gstreamer-1.0',
    fallback : ['gstreamer', 'gst_dep'])

gstvideo_dep = dependency('gstreamer-video-1.0', 
    fallback : ['gst-plugins-base', 'video_dep'])
    
plugin_cpp_args = ['-DHAVE_CONFIG_H']


opencv_dep = dependency('opencv349',
    fallback: ['opencv', 'audio_dep'])

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-filtrito')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer Hello World Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

# Plugin 2 (audio filter example)
filtrito_sources = [
  'src/gstfiltrito.cpp',
  ]

gstfiltrito = library('gstfiltrito',
  filtrito_sources,
  cpp_args: plugin_cpp_args,
  dependencies : [gst_dep, opencv_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)
