// File: main.c
// Date: 2024-09-01

#include <gtk/gtk.h>

#define WINDOW_SIZE_WIDTH  300
#define WINDOW_SIZE_HEIGHT 400
#define INDENTATION 10

#define BUTTON_LABEL_0 "0"
#define BUTTON_LABEL_1 "1"
#define BUTTON_LABEL_2 "2"
#define BUTTON_LABEL_3 "3"
#define BUTTON_LABEL_4 "4"
#define BUTTON_LABEL_5 "5"
#define BUTTON_LABEL_6 "6"
#define BUTTON_LABEL_7 "7"
#define BUTTON_LABEL_8 "8"
#define BUTTON_LABEL_9 "9"

#define BUTTON_LABEL_PLUS "+"
#define BUTTON_LABEL_MULTIPLICATION "*"
#define BUTTON_LABEL_MINUS "-"
#define BUTTON_LABEL_DIVISION "/"
#define BUTTON_LABEL_EQUALS "="

#define BUTTON_LABEL_LEFT_PARETHENSIS "("
#define BUTTON_LABEL_RIGHT_PARETHENSIS ")"

#define BUTTON_LABEL_POINT "."

struct
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *box;
    GtkWidget *entry;
    GtkApplication *app;
}App;

struct
{
    GtkWidget *numpad[10];
    GtkWidget *plus;
    GtkWidget *multiplication;
    GtkWidget *minus;
    GtkWidget *division;
    GtkWidget *equals;
    GtkWidget *left_parenthesis;
    GtkWidget *right_parenthesis;
    GtkWidget *point;
}Buttons;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static double evaluateExpression(const gchar *expression);

static double compute(double a, double b, char op)
{
    switch (op)
    {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    default: return 0.0;
    }
}

static double parseExpression(const gchar *expression, int *endPos)
{
    double result = 0.0;
    char op = '+';
    double current = 0.0;
    int i = 0;
    
    while (expression[i] != '\0')
    {
        if (expression[i] == '(')
        {
            int startPos = i + 1;
            int count = 1;
            while (expression[i] != '\0' && count > 0)
            {
                i++;
                if (expression[i] == '(') count++;
                if (expression[i] == ')') count--;
            }
            if (count == 0)
            {
                gchar *subExpr = g_strndup(&expression[startPos], i - startPos);
                current = parseExpression(subExpr, &i);
                g_free(subExpr);
            }
        }
        else if (expression[i] >= '0' && expression[i] <= '9')
        {
            current = strtod(&expression[i], NULL);
            while (expression[i] >= '0' && expression[i] <= '9') i++;
            i--;
        }
        else if (strchr("+-*/", expression[i]) != NULL)
        {
            result = compute(result, current, op);
            op = expression[i];
            current = 0.0;
        }
        i++;
    }

    result = compute(result, current, op);
    if (endPos != NULL) *endPos = i;
    return result;
}

static double evaluateExpression(const gchar *expression)
{
    int endPos = 0;
    return parseExpression(expression, &endPos);
}


static void buttonAction(GtkWidget *widget, gpointer data)
{
    const gchar *label = (const gchar *)data;

    if (strcmp(label, "=") == 0)
    {
        const gchar *expression = gtk_editable_get_text(GTK_EDITABLE(App.entry));
        double result = evaluateExpression(expression);

        gchar resultStr[64];
        snprintf(resultStr, sizeof(resultStr), "%f", result);
        gtk_editable_set_text(GTK_EDITABLE(App.entry), resultStr);
    }
    else
    {
        const gchar *currentText = gtk_editable_get_text(GTK_EDITABLE(App.entry));
        gchar *newText = g_strconcat(currentText, label, NULL);
        gtk_editable_set_text(GTK_EDITABLE(App.entry), newText);
        g_free(newText);
    }
}


