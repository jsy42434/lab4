#include <gtk/gtk.h>

// 버튼 클릭 시 호출되는 콜백 함수
void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hello, GUI! 버튼이 클릭되었습니다!\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    // GTK 초기화
    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ 예제");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 윈도우 종료 버튼 동작
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 버튼 생성
    button = gtk_button_new_with_label("Hello, GUI!");
    gtk_container_add(GTK_CONTAINER(window), button);

    // 버튼 클릭 이벤트 연결
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // 윈도우 표시
    gtk_widget_show_all(window);

    // GTK 메인 루프 실행
    gtk_main();

    return 0;
}

