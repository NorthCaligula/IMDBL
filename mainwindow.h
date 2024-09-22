#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase> //БД
#include <QtSql/QSqlError>    //Ошибки
#include <QtSql/QSqlQuery>    //Запросы
#include <QMessageBox>
#include <QTableWidgetItem>
#include <math.h>
#include <QDebug>
#include <QPushButton>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_show_r_r_clicked();
    void on_pushButton_registr_clicked();
    void on_pushButton_log_in_clicked();
    void on_pushButton_Entering_clicked();
    void on_radioButton_adm_clicked();
    void on_radioButton_usr_clicked();
    void log_in();
    void zaloginino();
    void zaloginino_extra(bool show);
    void on_pushButton_add_rr_to_usr_clicked();
    void on_pushButton_search_clicked();
    void content_checked();
    void on_pushButton_save_clicked();
    void on_radioButton_rating_clicked();
    void on_radioButton_review_clicked();
    void content_chose();
    void on_pushButton_cancel_clicked();
    void proverka_r_r();
    void content_chic();
    void on_pushButton_team_clicked();
    void edit_team();
    void delete_person();
    void chose_person();
    void add_person();
    void new_person();
    void buuton_cancel();
    void delete_rr_user();
    void add_content();
    void delete_content();
    void content_censor();
    void content_censor_user();

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlQuery qw;
    QPushButton *pb_cancel_fantom; // Используем для отмены при редактировании съмочной команды
    QPushButton *pb_delete_rr_user;// Используем чтобы удалить оценку/отзыв от лица пользователя

    int id_log_in = 0;// 0 - Нет входа в ак; 1 - процесс входа в ак польз.; 2 - процесс регистр. в ак польз.; 3 - процесс входа в ак админ.;
                   // 6 - Активный акаунт пользователя;  7 - Активный акаунт администратора.

    int id_create = 0;// 0 - Нет добавления съемочной команды; 1 - процесс удаления/добавления члена команды;
                      // 2 - добавление в фильм уже созданного человека со своей ролью; 3 - создание нового человека


    //Здесь запишем id и логин активного пользователя
    int id_user_global = -1;
    QString login_global = "NULLPTR";

    //Индекс для выбора контента из таблицы
    int id_content_table = 0;
    int id_member = 0;

    bool flag_gl_zalog = false;// Флаг для включения и выключения интерфейса где false - off а true - on

    //Добавляю флаги для наличия/отсутсвия оценки/рецензия
    bool flag_rating = false, flag_review = false;

    bool flag_globb = false; //флаг для понимания изменяем мы характеристики о фильме, или создаем новый
    //false - обновляем; true - создаем

};

#endif // MAINWINDOW_H
