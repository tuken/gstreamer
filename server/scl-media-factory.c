#include "scl-media-factory.h"
#include "scl-media.h"

G_DEFINE_TYPE(SclMediaFactory, scl_media_factory, GST_TYPE_RTSP_MEDIA_FACTORY)

GST_DEBUG_CATEGORY_STATIC(scl_media_factory);

static gchararray on_format_location_full(GstElement *splitmux, gint fragment_id, GstSample *first_sample, gpointer user_data) {
    // g_print("format-location-full signal received! fragment_id=%d\n", fragment_id);

    // GstSegment *seg = gst_sample_get_segment(first_sample);
    // GstBuffer *buf = gst_sample_get_buffer(first_sample);
    // gint64 pts = GST_BUFFER_PTS(buf);
    // gint64 stream_time = gst_segment_to_stream_time(seg, GST_FORMAT_TIME, pts);

    // g_print("stream_time: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(stream_time));
    // g_print("start: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(seg->start));
    // g_print("stop: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(seg->stop));
    // g_print("duration: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(seg->duration));
    // g_print("\n");

    return g_strdup_printf("segment-%03d.mp4", fragment_id);
}
static void on_new_stream(GstRTSPMedia *media, GstRTSPStream *object, gpointer user_data) { GST_FIXME("new-stream!!!"); }

static void on_removed_stream(GstRTSPMedia *media, GstRTSPStream *object, gpointer user_data) { GST_FIXME("removed-stream!!!"); }

// configure仮想関数のオーバーライド
static GstRTSPMedia *scl_media_factory_construct(GstRTSPMediaFactory *factory, const GstRTSPUrl *url) {
    GST_CAT_FIXME(scl_media_factory, "construct!!!");

    GstRTSPMediaFactoryClass *parent_class = GST_RTSP_MEDIA_FACTORY_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(factory)));
    if (parent_class->construct)
        return parent_class->construct(factory, url);

    return NULL;
}

// create_element仮想関数のオーバーライド
static GstElement *scl_media_factory_create_element(GstRTSPMediaFactory *factory, const GstRTSPUrl *url) {
    GST_CAT_FIXME(scl_media_factory, "create_element!!!");

    GstElement *pipeline = gst_pipeline_new("scl-media-pipeline");

    GstElement *depay0 = gst_element_factory_make("rtph264depay", "depay0");
    GstElement *parse0 = gst_element_factory_make("h264parse", "parse0");
    g_object_set(parse0, "config-interval", 1, NULL);
    g_object_set(parse0, "update-timecode", TRUE, NULL);
    GstElement *queue0 = gst_element_factory_make("queue", "queue0");

    GstElement *depay1 = gst_element_factory_make("rtpmp4gdepay", "depay1");
    GstElement *parse1 = gst_element_factory_make("aacparse", "parse1");
    GstElement *queue1 = gst_element_factory_make("queue", "queue1");

    GstElement *sink = gst_element_factory_make("splitmuxsink", "sink0");

    GstStructure *mux_props = gst_structure_new("mp4mux", "reserved-moov-update-period", G_TYPE_UINT64, 1000000000, "reserved-max-duration", G_TYPE_UINT64, 8000000000, NULL);
    g_object_set(sink, "muxer-properties", mux_props, NULL);
    gst_structure_free(mux_props);

    g_object_set(sink, "location", "unused.mp4", NULL);
    g_object_set(sink, "max-size-time", (guint64)8000000000, NULL);
    g_object_set(sink, "async-finalize", TRUE, NULL);
    g_object_set(sink, "use-robust-muxing", TRUE, NULL);

    gst_bin_add_many(GST_BIN(pipeline), depay0, parse0, queue0, depay1, parse1, queue1, sink, NULL);

    // 自動では繋がらないっぽいので、手動で繋ぐ
    // gboolean is_video_linked = gst_element_link_many(depay0, parse0, queue0, sink, NULL);
    // GST_CAT_FIXME(scl_media_factory, "gst_element_link_many(video) = %s", is_video_linked ? "TRUE" : "FALSE");

    // gboolean is_audio_linked = gst_element_link_many(depay1, parse1, queue1, sink, NULL);
    // GST_CAT_FIXME(scl_media_factory, "gst_element_link_many(audio) = %s", is_audio_linked ? "TRUE" : "FALSE");

    GstPad *depay0_src_pad = gst_element_get_static_pad(depay0, "src");
    GstPad *parse0_sink_pad = gst_element_get_static_pad(parse0, "sink");
    GstPad *parse0_src_pad = gst_element_get_static_pad(parse0, "src");
    GstPad *queue0_sink_pad = gst_element_get_static_pad(queue0, "sink");
    GstPad *queue0_src_pad = gst_element_get_static_pad(queue0, "src");
    GstPad *sink_video_pad = gst_element_request_pad_simple(sink, "video");

    GstPadLinkReturn ret01 = gst_pad_link(depay0_src_pad, parse0_sink_pad);
    GST_FIXME("depay0とparse0のリンク = %d", ret01);
    GstPadLinkReturn ret02 = gst_pad_link(parse0_src_pad, queue0_sink_pad);
    GST_FIXME("parse0とqueue0のリンク = %d", ret02);
    GstPadLinkReturn ret03 = gst_pad_link(queue0_src_pad, sink_video_pad);
    GST_FIXME("queue0とsinkのリンク = %d", ret03);

    gst_object_unref(sink_video_pad);
    gst_object_unref(queue0_src_pad);
    gst_object_unref(queue0_sink_pad);
    gst_object_unref(parse0_src_pad);
    gst_object_unref(parse0_sink_pad);
    gst_object_unref(depay0_src_pad);

    GstPad *depay1_src_pad = gst_element_get_static_pad(depay1, "src");
    GstPad *parse1_sink_pad = gst_element_get_static_pad(parse1, "sink");
    GstPad *parse1_src_pad = gst_element_get_static_pad(parse1, "src");
    GstPad *queue1_sink_pad = gst_element_get_static_pad(queue1, "sink");
    GstPad *queue1_src_pad = gst_element_get_static_pad(queue1, "src");
    GstPad *sink_audio_pad = gst_element_request_pad_simple(sink, "audio_0");

    GstPadLinkReturn ret11 = gst_pad_link(depay1_src_pad, parse1_sink_pad);
    GST_FIXME("depay1とparse1のリンク = %d", ret11);
    GstPadLinkReturn ret12 = gst_pad_link(parse1_src_pad, queue1_sink_pad);
    GST_FIXME("parse1とqueue1のリンク = %d", ret12);
    GstPadLinkReturn ret13 = gst_pad_link(queue1_src_pad, sink_audio_pad);
    GST_FIXME("queue1とsinkのリンク = %d", ret13);

    gst_object_unref(sink_audio_pad);
    gst_object_unref(queue1_src_pad);
    gst_object_unref(queue1_sink_pad);
    gst_object_unref(parse1_src_pad);
    gst_object_unref(parse1_sink_pad);
    gst_object_unref(depay1_src_pad);

    return pipeline;
    // return gst_parse_launch("rtph264depay name=depay0 ! h264parse ! splitmuxsink location=unused.mp4 max-size-time=5000000000 async-finalize=true use-robust-muxing=true
    // muxer-properties={mp4mux,reserved-moov-update-period=1000000000,reserved-max-duration=8000000000}", NULL);
}

