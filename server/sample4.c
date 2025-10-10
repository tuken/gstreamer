#include "scl-media-factory.h"

// static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
//     switch (GST_MESSAGE_TYPE(msg)) {
//         case GST_MESSAGE_EOS:
//             g_print("EOS received!\n");
//             if (data)
//                 g_main_loop_quit((GMainLoop *)data);
//             break;

//         case GST_MESSAGE_ERROR:
//             g_print("Error occurred!!!!\n");
//             if (data)
//                 g_main_loop_quit((GMainLoop *)data);
//             break;

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
// }

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

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