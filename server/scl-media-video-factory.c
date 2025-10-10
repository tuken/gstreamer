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
/*
static void on_prepared(GstRTSPMedia *media, gpointer user_data) {
    GST_FIXME("prepared!!!");

    GstElement *pipeline = gst_rtsp_media_get_element(media);
    // GstIterator *it = gst_bin_iterate_elements(GST_BIN(pipeline));
    // GValue item = G_VALUE_INIT;
    // while (gst_iterator_next(it, &item) == GST_ITERATOR_OK) {
    //     GstElement *elem = g_value_get_object(&item);
    //     g_print("Element: %s (%s)\n", GST_ELEMENT_NAME(elem), G_OBJECT_TYPE_NAME(elem));
    //     g_value_unset(&item);
    // }
    // gst_iterator_free(it);

    // element取得（rtph264depayとh264parseとsplitmuxsinkのnameを指定）
    GstElement *depay = gst_bin_get_by_name(GST_BIN(pipeline), "depay0");
    GST_FIXME("depay pointer: %p", depay);
    GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "parse0");
    GST_FIXME("parse pointer: %p", parse);
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
    GST_FIXME("sink pointer: %p", sink);
    GstStructure *mux_props = NULL;
    g_object_get(sink, "muxer-properties", &mux_props, NULL);
    if (mux_props) {
        gchar *str = gst_structure_to_string(mux_props);
        GST_FIXME("muxer-properties: %s", str);
        g_free(str);
        gst_structure_free(mux_props);
    }

    if (depay && parse && sink) {
        GstPad *depay_src_pad = gst_element_get_static_pad(depay, "src");
        GstPad *parse_sink_pad = gst_element_get_static_pad(parse, "sink");
        GstPad *parse_src_pad = gst_element_get_static_pad(parse, "src");
        GstPad *sink_video_pad = gst_element_request_pad_simple(sink, "video");

        GstPadLinkReturn ret1 = gst_pad_link(depay_src_pad, parse_sink_pad);
        GST_FIXME("depayとparseのリンク = %d", ret1);
        GstPadLinkReturn ret2 = gst_pad_link(parse_src_pad, sink_video_pad);
        GST_FIXME("parseとsinkのリンク = %d", ret2);

        gst_object_unref(sink_video_pad);
        gst_object_unref(parse_src_pad);
        gst_object_unref(parse_sink_pad);
        gst_object_unref(depay_src_pad);

        // gboolean is_linked = gst_element_link(depay, parse);
        // GST_FIXME("depayとparseのリンク = %s", is_linked ? "TRUE" : "FALSE");
        // is_linked = gst_element_link(parse, sink);
        // GST_FIXME("parseとsinkのリンク = %s", is_linked ? "TRUE" : "FALSE");

        gst_object_unref(sink);
        gst_object_unref(parse);
        gst_object_unref(depay);
    }

    gst_object_unref(pipeline);
}

static void on_unprepared(GstRTSPMedia *media, gpointer user_data) {
    GST_FIXME("unprepared!!!");

    GstElement *pipeline = gst_rtsp_media_get_element(media);
    // gst_element_send_event(pipeline, gst_event_new_eos());

    // // sink取得（splitmuxsinkのnameを指定）
    // GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "splitmuxsink0");
    // if (sink) {
    //     // GST_FIXME("split-now!!!");
    //     // g_signal_emit_by_name(sink, "split-now");
    //     GstPad *sinkpad = gst_element_get_static_pad(sink, "video");
    //     if (sinkpad) {
    //         GST_FIXME("split-now!!!");
    //         gst_pad_send_event(sinkpad, gst_event_new_eos());
    //         gst_object_unref(sinkpad);
    //     }

    //     gst_object_unref(sink);
    // }

    // element取得（rtph264depayとh264parseとsplitmuxsinkのnameを指定）
    GstElement *depay = gst_bin_get_by_name(GST_BIN(pipeline), "depay0");
    GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "parse0");
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
    if (depay && parse && sink) {
        // GstIterator *it = gst_element_iterate_sink_pads(sink);
        // GValue item = G_VALUE_INIT;
        // while (gst_iterator_next(it, &item) == GST_ITERATOR_OK) {
        //     GstPad *pad = g_value_get_object(&item);
        //     g_print("Pad: %s (%s)\n", GST_PAD_NAME(pad), G_OBJECT_TYPE_NAME(pad));
        //     // gst_pad_send_event(pad, gst_event_new_eos());
        //     g_value_unset(&item);
        // }
        // gst_iterator_free(it);

        // GstPad *video_pad = gst_element_get_static_pad(sink, "video");
        // if (video_pad) {
        //     gst_pad_send_event(video_pad, gst_event_new_eos());
        //     GST_FIXME("Send EOS to video pad!!!");
        //     gst_object_unref(video_pad);
        // }

        // gst_element_unlink(parse, sink);
        // gst_element_unlink(depay, parse);
        // GST_FIXME("unlink!!!");
        GstPad *depay_src_pad = gst_element_get_static_pad(depay, "src");
        GstPad *parse_sink_pad = gst_element_get_static_pad(parse, "sink");
        GstPad *parse_src_pad = gst_element_get_static_pad(parse, "src");
        GstPad *sink_video_pad = gst_element_get_static_pad(sink, "video");

        // gst_pad_send_event(sink_video_pad, gst_event_new_eos());
        // GST_FIXME("sinkのvideo padにEOSを送信!!!");

        gboolean is_unlinked1 = gst_pad_unlink(depay_src_pad, parse_sink_pad);
        GST_FIXME("depayとparseのアンリンク = %s", is_unlinked1 ? "TRUE" : "FALSE");
        gboolean is_unlinked2 = gst_pad_unlink(parse_src_pad, sink_video_pad);
        GST_FIXME("parseとsinkのアンリンク = %s", is_unlinked2 ? "TRUE" : "FALSE");

        gst_element_release_request_pad(sink, sink_video_pad);

        gst_object_unref(sink_video_pad);
        gst_object_unref(parse_src_pad);
        gst_object_unref(parse_sink_pad);
        gst_object_unref(depay_src_pad);

        gst_object_unref(sink);
        gst_object_unref(parse);
        gst_object_unref(depay);
    }

    gst_object_unref(pipeline);
}
*/
static void on_new_stream(GstRTSPMedia *media, GstRTSPStream *object, gpointer user_data) { GST_FIXME("new-stream!!!"); }

