#include "scl-media-factory.h"

// static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
//     GMainLoop *loop = (GMainLoop *)data;

//     switch (GST_MESSAGE_TYPE(msg)) {
//         case GST_MESSAGE_EOS:
//             GST_FIXME("EOS received!\n");
//             g_main_loop_quit(loop);
//             break;

//         case GST_MESSAGE_ERROR: {
//             GError *err = NULL;
//             gchar *debug = NULL;
//             gst_message_parse_error(msg, &err, &debug);
//             GST_FIXME("Error: %s\n", err->message);
//             if (debug)
//                 GST_FIXME("Debug: %s\n", debug);
//             if (err && strstr(err->message, "Media has more or less streams than SDP")) {
//                 GST_FIXME("Detected SDP/stream mismatch. Restarting RTSP server...\n");
//                 g_main_loop_quit(loop);
//             }
//             g_error_free(err);
//             g_free(debug);
//             break;
//         }

//         default:
//             break;
//     }

//     return TRUE;
// }

// static void on_media_constructed(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data) {
//     GST_FIXME("on_media_constructed!!!");

//     GstElement *pipeline = gst_rtsp_media_get_element(media);
//     GstBus *bus = gst_element_get_bus(pipeline);
//     gst_bus_add_watch(bus, bus_call, user_data);
//     gst_object_unref(bus);
// }

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);
    // void run_rtsp_server() {
    GstRTSPServer *server = gst_rtsp_server_new();

    g_object_set(server, "service", "8554", NULL);

    SclMediaFactory *factory = g_object_new(SCL_TYPE_MEDIA_FACTORY, NULL);
    gst_rtsp_media_factory_set_transport_mode(GST_RTSP_MEDIA_FACTORY(factory), GST_RTSP_TRANSPORT_MODE_RECORD);
    gst_rtsp_media_factory_set_latency(GST_RTSP_MEDIA_FACTORY(factory), 2000);

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
    gst_rtsp_mount_points_add_factory(mounts, "/test", GST_RTSP_MEDIA_FACTORY(factory));
    g_object_unref(mounts);

    gst_rtsp_server_attach(server, NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    // g_signal_connect(factory, "media-constructed", G_CALLBACK(on_media_constructed), loop);

    g_main_loop_run(loop);

    g_main_loop_unref(loop);

    g_object_unref(server);

    return 0;
}

// int main(int argc, char *argv[]) {
//     gst_init(&argc, &argv);

//     while (1) {
//         run_rtsp_server();
//         GST_FIXME("Restarting RTSP server after error...\n");
//         // 必要ならsleepやリソース解放処理を追加
//     }
//     return 0;
// }
