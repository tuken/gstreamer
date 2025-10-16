#include "scl-media.h"

typedef struct _SclMediaPrivate {
    gboolean has_video;
    gboolean has_audio;
} SclMediaPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(SclMedia, scl_media, GST_TYPE_RTSP_MEDIA)

GST_DEBUG_CATEGORY_STATIC(scl_media);

// prepare仮想関数のオーバーライド
static gboolean scl_media_prepare(GstRTSPMedia *media, GstRTSPThread *thread) {
    GST_FIXME("prepare!!!");

    GstRTSPMediaClass *parent_class = GST_RTSP_MEDIA_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(media)));
    if (parent_class->prepare)
        if (!parent_class->prepare(media, thread))
            return FALSE;

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
    GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "h264parse0");
    GST_FIXME("parse pointer: %p", parse);
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "splitmuxsink0");
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

    return TRUE;
}

// unprepare仮想関数のオーバーライド
static gboolean scl_media_unprepare(GstRTSPMedia *media) {
    GST_FIXME("unprepare!!!");

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
    GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "h264parse0");
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "splitmuxsink0");
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

        gst_pad_send_event(sink_video_pad, gst_event_new_eos());
        GST_FIXME("sinkのvideo padにEOSを送信!!!");

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

    GstRTSPMediaClass *parent_class = GST_RTSP_MEDIA_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(media)));
    if (parent_class->unprepare)
        return parent_class->unprepare(media);

    return FALSE;
}

gboolean scl_media_video_link(GstRTSPMedia *media) {
    GstElement *pipeline = gst_rtsp_media_get_element(media);

    GstElement *depay = gst_bin_get_by_name(GST_BIN(pipeline), "depay0");
    GST_CAT_FIXME(scl_media, "depay pointer: %p", depay);
    if (depay) {
        gst_bin_remove(GST_BIN(pipeline), depay);
    }

    depay = gst_element_factory_make("rtph264depay", "depay0");
    gst_bin_add(GST_BIN(pipeline), depay);
    GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "parse0");
    GST_CAT_FIXME(scl_media, "parse pointer: %p", parse);
    GstElement *queue = gst_bin_get_by_name(GST_BIN(pipeline), "queue0");
    GST_CAT_FIXME(scl_media, "queue0 pointer: %p", queue);
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
    GST_CAT_FIXME(scl_media, "sink pointer: %p", sink);

    if (depay && parse && queue && sink) {
        GstPad *depay_src_pad = gst_element_get_static_pad(depay, "src");
        GstPad *parse_sink_pad = gst_element_get_static_pad(parse, "sink");
        GstPad *parse_src_pad = gst_element_get_static_pad(parse, "src");
        GstPad *queue_sink_pad = gst_element_get_static_pad(queue, "sink");
        GstPad *queue_src_pad = gst_element_get_static_pad(queue, "src");
        GstPad *sink_video_pad = gst_element_request_pad_simple(sink, "video");

        GstPadLinkReturn ret01 = gst_pad_link(depay_src_pad, parse_sink_pad);
        GST_CAT_FIXME(scl_media, "depayとparseのリンク = %d", ret01);
        GstPadLinkReturn ret02 = gst_pad_link(parse_src_pad, queue_sink_pad);
        GST_CAT_FIXME(scl_media, "parseとqueueのリンク = %d", ret02);
        GstPadLinkReturn ret03 = gst_pad_link(queue_src_pad, sink_video_pad);
        GST_CAT_FIXME(scl_media, "queueとsinkのリンク = %d", ret03);

        gst_object_unref(sink_video_pad);
        gst_object_unref(queue_src_pad);
        gst_object_unref(queue_sink_pad);
        gst_object_unref(parse_src_pad);
        gst_object_unref(parse_sink_pad);
        gst_object_unref(depay_src_pad);

        gst_object_unref(sink);
        gst_object_unref(queue);
        gst_object_unref(parse);
        gst_object_unref(depay);
    } else {
        return FALSE;
    }

    gst_object_unref(pipeline);

    gst_rtsp_media_collect_streams(media);

    return TRUE;
}

