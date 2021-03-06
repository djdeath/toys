#include "wh-video-row-renderer.h"
#include "wh-video-model.h"
#include "wh-video-model-row.h"
#include "util.h"

G_DEFINE_TYPE (WHVideoRowRenderer, wh_video_row_renderer, CLUTTER_TYPE_ACTOR);

#define PAD 4

#define VIDEO_ROW_RENDERER_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), WH_TYPE_VIDEO_ROW_RENDERER, WHVideoRowRendererPrivate))

typedef struct _WHVideoRowRendererPrivate WHVideoRowRendererPrivate;

struct _WHVideoRowRendererPrivate
{
  WHVideoModelRow *row;
  ClutterActor    *container;
  ClutterActor    *thumbnail, *thumbnail_image;
  ClutterActor    *title_label, *info_label, *date_label, *hr;
  gint             width, height;
  gboolean         active;
};

enum
{
  PROP_0,
  PROP_ROW
};

static void
sync_thumbnail (WHVideoRowRenderer *renderer)
{
  GdkPixbuf                 *pixbuf; 
  WHVideoRowRendererPrivate *priv;  

  priv = VIDEO_ROW_RENDERER_PRIVATE(renderer);

  pixbuf = wh_video_model_row_get_thumbnail (priv->row);

  if (pixbuf)
    {
      ClutterEffectTemplate *effect;

      if (priv->thumbnail_image)
	g_object_unref (priv->thumbnail_image);

      
      priv->thumbnail_image = clutter_texture_new ();
      if (priv->thumbnail_image == NULL)
	return;

      clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (priv->thumbnail_image),
					 gdk_pixbuf_get_pixels (pixbuf),
					 gdk_pixbuf_get_has_alpha (pixbuf),
					 gdk_pixbuf_get_width (pixbuf),
					 gdk_pixbuf_get_height (pixbuf),
					 gdk_pixbuf_get_rowstride (pixbuf),
					 gdk_pixbuf_get_n_channels (pixbuf), 
					 0,
					 NULL);

      clutter_actor_set_position (priv->thumbnail_image, PAD + 2, PAD + 2);
      clutter_actor_set_size (priv->thumbnail_image, 
			      priv->height - (PAD*2) - 4, 
			      priv->height - (PAD*2) - 4);
      clutter_group_add(CLUTTER_GROUP(priv->container),
			priv->thumbnail_image);

      effect
	= clutter_effect_template_new (clutter_timeline_new (20, 60),
				       CLUTTER_ALPHA_SINE_INC);

      clutter_actor_set_opacity (priv->thumbnail_image, 0);
      clutter_actor_show (priv->thumbnail_image);
      clutter_effect_fade (effect,
			   priv->thumbnail_image,
			   0xff,
			   NULL,
			   NULL);
      g_object_unref (effect);
    }
}

static void 
on_thumbnail_change (GObject        *object,
		     GParamSpec     *pspec,
		     WHVideoRowRenderer *renderer)
{
  sync_thumbnail (renderer);
}