static void createButtons(GtkWidget *grid)
{
    const gchar *button_num_labels[10] =
    {
        BUTTON_LABEL_0,
        BUTTON_LABEL_1,
        BUTTON_LABEL_2,
        BUTTON_LABEL_3,
        BUTTON_LABEL_4,
        BUTTON_LABEL_5,
        BUTTON_LABEL_6,
        BUTTON_LABEL_7,
        BUTTON_LABEL_8,
        BUTTON_LABEL_9
    };

    for (int i = 0; i < 10; ++i)
    {
        Buttons.numpad[i] = gtk_button_new_with_label(button_num_labels[i]);
        g_signal_connect(Buttons.numpad[i], "clicked", G_CALLBACK(buttonAction), (gpointer)button_num_labels[i]);
    }

    Buttons.plus = gtk_button_new_with_label(BUTTON_LABEL_PLUS);
    Buttons.multiplication = gtk_button_new_with_label(BUTTON_LABEL_MULTIPLICATION);
    Buttons.minus = gtk_button_new_with_label(BUTTON_LABEL_MINUS);
    Buttons.division = gtk_button_new_with_label(BUTTON_LABEL_DIVISION);
    Buttons.equals = gtk_button_new_with_label(BUTTON_LABEL_EQUALS);
    Buttons.left_parenthesis = gtk_button_new_with_label(BUTTON_LABEL_LEFT_PARETHENSIS);
    Buttons.right_parenthesis = gtk_button_new_with_label(BUTTON_LABEL_RIGHT_PARETHENSIS);
    Buttons.point = gtk_button_new_with_label(BUTTON_LABEL_POINT);

    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[1], 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[2], 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[3], 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[4], 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[5], 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[6], 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[7], 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[8], 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[9], 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.numpad[0], 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.plus, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.minus, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.multiplication, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.division, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.equals, 4, 2, 1, 2);
    gtk_grid_attach(GTK_GRID(grid), Buttons.left_parenthesis, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.right_parenthesis, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Buttons.point, 2, 3, 1, 1);

    g_signal_connect(Buttons.plus, "clicked", G_CALLBACK(buttonAction), "+");
    g_signal_connect(Buttons.multiplication, "clicked", G_CALLBACK(buttonAction), "*");
    g_signal_connect(Buttons.minus, "clicked", G_CALLBACK(buttonAction), "-");
    g_signal_connect(Buttons.division, "clicked", G_CALLBACK(buttonAction), "/");
    g_signal_connect(Buttons.equals, "clicked", G_CALLBACK(buttonAction), "=");
    g_signal_connect(Buttons.left_parenthesis, "clicked", G_CALLBACK(buttonAction), "(");
    g_signal_connect(Buttons.right_parenthesis, "clicked", G_CALLBACK(buttonAction), ")");
    g_signal_connect(Buttons.point, "clicked", G_CALLBACK(buttonAction), ".");
}

static void createLayout()
{
    App.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, INDENTATION);
    gtk_window_set_child(GTK_WINDOW(App.window), App.box);
    gtk_widget_set_margin_start(App.box, INDENTATION);
    gtk_widget_set_margin_end(App.box, INDENTATION);
    gtk_widget_set_margin_top(App.box, INDENTATION);
    gtk_widget_set_margin_bottom(App.box, INDENTATION);

    App.entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(App.box), App.entry);
    gtk_widget_set_size_request(App.entry, -1, 60);
    gtk_entry_set_placeholder_text(GTK_ENTRY(App.entry), "Operation");

    App.grid = gtk_grid_new();
    gtk_box_append(GTK_BOX(App.box), App.grid);
    gtk_grid_set_row_spacing(GTK_GRID(App.grid), INDENTATION);
    gtk_grid_set_column_spacing(GTK_GRID(App.grid), INDENTATION);

    gtk_widget_set_hexpand(App.grid, TRUE);
    gtk_widget_set_vexpand(App.grid, TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(App.grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(App.grid), TRUE);

    createButtons(App.grid);
}

static void window(GtkWidget *widget, gpointer data)
{
    App.window = gtk_application_window_new(App.app);
    createLayout();
    gtk_window_set_title(GTK_WINDOW(App.window), "Calculator");
    gtk_window_set_resizable(GTK_WINDOW(App.window), FALSE);
    gtk_window_present(GTK_WINDOW(App.window));
}

int main(int argc, char **argv)
{
    App.app = gtk_application_new("org.oddyst.text", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(App.app, "activate", G_CALLBACK(window), NULL);
    int status = g_application_run(G_APPLICATION(App.app), argc, argv);
    g_object_unref(App.app);
    return status;
}