static gboolean scl_media_handle_sdp(GstRTSPMedia *media, GstSDPMessage *sdp) {
    GST_CAT_FIXME(scl_media, "handle_sdp called");

    GstRTSPMediaClass *parent_class = GST_RTSP_MEDIA_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(media)));
    gboolean parent_result = TRUE;
    if (parent_class->handle_sdp)
        parent_result = parent_class->handle_sdp(media, sdp);
    if (!parent_result)
        return FALSE;

    SclMediaPrivate *priv = scl_media_get_instance_private((SclMedia *)media);
    priv->has_video = FALSE;
    priv->has_audio = FALSE;

    guint media_msg_count = gst_sdp_message_medias_len(sdp);
    for (guint i = 0; i < media_msg_count; i++) {
        const GstSDPMedia *media_msg = gst_sdp_message_get_media(sdp, i);
        if (media_msg) {
            const gchar *desc = gst_sdp_media_get_media(media_msg);
            if (g_strcmp0(desc, "video") == 0)
                priv->has_video = TRUE;
            if (g_strcmp0(desc, "audio") == 0)
                priv->has_audio = TRUE;
        }
    }

    return TRUE;
}

// preparedシグナル用コールバック
static void on_prepared(GstRTSPMedia *media, gpointer user_data) {
    GST_CAT_FIXME(scl_media, "on_prepared called");

    SclMediaPrivate *priv = scl_media_get_instance_private((SclMedia *)media);
    GstElement *pipeline = gst_rtsp_media_get_element(media);

    if (priv->has_video) {
        GstElement *depay = gst_bin_get_by_name(GST_BIN(pipeline), "depay0");
        GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "parse0");
        GstElement *queue = gst_bin_get_by_name(GST_BIN(pipeline), "queue0");
        GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
        if (depay && parse && queue && sink) {
            GstPad *depay_src_pad = gst_element_get_static_pad(depay, "src");
            GstPad *parse_sink_pad = gst_element_get_static_pad(parse, "sink");
            GstPad *parse_src_pad = gst_element_get_static_pad(parse, "src");
            GstPad *queue_sink_pad = gst_element_get_static_pad(queue, "sink");
            GstPad *queue_src_pad = gst_element_get_static_pad(queue, "src");
            GstPad *sink_video_pad = gst_element_request_pad_simple(sink, "video");

            gst_pad_link(depay_src_pad, parse_sink_pad);
            gst_pad_link(parse_src_pad, queue_sink_pad);
            gst_pad_link(queue_src_pad, sink_video_pad);

            gst_object_unref(sink_video_pad);
            gst_object_unref(queue_src_pad);
            gst_object_unref(queue_sink_pad);
            gst_object_unref(parse_src_pad);
            gst_object_unref(parse_sink_pad);
            gst_object_unref(depay_src_pad);
        }
    }
    // audioも同様に必要ならリンク
    if (priv->has_audio) {
        GstElement *depay = gst_bin_get_by_name(GST_BIN(pipeline), "audio");
        GstElement *parse = gst_bin_get_by_name(GST_BIN(pipeline), "parse1");
        GstElement *queue = gst_bin_get_by_name(GST_BIN(pipeline), "queue1");
        GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink0");
        if (depay && parse && queue && sink) {
            GstPad *depay_src_pad = gst_element_get_static_pad(depay, "src");
            GstPad *parse_sink_pad = gst_element_get_static_pad(parse, "sink");
            GstPad *parse_src_pad = gst_element_get_static_pad(parse, "src");
            GstPad *queue_sink_pad = gst_element_get_static_pad(queue, "sink");
            GstPad *queue_src_pad = gst_element_get_static_pad(queue, "src");
            GstPad *sink_audio_pad = gst_element_request_pad_simple(sink, "audio_0");

            gst_pad_link(depay_src_pad, parse_sink_pad);
            gst_pad_link(parse_src_pad, queue_sink_pad);
            gst_pad_link(queue_src_pad, sink_audio_pad);

            gst_object_unref(sink_audio_pad);
            gst_object_unref(queue_src_pad);
            gst_object_unref(queue_sink_pad);
            gst_object_unref(parse_src_pad);
            gst_object_unref(parse_sink_pad);
            gst_object_unref(depay_src_pad);
        }
    }
    gst_object_unref(pipeline);
}

static void scl_media_init(SclMedia *media) { g_signal_connect(media, "prepared", G_CALLBACK(on_prepared), NULL); }

static void scl_media_class_init(SclMediaClass *klass) {
    GstRTSPMediaClass *factory_class = GST_RTSP_MEDIA_CLASS(klass);
    // factory_class->prepare = scl_media_prepare;
    // factory_class->unprepare = scl_media_unprepare;
    factory_class->handle_sdp = scl_media_handle_sdp;

    GST_DEBUG_CATEGORY_INIT(scl_media, "sclmedia", 0, "SclMedia");
}

// preparedシグナルの接続はfactory/configure等で
// g_signal_connect(media, "prepared", G_CALLBACK(on_prepared), NULL);
