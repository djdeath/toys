/*
 * Clutter PLY - A library for displaying PLY models in a Clutter scene
 * Copyright (C) 2010  Intel Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(__CLUTTER_PLY_H_INSIDE__) && !defined(CLUTTER_PLY_COMPILATION)
#error "Only <clutter-ply/clutter-ply.h> can be included directly."
#endif

#ifndef __CLUTTER_PLY_DATA_H__
#define __CLUTTER_PLY_DATA_H__

#include <clutter/clutter.h>

G_BEGIN_DECLS

#define CLUTTER_PLY_TYPE_DATA                   \
  (clutter_ply_data_get_type())
#define CLUTTER_PLY_DATA(obj)                           \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj),                   \
                               CLUTTER_PLY_TYPE_DATA,   \
                               ClutterPlyData))
#define CLUTTER_PLY_DATA_CLASS(klass)                   \
  (G_TYPE_CHECK_CLASS_CAST ((klass),                    \
                            CLUTTER_PLY_TYPE_DATA,      \
                            ClutterPlyDataClass))
#define CLUTTER_PLY_IS_DATA(obj)                        \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                   \
                               CLUTTER_PLY_TYPE_DATA))
#define CLUTTER_PLY_IS_DATA_CLASS(klass)                \
  (G_TYPE_CHECK_CLASS_TYPE ((klass),                    \
                            CLUTTER_PLY_TYPE_DATA))
#define CLUTTER_PLY_DATA_GET_CLASS(obj)                 \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                    \
                              CLUTTER_PLY_TYPE_DATA,    \
                              ClutterPlyDataClass))

/**
 * CLUTTER_PLY_DATA_ERROR:
 *
 * Error domain for #ClutterPlyData errors
 */
#define CLUTTER_PLY_DATA_ERROR clutter_ply_data_error_quark ()

typedef struct _ClutterPlyData        ClutterPlyData;
typedef struct _ClutterPlyDataClass   ClutterPlyDataClass;
typedef struct _ClutterPlyDataPrivate ClutterPlyDataPrivate;

/**
 * ClutterPlyDataClass:
 *
 * The #ClutterPlyDataClass structure contains only private data.
 */
struct _ClutterPlyDataClass
{
  /*< private >*/
  GObjectClass parent_class;
};

/**
 * ClutterPlyData:
 *
 * The #ClutterPlyData structure contains only private data.
 */
struct _ClutterPlyData
{
  /*< private >*/
  GObject parent;

  ClutterPlyDataPrivate *priv;
};

/**
 * ClutterPlyDataError:
 * @CLUTTER_PLY_DATA_ERROR_PLY: The underlying PLY library reported an
 *  error.
 * @CLUTTER_PLY_DATA_ERROR_MISSING_PROPERTY: A property that is needed
 *  by #ClutterPlyData is not present in the file. For example, this
 *  will happen if the file does not contain the x, y and z properties.
 * @CLUTTER_PLY_DATA_ERROR_INVALID: The PLY file is not valid.
 * @CLUTTER_PLY_DATA_ERROR_UNSUPPORTED: The PLY file is not supported
 *  by your GL driver. This will happen if your driver can't support
 *  GL_UNSIGNED_INT indices but the model has more than 65,536
 *  vertices.
 *
 * Error enumeration for #ClutterPlyData
 */
typedef enum
  {
    CLUTTER_PLY_DATA_ERROR_PLY,
    CLUTTER_PLY_DATA_ERROR_MISSING_PROPERTY,
    CLUTTER_PLY_DATA_ERROR_INVALID,
    CLUTTER_PLY_DATA_ERROR_UNSUPPORTED
  } ClutterPlyDataError;

GType clutter_ply_data_get_type (void) G_GNUC_CONST;

ClutterPlyData *clutter_ply_data_new (void);

gboolean clutter_ply_data_load (ClutterPlyData *self,
                                const gchar *filename,
                                GError **error);

void clutter_ply_data_render (ClutterPlyData *self);

GQuark clutter_ply_data_error_quark (void);

void clutter_ply_data_get_extents (ClutterPlyData *self,
                                   ClutterVertex *min_vertex,
                                   ClutterVertex *max_vertex);

G_END_DECLS

#endif /* __CLUTTER_PLY_DATA_H__ */
