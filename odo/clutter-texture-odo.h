/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Authored By Tomas Frydrych  <tf@openedhand.com>
 *
 * Copyright (C) 20067 OpenedHand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _HAVE_CLUTTER_TEXTURE_ODO_H
#define _HAVE_CLUTTER_TEXTURE_ODO_H

#include <clutter/clutter-actor.h>
#include <clutter/clutter-texture.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_TEXTURE_ODO (clutter_texture_odo_get_type ())

#define CLUTTER_TEXTURE_ODO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CLUTTER_TYPE_TEXTURE_ODO, ClutterTextureOdo))

#define CLUTTER_TEXTURE_ODO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CLUTTER_TYPE_TEXTURE_ODO, ClutterTextureOdoClass))

#define CLUTTER_IS_TEXTURE_ODO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CLUTTER_TYPE_TEXTURE_ODO))

#define CLUTTER_IS_TEXTURE_ODO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CLUTTER_TYPE_TEXTURE_ODO))

#define CLUTTER_TEXTURE_ODO_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CLUTTER_TYPE_TEXTURE_ODO, ClutterTextureOdoClass))

typedef struct _ClutterTextureOdo        ClutterTextureOdo;
typedef struct _ClutterTextureOdoPrivate ClutterTextureOdoPrivate;
typedef struct _ClutterTextureOdoClass   ClutterTextureOdoClass;

struct _ClutterTextureOdo
{
  ClutterActor                 parent;
  
  /*< priv >*/
  ClutterTextureOdoPrivate    *priv;
};

struct _ClutterTextureOdoClass 
{
  ClutterActorClass parent_class;

  /* padding for future expansion */
  void (*_clutter_clone_1) (void);
  void (*_clutter_clone_2) (void);
  void (*_clutter_clone_3) (void);
  void (*_clutter_clone_4) (void);
}; 

typedef void (*ClutterTextureDistortFunc) (ClutterTexture * tex,
					   gint x, gint y, gint z,
					   gint *x2, gint *y2, gint *z2,
					   gpointer user_data);

GType           clutter_texture_odo_get_type           (void) G_GNUC_CONST;

ClutterActor *  clutter_texture_odo_new                (ClutterTexture      *texture);
ClutterTexture *clutter_texture_odo_get_parent_texture (ClutterTextureOdo   *otex);
void            clutter_texture_odo_set_parent_texture (ClutterTextureOdo   *otex,
							ClutterTexture      *tex);

G_END_DECLS

#endif