// create_pipeline仮想関数のオーバーライド
static GstElement *scl_media_factory_create_pipeline(GstRTSPMediaFactory *factory, GstRTSPMedia *media) {
    GST_CAT_FIXME(scl_media_factory, "create_pipeline!!!");

    GstRTSPMediaFactoryClass *parent_class = GST_RTSP_MEDIA_FACTORY_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(factory)));
    if (parent_class->create_pipeline)
        return parent_class->create_pipeline(factory, media);

    return NULL;
}

// configure仮想関数のオーバーライド
static void scl_media_factory_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media) {
    GST_CAT_FIXME(scl_media_factory, "configure!!!");

    g_signal_connect(media, "new-stream", G_CALLBACK(on_new_stream), NULL);
    g_signal_connect(media, "removed-stream", G_CALLBACK(on_removed_stream), NULL);

    GstElement *pipeline = gst_rtsp_media_get_element(media);
    if (pipeline) {
        GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
        if (sink) {
            g_signal_connect(sink, "format-location-full", G_CALLBACK(on_format_location_full), NULL);
            gst_object_unref(sink);
        }

        gst_object_unref(pipeline);
    }
}

static void scl_media_factory_init(SclMediaFactory *self) {
    // gst_rtsp_media_factory_set_media_gtype(GST_RTSP_MEDIA_FACTORY(self), SCL_TYPE_MEDIA);
}

static void scl_media_factory_class_init(SclMediaFactoryClass *klass) {
    GstRTSPMediaFactoryClass *factory_class = GST_RTSP_MEDIA_FACTORY_CLASS(klass);
    factory_class->construct = scl_media_factory_construct;
    factory_class->create_element = scl_media_factory_create_element;
    factory_class->create_pipeline = scl_media_factory_create_pipeline;
    factory_class->configure = scl_media_factory_configure;

    GST_DEBUG_CATEGORY_INIT(scl_media_factory, "sclmediafactory", 0, "SclMediaFactory");
}
