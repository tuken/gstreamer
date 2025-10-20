#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#ifndef __SCL_MEDIA_VIDEO_FACTORY_H__
#define __SCL_MEDIA_VIDEO_FACTORY_H__

G_BEGIN_DECLS

#define SCL_TYPE_MEDIA_VIDEO_FACTORY (scl_media_video_factory_get_type())

G_DECLARE_FINAL_TYPE(SclMediaVideoFactory, scl_media_video_factory, SCL, MEDIA_VIDEO_FACTORY, GstRTSPMediaFactory)

typedef struct _SclMediaVideoFactory SclMediaVideoFactory;

struct _SclMediaVideoFactory {
    GstRTSPMediaFactory parent;
};

// GST_RTSP_SERVER_API
// static GstElement *scl_media_factory_create_element(GstRTSPMediaFactory *factory, const GstRTSPUrl *url);

G_END_DECLS

#endif /* __SCL_MEDIA_VIDEO_FACTORY_H__ */
