#include <gtk/gtk.h>
#include <stdlib.h>

// 입력 필드와 결과 레이블
GtkWidget *entry1, *entry2, *result_label;

// 연산 함수
void on_calculate(GtkWidget *widget, gpointer data) {
    const char *op = (const char *)data;
    const char *text1 = gtk_entry_get_text(GTK_ENTRY(entry1));
    const char *text2 = gtk_entry_get_text(GTK_ENTRY(entry2));

    double num1 = atof(text1);
    double num2 = atof(text2);
    double result = 0.0;
    char result_text[100];

    if (g_strcmp0(op, "+") == 0) {
        result = num1 + num2;
    } else if (g_strcmp0(op, "-") == 0) {
        result = num1 - num2;
    } else if (g_strcmp0(op, "*") == 0) {
        result = num1 * num2;
    } else if (g_strcmp0(op, "/") == 0) {
        if (num2 != 0) {
            result = num1 / num2;
        } else {
            gtk_label_set_text(GTK_LABEL(result_label), "Error: Division by zero");
            return;
        }
    }

    snprintf(result_text, sizeof(result_text), "Result: %.2f", result);
    gtk_label_set_text(GTK_LABEL(result_label), result_text);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *add_button, *sub_button, *mul_button, *div_button;

    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 그리드 레이아웃 생성
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 입력 필드
    entry1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry1, 0, 0, 2, 1);
    entry2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry2, 0, 1, 2, 1);

    // 버튼 생성
    add_button = gtk_button_new_with_label("+");
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 2, 1, 1);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_calculate), (gpointer)"+");

    sub_button = gtk_button_new_with_label("-");
    gtk_grid_attach(GTK_GRID(grid), sub_button, 1, 2, 1, 1);
    g_signal_connect(sub_button, "clicked", G_CALLBACK(on_calculate), (gpointer)"-");

    mul_button = gtk_button_new_with_label("*");
    gtk_grid_attach(GTK_GRID(grid), mul_button, 0, 3, 1, 1);
    g_signal_connect(mul_button, "clicked", G_CALLBACK(on_calculate), (gpointer)"*");

    div_button = gtk_button_new_with_label("/");
    gtk_grid_attach(GTK_GRID(grid), div_button, 1, 3, 1, 1);
    g_signal_connect(div_button, "clicked", G_CALLBACK(on_calculate), (gpointer)"/");

    // 결과 레이블
    result_label = gtk_label_new("Result: ");
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, 4, 2, 1);

    // 윈도우 표시
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