static void
wh_video_row_renderer_get_property (GObject *object, guint property_id,
				    GValue *value, GParamSpec *pspec)
{
  WHVideoRowRenderer        *row = WH_VIDEO_ROW_RENDERER(object);
  WHVideoRowRendererPrivate *priv;  
  
  priv = VIDEO_ROW_RENDERER_PRIVATE(row);

  switch (property_id) 
    {
    case PROP_ROW:
      g_value_set_object (value, priv->row);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
wh_video_row_renderer_set_property (GObject *object, guint property_id,
				    const GValue *value, GParamSpec *pspec)
{
  WHVideoRowRenderer        *row = WH_VIDEO_ROW_RENDERER(object);
  WHVideoRowRendererPrivate *priv;  
  
  priv = VIDEO_ROW_RENDERER_PRIVATE(row);

  switch (property_id) 
    {
    case PROP_ROW:
      if (priv->row)
	g_object_unref(priv->row);
      priv->row = g_value_get_object (value);
      g_signal_connect (priv->row,
			"notify::thumbnail",
			G_CALLBACK (on_thumbnail_change),
			row);
      g_object_ref(priv->row);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
wh_video_row_renderer_dispose (GObject *object)
{
  if (G_OBJECT_CLASS (wh_video_row_renderer_parent_class)->dispose)
    G_OBJECT_CLASS (wh_video_row_renderer_parent_class)->dispose (object);
}

static void
wh_video_row_renderer_finalize (GObject *object)
{
  G_OBJECT_CLASS (wh_video_row_renderer_parent_class)->finalize (object);
}

static void
wh_video_row_renderer_allocate (ClutterActor    *self,
				const ClutterActorBox *box,
				gboolean absolute_origin_changed)
{
  WHVideoRowRenderer        *row = WH_VIDEO_ROW_RENDERER(self);
  WHVideoRowRendererPrivate *priv;  
  ClutterActorBox            child_box;
  ClutterUnit                container_width, container_height;
  
  priv = VIDEO_ROW_RENDERER_PRIVATE(row);

  if ( (CLUTTER_UNITS_TO_INT(box->x2 - box->x1) != priv->width) 
       || (CLUTTER_UNITS_TO_INT(box->y2 - box->y1) != priv->height))
    {
      ClutterColor color      = { 0xcc, 0xcc, 0xcc, 0xff };
      ClutterColor info_color = { 0xde, 0xde, 0xde, 0xff };
      gint  w,h;
      gchar font_desc[32];
      gchar *episode = NULL, *series = NULL, *info = NULL;
      GDate *date;      
      gchar  date_buf[32];

      /* Keep a simple cache to avoid setting fonts up too much */
      w = priv->width  = CLUTTER_UNITS_TO_INT(box->x2 - box->x1);
      h = priv->height = CLUTTER_UNITS_TO_INT(box->y2 - box->y1);

      clutter_actor_set_position (priv->thumbnail, PAD, PAD);
      clutter_actor_set_size (priv->thumbnail, h-(PAD*2), h-(PAD*2));

      g_snprintf(font_desc, 32, "Sans %ipx", (h*4)/8); 

      clutter_label_set_text (CLUTTER_LABEL(priv->title_label),
			      wh_video_model_row_get_title (priv->row));
      clutter_label_set_font_name (CLUTTER_LABEL(priv->title_label), 
				   font_desc); 
      clutter_label_set_color (CLUTTER_LABEL(priv->title_label), &color);
      clutter_label_set_line_wrap (CLUTTER_LABEL(priv->title_label), FALSE);
      clutter_label_set_ellipsize  (CLUTTER_LABEL(priv->title_label), 
				    PANGO_ELLIPSIZE_MIDDLE);

      clutter_actor_set_width (priv->title_label, w - ((2*(h+PAD))));
      clutter_actor_set_position (priv->title_label, h + PAD, PAD); 

      g_snprintf(font_desc, 32, "Sans %ipx", (h*3)/12); 
      wh_video_model_row_get_extended_info (priv->row, &series, &episode);

      date = g_date_new();
      
      g_date_set_time_t (date, wh_video_model_row_get_age(priv->row)); 
      g_date_strftime (date_buf, 32, "%x", date);
      
      info = g_strdup_printf("%s%s%s%s%s%s"
			     "Added: %s",
			     series != NULL  ? "Series: " : "",
			     series != NULL  ?  series : "",
			     series != NULL  ?  " " : "",
			     episode != NULL ? "Episode: " : "",
			     episode != NULL ?  episode : "",
			     episode != NULL ?  " " : "",
			     date_buf);
      
      clutter_label_set_text (CLUTTER_LABEL(priv->info_label), info);
      clutter_label_set_font_name (CLUTTER_LABEL(priv->info_label), 
				   font_desc); 
      clutter_label_set_color (CLUTTER_LABEL(priv->info_label), 
			       &info_color);
      clutter_label_set_line_wrap (CLUTTER_LABEL(priv->info_label), FALSE);
      clutter_label_set_use_markup (CLUTTER_LABEL(priv->info_label), TRUE);
      
      clutter_actor_set_position (priv->info_label, 
				  h + PAD, 
				  PAD + clutter_actor_get_height(priv->title_label)); 
      clutter_actor_set_width (priv->title_label, w - (2*h) + (2*PAD));
      
      g_free (info);
      g_free (series);
      g_free (episode);
      g_date_free(date);

      clutter_actor_set_size (priv->hr, w, 1);
      clutter_actor_set_position (priv->hr, 0, h-1);

      sync_thumbnail (row);

      /* Force Update active look */
      priv->active = ~priv->active;
      wh_video_row_renderer_set_active (row, ~priv->active); 
    }
  
  clutter_actor_get_sizeu (priv->container, 
			   &container_width,
			   &container_height);
  child_box.x1 = 0;
  child_box.y1 = 0;
  child_box.x2 = container_width;
  child_box.y2 = container_height;
  clutter_actor_allocate (priv->container, 
			  &child_box, 
			  absolute_origin_changed);

  CLUTTER_ACTOR_CLASS (wh_video_row_renderer_parent_class)->
    allocate (self, box, absolute_origin_changed);
}

static void
wh_video_row_renderer_paint (ClutterActor *actor)
{
  WHVideoRowRenderer        *row = WH_VIDEO_ROW_RENDERER(actor);
  WHVideoRowRendererPrivate *priv;  
  
  priv = VIDEO_ROW_RENDERER_PRIVATE(row);

  if (priv->width == 0 || priv->height ==0)
    return;

  clutter_actor_paint (CLUTTER_ACTOR(priv->container));
}

static void
wh_video_row_renderer_class_init (WHVideoRowRendererClass *klass)
{
  GObjectClass        *object_class = G_OBJECT_CLASS (klass);
  ClutterActorClass   *actor_class = CLUTTER_ACTOR_CLASS (klass);

  g_type_class_add_private (klass, sizeof (WHVideoRowRendererPrivate));

  object_class->get_property = wh_video_row_renderer_get_property;
  object_class->set_property = wh_video_row_renderer_set_property;
  object_class->dispose      = wh_video_row_renderer_dispose;
  object_class->finalize     = wh_video_row_renderer_finalize;

  actor_class->paint          = wh_video_row_renderer_paint;
  actor_class->allocate = wh_video_row_renderer_allocate;
  /* 
   *  actor_class->realize    = wh_video_row_renderer__realize;
   *  actor_class->unrealize  = parent_class->unrealize;
  */  

  g_object_class_install_property 
    (object_class,
     PROP_ROW,
     g_param_spec_object ("row",
			  "Row",
			  "Row to render",
			  WH_TYPE_VIDEO_MODEL_ROW,
			  G_PARAM_CONSTRUCT|G_PARAM_READWRITE));
}

static void
wh_video_row_renderer_init (WHVideoRowRenderer *self)
{
  ClutterColor color = { 0xcc, 0xcc, 0xcc, 0xff };
  ClutterColor grey_col = { 0xde, 0xde, 0xde, 0xff };
  WHVideoRowRendererPrivate *priv;  
  
  priv = VIDEO_ROW_RENDERER_PRIVATE(self);

  priv->hr = clutter_rectangle_new_with_color (&grey_col);

  priv->thumbnail = clutter_rectangle_new_with_color(&color);

  priv->title_label = clutter_label_new();
  priv->info_label = clutter_label_new();

  priv->container = clutter_group_new();
  clutter_actor_set_parent (priv->container, CLUTTER_ACTOR(self));

  clutter_group_add_many (CLUTTER_GROUP(priv->container), 
			  priv->hr,
			  priv->thumbnail,
			  priv->title_label,
			  priv->info_label,
			  NULL);

  clutter_actor_show_all (priv->container);
}

void
wh_video_row_renderer_set_active (WHVideoRowRenderer *renderer, 
				  gboolean            setting)
{
  /* FIXME: should be prop */
  WHVideoRowRendererPrivate *priv = VIDEO_ROW_RENDERER_PRIVATE(renderer);

  ClutterColor inactive_col = { 0xaa, 0xaa, 0xaa, 0xff };
  ClutterColor   active_col = { 0xff, 0xff, 0xff, 0xff };
  ClutterColor info_inactive_col = { 0xbb, 0xbb, 0xbb, 0xff };
  ClutterColor info_active_col   = { 0xf3, 0xf3, 0xf3, 0xff };

  if (priv->active == setting)
    return;

  priv->active = setting;

  if (setting)
    {
      clutter_label_set_color (CLUTTER_LABEL(priv->title_label), 
			       &active_col);
      clutter_label_set_color (CLUTTER_LABEL(priv->info_label), 
			       &info_active_col);
      clutter_actor_set_opacity (CLUTTER_ACTOR(renderer), 0xff);

    }
  else
    {
      clutter_label_set_color (CLUTTER_LABEL(priv->title_label), 
			       &inactive_col);
      clutter_label_set_color (CLUTTER_LABEL(priv->info_label), 
			       &info_inactive_col);
      clutter_actor_set_opacity (CLUTTER_ACTOR(renderer), 0xff);
    }



}

WHVideoRowRenderer*
wh_video_row_renderer_new (WHVideoModelRow *row)
{
  return g_object_new (WH_TYPE_VIDEO_ROW_RENDERER, "row", row, NULL);
}

