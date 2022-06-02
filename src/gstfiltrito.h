/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2008 Michael Sheldon <mike@mikeasoft.com>
 * Copyright (C) 2011 Robert Jobbagy <jobbagy.robert@gmail.com>
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_FILTRITO_H__
#define __GST_FILTRITO_H__

#include <gst/opencv/gstopencvvideofilter.h>
#include <gst/video/gstvideofilter.h>

// #include <opencv2/core.hpp>
// #include <opencv2/objdetect.hpp>

G_BEGIN_DECLS
/* #defines don't like whitespacey bits */
#define GST_TYPE_FILTRITO \
  (gst_filtrito_get_type())
#define GST_FILTRITO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_FILTRITO,GstFiltrito))
#define GST_FILTRITO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_FILTRITO,GstFiltritoClass))
#define GST_IS_FILTRITO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_FILTRITO))
#define GST_IS_FILTRITO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_FILTRITO))
typedef struct _GstFiltrito GstFiltrito;
typedef struct _GstFiltritoClass GstFiltritoClass;

struct _GstFiltrito
{
  GstOpencvVideoFilter element;

  gboolean sent_profile_load_failed_msg;

  gchar *profile;
  gboolean display;
  gdouble scale_factor;

  cv::Mat cvGray;
  // cv::CascadeClassifier *cvCascade;
};

struct _GstFiltritoClass
{
  GstOpencvVideoFilterClass parent_class;
};

GType gst_filtrito_get_type (void);

GST_ELEMENT_REGISTER_DECLARE (filtrito);

G_END_DECLS
#endif /* __GST_FILTRITO_H__ */
