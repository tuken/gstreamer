#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#ifndef __SCL_MEDIA_H__
#define __SCL_MEDIA_H__

G_BEGIN_DECLS

#define SCL_TYPE_MEDIA (scl_media_get_type())

G_DECLARE_FINAL_TYPE(SclMedia, scl_media, SCL, MEDIA, GstRTSPMedia)

typedef struct _SclMedia SclMedia;

struct _SclMedia {
    GstRTSPMedia parent;
};

G_END_DECLS

#endif /* __SCL_MEDIA_H__ */