static void on_removed_stream(GstRTSPMedia *media, GstRTSPStream *object, gpointer user_data) { GST_FIXME("removed-stream!!!"); }

// configure仮想関数のオーバーライド
static GstRTSPMedia *scl_media_factory_construct(GstRTSPMediaFactory *factory, const GstRTSPUrl *url) {
    GST_FIXME("construct!!!");

    GstRTSPMediaFactoryClass *parent_class = GST_RTSP_MEDIA_FACTORY_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(factory)));
    if (parent_class->construct)
        return parent_class->construct(factory, url);

    return NULL;
}

// create_element仮想関数のオーバーライド
static GstElement *scl_media_factory_create_element(GstRTSPMediaFactory *factory, const GstRTSPUrl *url) {
    GST_FIXME("create_element!!!");

    GstElement *pipeline = gst_pipeline_new("scl-media-pipeline");

    GstElement *depay = gst_element_factory_make("rtph264depay", "depay0");

    GstElement *parse = gst_element_factory_make("h264parse", "parse0");
    g_object_set(parse, "config-interval", 1, NULL);
    g_object_set(parse, "update-timecode", TRUE, NULL);

    GstElement *sink = gst_element_factory_make("splitmuxsink", "sink0");

    GstStructure *mux_props = gst_structure_new("mp4mux", "reserved-moov-update-period", G_TYPE_UINT64, 1000000000, "reserved-max-duration", G_TYPE_UINT64, 8000000000, NULL);
    g_object_set(sink, "muxer-properties", mux_props, NULL);
    gst_structure_free(mux_props);

    g_object_set(sink, "location", "unused.mp4", NULL);
    g_object_set(sink, "max-size-time", (guint64)8000000000, NULL);
    g_object_set(sink, "async-finalize", TRUE, NULL);
    g_object_set(sink, "use-robust-muxing", TRUE, NULL);

    // パイプラインに追加
    gst_bin_add_many(GST_BIN(pipeline), depay, parse, sink, NULL);

    gboolean is_linked = gst_element_link_many(depay, parse, sink, NULL);
    GST_FIXME("gst_element_link_many = %s", is_linked ? "TRUE" : "FALSE");

    // // リクエストパッドにリンク
    // GstPad *video_pad = gst_element_request_pad_simple(sink, "video");
    // // GstPad *audio_pad = gst_element_get_request_pad(sink, "audio_0");

    // GstElement *video_last = parse;
    // // GstElement *audio_last = audio_queue;

    // GstPad *video_src_pad = gst_element_get_static_pad(video_last, "src");
    // // GstPad *audio_src_pad = gst_element_get_static_pad(audio_last, "src");

    // gst_pad_link(video_src_pad, video_pad);
    // // gst_pad_link(audio_src_pad, audio_pad);

    // gst_object_unref(video_pad);
    // // gst_object_unref(audio_pad);
    // gst_object_unref(video_src_pad);
    // // gst_object_unref(audio_src_pad);

    return pipeline;
    // return gst_parse_launch("rtph264depay name=depay0 ! h264parse ! splitmuxsink location=unused.mp4 max-size-time=5000000000 async-finalize=true use-robust-muxing=true
    // muxer-properties={mp4mux,reserved-moov-update-period=1000000000,reserved-max-duration=8000000000}", NULL);
}

// create_pipeline仮想関数のオーバーライド
static GstElement *scl_media_factory_create_pipeline(GstRTSPMediaFactory *factory, GstRTSPMedia *media) {
    GST_FIXME("create_pipeline!!!");

    GstRTSPMediaFactoryClass *parent_class = GST_RTSP_MEDIA_FACTORY_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(factory)));
    if (parent_class->create_pipeline)
        return parent_class->create_pipeline(factory, media);

    return NULL;
}

// configure仮想関数のオーバーライド
static void scl_media_factory_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media) {
    GST_FIXME("configure!!!");

    g_signal_connect(media, "new-stream", G_CALLBACK(on_new_stream), NULL);
    // g_signal_connect(media, "prepared", G_CALLBACK(on_prepared), NULL);
    // g_signal_connect(media, "unprepared", G_CALLBACK(on_unprepared), NULL);
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
