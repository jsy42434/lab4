#include <gtk/gtk.h>

GtkWidget *entry; // 입력 텍스트 상자

// 버튼 클릭 시 호출되는 콜백 함수
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current_text = gtk_entry_get_text(GTK_ENTRY(entry));
    char new_text[256];

    if (strcmp(button_label, "=") == 0) {
        // 수식을 계산
        double result = 0.0;
        if (sscanf(current_text, "%lf", &result) == 1) {
            snprintf(new_text, sizeof(new_text), "%.2lf", result);
        } else {
            snprintf(new_text, sizeof(new_text), "Error");
        }
        gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    } else if (strcmp(button_label, "C") == 0) {
        // 입력 초기화
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else {
        // 숫자 또는 연산자 추가
        snprintf(new_text, sizeof(new_text), "%s%s", current_text, button_label);
        gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *buttons[16];
    const char *labels[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "C", "0", "=", "+"
    };

    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK 계산기");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 종료 버튼 동작
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 그리드 레이아웃 생성
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 입력 텍스트 상자 생성
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0); // 오른쪽 정렬
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // 사용자가 직접 입력 불가
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // 버튼 생성 및 추가
    for (int i = 0; i < 16; i++) {
        buttons[i] = gtk_button_new_with_label(labels[i]);
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(on_button_clicked), NULL);
        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 4, (i / 4) + 1, 1, 1);
    }

    // 윈도우 표시
    gtk_widget_show_all(window);

    // GTK 메인 루프 실행
    gtk_main();

    return 0;
}
