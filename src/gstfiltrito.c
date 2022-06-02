/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2022 lotape6 <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-filtrito
 *
 * FIXME:Describe filtrito here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! filtrito ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include "gstfiltrito.h"

GST_DEBUG_CATEGORY_STATIC (gst_filtrito_debug);
#define GST_CAT_DEFAULT gst_filtrito_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */

#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ BGR }")

#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ BGR }")


static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (VIDEO_SRC_CAPS)
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (VIDEO_SINK_CAPS)
    );

#define gst_filtrito_parent_class parent_class
G_DEFINE_TYPE (GstFiltrito, gst_filtrito, GST_TYPE_VIDEO_FILTER);
// G_DEFINE_TYPE_WITH_CODE (GstFiltrito, gst_filtrito,
//     GST_TYPE_VIDEO_FILTER,
//     GST_DEBUG_CATEGORY_INIT (gst_filtrito_debug, "filtrito",
//         0, "debug category for filtrito element"));



static void gst_filtrito_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_filtrito_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);

static gboolean gst_filtrito_sink_event (GstPad * pad,
    GstObject * parent, GstEvent * event);
static GstFlowReturn gst_filtrito_transform_ip (GstVideoFilter * trans,
                                                GstVideoFrame * frame);

static GstCaps * gst_filtrito_transform_caps (GstBaseTransform * trans, GstPadDirection dir,
  GstCaps * caps, GstCaps * filter);

/* GObject vmethod implementations */

/* initialize the filtrito's class */
static void
gst_filtrito_class_init (GstFiltritoClass * klass)
{
  GObjectClass *gobject_class;
  GstVideoFilterClass *video_filter_class = GST_VIDEO_FILTER_CLASS (klass);
  
  GstElementClass *gstelement_class;

  GstBaseTransformClass *base_transform_class =
      GST_BASE_TRANSFORM_CLASS (klass);
  
  gobject_class = (GObjectClass *) klass;
	gstelement_class = (GstElementClass *) klass;

  base_transform_class->transform_caps = gst_filtrito_transform_caps;
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_filtrito_transform_ip);
  gobject_class->set_property = gst_filtrito_set_property;
  gobject_class->get_property = gst_filtrito_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE));

  // gst_element_class_set_details_simple (GST_ELEMENT_CLASS (klass),
  //     "Filtrito",
  //     "FIXME:Generic",
  //     "FIXME:Generic Template Element", "lotape6 <<user@hostname.org>>");

	gst_element_class_set_static_metadata (gstelement_class,
		"Video magnification", "Filter/Effect/Video",
		"Magnifies small color or motion temporal variations",
		"Chris Hiszpanski <chris@hiszpanski.name>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_filtrito_init (GstFiltrito * filter)
{
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_event_function (filter->sinkpad,
      GST_DEBUG_FUNCPTR (gst_filtrito_sink_event));
  // gst_pad_set_chain_function (filter->sinkpad,
  //     GST_DEBUG_FUNCPTR (gst_filtrito_transform_ip));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  filter->silent = FALSE;
}

static void
gst_filtrito_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstFiltrito *filter = GST_FILTRITO (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_filtrito_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstFiltrito *filter = GST_FILTRITO (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstCaps *
gst_filtrito_transform_caps (GstBaseTransform * trans, GstPadDirection dir,
  GstCaps * caps, GstCaps * filter)
  {
  GstCaps *to, *ret;
  GstCaps *templ;
  GstStructure *structure;
  GstPad *other;
  gint i;

  to = gst_caps_new_empty ();

  for (i = 0; i < gst_caps_get_size (caps); i++) {
    const GValue *v;
    GValue list = { 0, };
    GValue val = { 0, };

    structure = gst_structure_copy (gst_caps_get_structure (caps, i));

    g_value_init (&list, GST_TYPE_LIST);

    g_value_init (&val, G_TYPE_STRING);
    g_value_set_string (&val, "BGR");
    gst_value_list_append_value (&list, &val);
    g_value_unset (&val);

    v = gst_structure_get_value (structure, "format");

    gst_value_list_merge (&val, v, &list);
    gst_structure_set_value (structure, "format", &val);
    g_value_unset (&val);
    g_value_unset (&list);

    gst_structure_remove_field (structure, "colorimetry");
    gst_structure_remove_field (structure, "chroma-site");

    gst_caps_append_structure (to, structure);

  }
  /* filter against set allowed caps on the pad */
  other = (dir == GST_PAD_SINK) ? trans->srcpad : trans->sinkpad;
  templ = gst_pad_get_pad_template_caps (other);
  ret = gst_caps_intersect (to, templ);
  gst_caps_unref (to);
  gst_caps_unref (templ);

  if (ret && filter) {
    GstCaps *intersection;

    intersection =
        gst_caps_intersect_full (filter, ret, GST_CAPS_INTERSECT_FIRST);
    gst_caps_unref (ret);
    ret = intersection;
  }

  return ret;


  }


/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_filtrito_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  GstFiltrito *filter;
  gboolean ret;

  filter = GST_FILTRITO (parent);

  GST_LOG_OBJECT (filter, "Received %s event: %" GST_PTR_FORMAT,
      GST_EVENT_TYPE_NAME (event), event);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps *caps;

      gst_event_parse_caps (event, &caps);
      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }
  return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_filtrito_transform_ip (GstVideoFilter * trans,
                           GstVideoFrame * frame)
{
  GstFiltrito *filter;

  filter = GST_FILTRITO (trans);

  if (filter->silent == FALSE)
    g_print ("I'm plugged, therefore I'm in.\n");

  /* just push out the incoming buffer without touching it */
  return GST_FLOW_OK;
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
filtrito_init (GstPlugin * filtrito)
{
  /* debug category for filtering log messages
   *
   * exchange the string 'Template filtrito' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_filtrito_debug, "filtrito",
      0, "Template filtrito");

  return gst_element_register (filtrito, "filtrito", GST_RANK_NONE,
      GST_TYPE_FILTRITO);
}

/* PACKAGE: this is usually set by meson depending on some _INIT macro
 * in meson.build and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use meson to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstfiltrito"
#endif

/* gstreamer looks for this structure to register filtritos
 *
 * exchange the string 'Template filtrito' with your filtrito description
 */
GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    filtrito,
    "Template filtrito",
    filtrito_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
