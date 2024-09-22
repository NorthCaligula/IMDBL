#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) : //Конструтктор
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = nullptr;

    if(db != nullptr)
    {
        db->close();
        QMessageBox::information(this,"Ошибка","База данных не подключена",QMessageBox::Ok);
        return;
    }
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    //        QString dbName("Imdbl");//Старый
    QString dbName("IMDBl_new");//Новый
    QString host("127.0.0.1");
    QString usr("postgres");
    QString psw("changeme");
    db->setDatabaseName(dbName);
    db->setHostName(host);
    db->setUserName(usr);
    db->setPassword(psw);
    db->setPort(5432);
    if(!db->open())
    {
        QMessageBox::warning(this,"Error",db->lastError().text());
        delete db;
        db = nullptr;
    }
    qw = QSqlQuery(*db);
    ui->label_login->hide();
    ui->lineEdit_login->hide();
    ui->label_password->hide();
    ui->lineEdit_password->hide();
    ui->pushButton_Entering->hide();
    ui->pushButton_registr->hide();
    ui->radioButton_usr->hide();
    ui->radioButton_adm->hide();
    ui->label_email->hide();
    ui->lineEdit_email->hide();
    ui->label_gender->hide();
    ui->comboBox_gender->hide();
    ui->textEdit_name_content->setReadOnly(true);

    ui->pushButton_show_r_r->hide();
    ui->tableWidget_content->hide();

    ui->pushButton_add_rr_to_usr->hide();
    ui->pushButton_search->hide();

    ui->groupBox_rating_review->hide();
    ui->groupBox_content->hide();

    pb_cancel_fantom = new QPushButton(this);
    pb_cancel_fantom->setText("Назад");
    QFont font;
    font.setPointSize(16);
    pb_cancel_fantom->setFont(font);
    pb_cancel_fantom->setGeometry(230,450,121,61);
    connect(pb_cancel_fantom,SIGNAL(clicked()),this,SLOT(buuton_cancel()));
    pb_cancel_fantom->hide();

    pb_delete_rr_user = new QPushButton(this);
    pb_delete_rr_user->setText("Удалить");
    pb_delete_rr_user->setFont(font);
    pb_delete_rr_user->setGeometry(80,420,121,61);
    connect(pb_delete_rr_user,SIGNAL(clicked()),this,SLOT(delete_rr_user()));
    pb_delete_rr_user->hide();

    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",QRegularExpression::CaseInsensitiveOption);
    ui->lineEdit_email->setValidator(new QRegularExpressionValidator(rx,this));
}

MainWindow::~MainWindow() //Дестуктор
{
    delete ui;
}

void MainWindow::on_pushButton_Entering_clicked() //Кнопка входа
{
    switch (id_log_in) {
    case 1:
    {
        QString login = "LOGINNULL";
        QString password = "PASSWORDNULL";
        bool flag_entering_login=false;
        bool flag_entering_password=false;
        qw = db->exec("SELECT id_user, login, password FROM \"user\"");// Запрос логина и пароля

        while (qw.next())
        {
            login = qw.value(1).toString();
            password = qw.value(2).toString();
            if(login==ui->lineEdit_login->text())
            {
                flag_entering_login=true;
                if(password==ui->lineEdit_password->text())
                {
                    flag_entering_password=true;
                    login_global = login;
                    id_user_global = qw.value(0).toInt();
                }
            }
        }
        if(flag_entering_login)
        {
            if(flag_entering_password)
            {
                id_log_in = 6;
                log_in();
                zaloginino();

                QMessageBox::information(this,"Информация","Поздравляю, вы вошли как пользователь",QMessageBox::Ok);
            }
            else
                QMessageBox::warning(this,"Ошибка","Неверный пароль",QMessageBox::Ok);
        }
        else {
            QMessageBox::warning(this,"Ошибка","Пользователь не найден, проверьте логин",QMessageBox::Ok);
        }
        break;
    }
    case 2:
    {
        id_log_in = 1;
        log_in();
        break;
    }
    case 3: // Вход админа
    {

        QString login = ui->lineEdit_login->text();
        QString password = ui->lineEdit_password->text();

        if(login == "ADMIN" and password == "ADMIN")
        {
            QMessageBox::information(this,"Информация","Вы вошли как ADMIN");
            id_log_in = 7;
            log_in();
            zaloginino();
        }
        else {
            QMessageBox::information(this,"Ошибка","НЕВЕРНЫЙ ЛОГИН/ПАРОЛЬ");
        }
        break;
    }
    }
}

void MainWindow::on_pushButton_log_in_clicked()//Кнопка "Вход"
{
    if(id_log_in != 1)
    {
        id_log_in = 1; //Ставим индекс для входа в аккаунт
        log_in();
        ui->radioButton_usr->setChecked(true);
        if(flag_gl_zalog)
            zaloginino();
    }
    else
    {
        id_log_in = 0; //Ставим индекс для входа в аккаунт
        log_in();
    }
}

void MainWindow::on_pushButton_registr_clicked()//Кнопка "регистрация"
{
    if(id_log_in == 2)
    {
        if(db->isOpen())
        {
            QString login    = "LOGINNULL";
            QString password = "PASSWORDNULL";
            QString email    = "EMAILNULL";
            int gender       = 0;

            login    = ui->lineEdit_login->text().trimmed();
            password = ui->lineEdit_password->text().trimmed();
            email    = ui->lineEdit_email->text().trimmed();
            gender   = ui->comboBox_gender->currentIndex();

            if(!ui->lineEdit_email->hasAcceptableInput())
            {
                QMessageBox::warning(this,"Ошибка регистрации","Поменяйте email");
                return;
            }
            qw.prepare("INSERT INTO public.user (login, password, email, gender) "
                       "VALUES (:login, :password, :email, :gender)");
            qw.bindValue(":login", login);
            qw.bindValue(":password", password);
            qw.bindValue(":email", email);
            qw.bindValue(":gender", gender);
            if (!qw.exec())

                QMessageBox::information(this,"Нет","Данные не добавлены\n" + qw.lastError().text().split("DETAIL")[1].insert(0, "DETAIL: ").split(".")[0],QMessageBox::Ok);
            else
            {
                QMessageBox::information(this,"Ура","Данные добавлены",QMessageBox::Ok);
                id_log_in = 1;
                log_in();
            }
        }
        else
        {
            QMessageBox::warning(this,"Нет","База данных не открыта",QMessageBox::Ok);
        }
    }
    else
    {
        id_log_in = 2;// Ставим индекс для регистрации нового пользователя
        log_in();
    }
}

void MainWindow::on_radioButton_usr_clicked()//Радио баттон для входа в качестве юзера
{
    id_log_in = 1; //Ставим индекс для входа в аккаунт
    ui->spinBox_awards->setReadOnly(true);
    ui->textEdit_description->setReadOnly(true);
    log_in();
}

void MainWindow::on_radioButton_adm_clicked()// Радио баттон для входа в качесвте админа
{
    id_log_in = 3; //Ставим индекс для админа
    ui->spinBox_awards->setReadOnly(false);
    ui->textEdit_description->setReadOnly(false);
    log_in();
}

void MainWindow::log_in()//Функция для отображения кнопок при входе
{
    if(id_log_in == 0 || id_log_in == 6 ||id_log_in == 7)
    {
        ui->label_login->hide();
        ui->lineEdit_login->hide();
        ui->label_password->hide();
        ui->lineEdit_password->hide();
        ui->pushButton_Entering->hide();
        ui->pushButton_registr->hide();
        ui->radioButton_usr->hide();
        ui->radioButton_adm->hide();
        ui->label_email->hide();
        ui->lineEdit_email->hide();
        ui->label_gender->hide();
        ui->comboBox_gender->hide();

    }
    switch (id_log_in) {
    case 0:// Никуда не входим -> :-) <-
    {
        ui->pushButton_log_in->setText("Войти");

        ui->lineEdit_login->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_email->clear();
        break;
    }
    case 1:// Вход в акк польз
    {
        ui->label_login->show();
        ui->lineEdit_login->show();
        ui->label_password->show();
        ui->lineEdit_password->show();
        ui->pushButton_Entering->show();
        ui->pushButton_registr->show();
        ui->radioButton_usr->show();
        ui->radioButton_adm->show();
        ui->lineEdit_login->setPlaceholderText("Логин");
        ui->lineEdit_password->setPlaceholderText("Пароль");
        ui->lineEdit_email->setPlaceholderText("Email");

        ui->label_email->hide();
        ui->lineEdit_email->hide();
        ui->label_gender->hide();
        ui->comboBox_gender->hide();

        ui->lineEdit_login->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_email->clear();
        ui->comboBox_gender->setCurrentIndex(0);

        //делаем "красоту" с расположением кнопок
        QPoint point;
        point=ui->pushButton_registr->pos();
        point.setY(410);
        ui->pushButton_registr->move(point);
        point=ui->pushButton_Entering->pos();
        point.setY(360);
        ui->pushButton_Entering->move(point);
        ui->pushButton_Entering->setText("Войти");
        ui->pushButton_log_in->setText("Назад");
        break;
    }
    case 2:// Процесс регистрации
    {
        ui->label_email->show();
        ui->lineEdit_email->show();
        ui->label_gender->show();
        ui->comboBox_gender->show();

        ui->lineEdit_login->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_email->clear();
        ui->comboBox_gender->setCurrentIndex(0);

        //делаем "красоту" с расположением кнопок
        QPoint point;
        point=ui->pushButton_registr->pos();
        point.setY(490);
        ui->pushButton_registr->move(point);
        point=ui->pushButton_Entering->pos();
        point.setY(540);
        ui->pushButton_Entering->move(point);
        ui->pushButton_Entering->setText("Назад");

        ui->pushButton_log_in->setText("Назад");
        break;
    }
    case 3:// Вход в акк админ
    {
        ui->label_login->show();
        ui->lineEdit_login->show();
        ui->label_password->show();
        ui->lineEdit_password->show();
        ui->pushButton_Entering->show();
        ui->pushButton_registr->hide();
        ui->radioButton_usr->show();
        ui->radioButton_adm->show();
        ui->lineEdit_login->setPlaceholderText("Логин");
        ui->lineEdit_password->setPlaceholderText("Пароль");
        ui->label_email->hide();
        ui->lineEdit_email->hide();
        ui->label_gender->hide();
        ui->comboBox_gender->hide();

        ui->pushButton_log_in->setText("Назад");

        //делаем красоту с расположением кнопок
        QPoint point;

        point=ui->pushButton_Entering->pos();
        point.setY(360);
        ui->pushButton_Entering->move(point);
        ui->pushButton_Entering->setText("Войти");
        break;
    }
    case 6:// Сейчас в акаунте пользователи
    {
        ui->tableWidget_content->hide();
        break;

    }
    case 7:// Сейчас в акаунте администратора
    {
        ui->tableWidget_content->hide();
        break;
    }
    }
}

void MainWindow::zaloginino()// Функция включения главного интерфейса после входа в акаунт
{
    ui->tableWidget_content->hide();
    if(id_log_in==6)
    {
        if(!flag_gl_zalog)
        {
            zaloginino_extra(true);
            ui->pushButton_log_in->setText("User: "+login_global+"\nВыйти");
            ui->pushButton_add_rr_to_usr->setText("Добавить/редактировать\nоценку/отзыв");
            ui->pushButton_show_r_r->setText("Открыть журнал\nсвоих оценок/отызвов");
            ui->pushButton_search->setText("Обзор информации\nпо фильму");
        }
        else
            zaloginino_extra(false);
    }
    else
    {
        if(!flag_gl_zalog)
        {
            zaloginino_extra(true);
            ui->pushButton_log_in->setText("ADMIN\nВыйти");
            ui->pushButton_add_rr_to_usr->setText("Удалить \nоценку/отзыв");
            ui->pushButton_show_r_r->setText("Добавить/редактировать\n хар-ки кино");
            ui->pushButton_search->setText("Добавить/редактировать\nсъемочную команду");
        }
        else
            zaloginino_extra(false);
    }
    flag_gl_zalog = !flag_gl_zalog;
}

void MainWindow::zaloginino_extra(bool show)
{
    if(show)
    {
        ui->pushButton_add_rr_to_usr->show();
        ui->pushButton_show_r_r->show();
        ui->pushButton_search->show();
    }
    else
    {
        ui->pushButton_add_rr_to_usr->hide();
        ui->pushButton_show_r_r->hide();
        ui->pushButton_search->hide();
        ui->groupBox_rating_review->hide();
    }
}

void MainWindow::proverka_r_r() //Функция для подзагрузки уже поставленных оценко/рецензий
{
    if(db->isOpen())
    {
        bool flag = false;
        flag_rating = false;
        flag_review = false;
        int rt=NULL, rw=NULL;

        qw.prepare("SELECT id_rating, id_review FROM \"journal\" WHERE id_user = :id AND id_content = :content");
        qw.bindValue(":id",id_user_global);
        qw.bindValue(":content", id_content_table);
        qw.exec();

        while (qw.next())
        {
            flag = true;
            if(qw.value(0) != NULL)
            {
                flag_rating = true;
                rt = qw.value(0).toInt();
            }
            if(qw.value(1) != NULL)
            {
                flag_review = true;
                rw = qw.value(1).toInt();
            }
        }
        if(flag)
        {
            if(flag_rating) //Если есть уже поставленная оценка
            {
                ui->radioButton_rating->click();
                ui->radioButton_rating->setChecked(true);
                qw = db->exec("SELECT id_rating, rating FROM \"rating\"");//В ковычках пишем наш запрос

                while (qw.next())
                {
                    if(qw.value(0) == rt)
                        ui->spinBox_rating->setValue(qw.value(1).toInt());
                }
                pb_delete_rr_user->show();
            }
            if(flag_review) //Если есть уже написанный отзыв
            {
                qw = db->exec("SELECT id_review, text_review, recomendation FROM \"review\"");//В ковычках пишем наш запрос

                while (qw.next())
                {
                    if(qw.value(0) == rw)
                    {
                        ui->textEdit_review->setText(qw.value(1).toString());
                        if(qw.value(2) != NULL)
                        {
                            if(qw.value(2).toBool())
                                ui->comboBox_rec->setCurrentIndex(1);
                            else
                                ui->comboBox_rec->setCurrentIndex(2);
                        }
                    }
                }
                pb_delete_rr_user->show();
                if(!flag_rating)
                {
                    ui->radioButton_review->click();
                    ui->radioButton_review->setChecked(true);
                    ui->spinBox_rating->setValue(0);
                }
            }
        }
        else
        {
            if(ui->pushButton_search->isChecked())
            {
                ui->spinBox_rating->hide();
                if(ui->radioButton_rating->isChecked())
                    ui->label_rating_review->setText("Нет вашей оценки");
                else
                    ui->label_rating_review->setText("Нет вашей рецензии");
            }
            if(ui->pushButton_show_r_r->isChecked())
                ui->radioButton_rating->click();
            if(ui->pushButton_add_rr_to_usr->isChecked())
            {
                pb_delete_rr_user->hide();
                ui->radioButton_rating->click();
                ui->spinBox_rating->setValue(0);
            }
        }
    }
}

void MainWindow::on_pushButton_add_rr_to_usr_clicked()// Кнопка №1 Кнопка для добавление в БД новых заисей про фильм
{
    if(id_log_in == 6) // Работа в качестве пользователя
    {
        if(ui->pushButton_add_rr_to_usr->isChecked())
        {
            if (db->isOpen())
            {
                ui->pushButton_show_r_r->setEnabled(false);
                ui->pushButton_log_in->setEnabled(false);
                ui->pushButton_search->setEnabled(false);

                ui->tableWidget_content->show();

                qw.exec("SELECT name, id_content FROM \"content\" ORDER BY id_content");//В ковычках пишем наш запрос
                if(qw.size()>0)
                {
                    QStringList title;
                    title.append("Название");
                    title.append("Действие");
                    ui->tableWidget_content->setHorizontalHeaderLabels(title);
                    ui->tableWidget_content->setColumnCount(2);
                    ui->tableWidget_content->setRowCount(qw.size());
                    int num_row = 0;
                    while (qw.next())
                    {

                        QTableWidgetItem *item = new QTableWidgetItem(qw.value(0).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(num_row, 0, item);
                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Выбрать");
                        pb->setProperty("id_content",qw.value(1).toInt());
                        pb->setProperty("name_content",qw.value(0).toString());
                        connect(pb,SIGNAL(clicked()),this,SLOT(content_checked()));
                        ui->tableWidget_content->setCellWidget(num_row, 1, pb);

                        num_row++;
                    }

                    ui->radioButton_rating->show();
                    ui->radioButton_review->show();
                    ui->label_rating_review->show();
                }
                else {
                    ui->pushButton_add_rr_to_usr->click();
                    QMessageBox::information(this,"Внимание","В БД отсутсвуют фильмы)");
                }
            }
        }
        else
        {
            pb_delete_rr_user->hide();
            ui->tableWidget_content->clear();
            ui->tableWidget_content->setRowCount(1);


            pb_cancel_fantom->hide();
            ui->textEdit_name_content->setText("");
            ui->groupBox_rating_review->hide();

            ui->tableWidget_content->hide();

            ui->pushButton_show_r_r->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);
            ui->pushButton_search->setEnabled(true);
        }
    }
    if(id_log_in == 7) // Работа в качестве администратора
    {
        if(ui->pushButton_add_rr_to_usr->isChecked())
        {
            ui->pushButton_search->setEnabled(false);
            ui->pushButton_show_r_r->setEnabled(false);
            ui->pushButton_log_in->setEnabled(false);

            ui->tableWidget_content->show();

            if (db->isOpen())
            {
                ui->pushButton_show_r_r->setEnabled(false);
                ui->pushButton_log_in->setEnabled(false);
                ui->pushButton_search->setEnabled(false);

                qw = db->exec("SELECT name, id_content FROM \"content\" ORDER BY id_content");//В ковычках пишем наш запрос
                if(qw.size()>0)
                {
                    QStringList title;
                    title.append("Название");
                    title.append("Действие");
                    ui->tableWidget_content->setHorizontalHeaderLabels(title);
                    ui->tableWidget_content->setColumnCount(2);
                    ui->tableWidget_content->setRowCount(qw.size());
                    int num_row = 0;
                    while (qw.next())
                    {

                        QTableWidgetItem *item =  new QTableWidgetItem(qw.value(0).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(num_row, 0, item);
                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Выбрать");
                        pb->setProperty("id_content",qw.value(1).toInt());
                        connect(pb,SIGNAL(clicked()),this,SLOT(content_censor()));
                        ui->tableWidget_content->setCellWidget(num_row, 1, pb);

                        num_row++;
                    }
                }
                else {
                    ui->pushButton_add_rr_to_usr->click();
                    QMessageBox::information(this,"Внимание","В БД отсутсвуют фильмы)");
                }
            }
        }
        else
        {
            ui->pushButton_search->setEnabled(true);
            ui->pushButton_show_r_r->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);

            ui->tableWidget_content->clear();
            pb_cancel_fantom->hide();
            ui->tableWidget_content->hide();

            ui->tableWidget_content->setRowCount(1);
            ui->tableWidget_content->setColumnCount(2);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(1,300);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(0,300);
            ui->groupBox_rating_review->hide();
            ui->groupBox_content->hide();
        }
    }
}

void MainWindow::content_checked() // Выбираем фильм для написание отзыва / оценки
{
    ui->groupBox_rating_review->show();
    ui->groupBox_rating_review->setTitle("Добавление/Просмотр оценки/рецензии");
    int id_content = -1;
    QString name_content= "NULL";

    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    if (pb!=nullptr)
    {
        id_content = pb->property("id_content").toInt();
        name_content = pb->property("name_content").toString();
        ui->textEdit_name_content->setText(name_content);

        id_content_table = id_content;
    }
    else {
        return;
    }
    proverka_r_r();
}

void MainWindow::on_pushButton_show_r_r_clicked()// Кнопка №2
{
    if(id_log_in == 6) // Работа в качестве пользователя
    {
        if(ui->pushButton_show_r_r->isChecked())
        {
            if (db->isOpen())
            {
                ui->pushButton_add_rr_to_usr->setEnabled(false);
                ui->pushButton_log_in->setEnabled(false);
                ui->pushButton_search->setEnabled(false);

                ui->tableWidget_content->show();

                qw.prepare("SELECT name, journal.id_content FROM journal JOIN content ON journal.id_content = content.id_content WHERE id_user = :id ORDER BY id_content ");
                qw.bindValue(":id", id_user_global);
                qw.exec();
                if(qw.size()>0)
                {
                    QStringList title;
                    title.append("Название");
                    title.append("Действие");
                    ui->tableWidget_content->setHorizontalHeaderLabels(title);
                    ui->tableWidget_content->setColumnCount(2);
                    ui->tableWidget_content->setRowCount(qw.size());
                    QString str="ВЫБРАТЬ";
                    qw.first();
                    for (int i = 0; i<qw.size();i++)
                    {
                        QTableWidgetItem *item =   new QTableWidgetItem(qw.value(0).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(i, 0, item);
                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Выбрать");
                        pb->setProperty("id_content",qw.value(1).toInt());
                        pb->setProperty("name_content",qw.value(0).toString());
                        connect(pb,SIGNAL(clicked()),this,SLOT(content_chose()));
                        ui->tableWidget_content->setCellWidget(i, 1, pb);
                        qw.next();
                    }
                }
                else
                {
                    ui->pushButton_show_r_r->click();
                    QMessageBox::information(this,"Внимание","У вас нет записей");
                }
            }
        }
        else
        {
            ui->tableWidget_content->clear();
            ui->tableWidget_content->setRowCount(1);

            ui->textEdit_name_content->setText("");
            ui->groupBox_rating_review->hide();

            ui->tableWidget_content->hide();

            ui->pushButton_add_rr_to_usr->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);
            ui->pushButton_search->setEnabled(true);
            pb_delete_rr_user->hide();
        }
    }
    if(id_log_in == 7) // Работа в качестве админа
    {
        if(ui->pushButton_show_r_r->isChecked())
        {
            ui->pushButton_add_rr_to_usr->setEnabled(false);
            ui->pushButton_search->setEnabled(false);
            ui->pushButton_log_in->setEnabled(false);

            ui->tableWidget_content->show();
            ui->tableWidget_content->setRowCount(0);
            if (db->isOpen())
            {
                qw = db->exec("SELECT id_content, name FROM \"content\" ORDER BY id_content");//В ковычках пишем наш запрос
                qw.first();
                ui->tableWidget_content->setColumnCount(3);

                QStringList title;
                title.append("Название");
                title.append("Действие");
                title.append("Действие удаления");
                ui->tableWidget_content->setHorizontalHeaderLabels(title);
                ui->tableWidget_content->setRowCount(qw.size()+1);
                ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
                ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
                ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

                if(qw.size()>0)
                {
                    qw.first();

                    int i = 0;

                    QTableWidgetItem *item =   new QTableWidgetItem(qw.value(1).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(i, 0, item);

                    QPushButton * pb1 = new QPushButton(this);
                    pb1->setText("Выбрать");
                    pb1->setProperty("id_content",qw.value(0).toInt());
                    pb1->setProperty("name_content",qw.value(1).toString());
                    connect(pb1,SIGNAL(clicked()),this,SLOT(content_chose()));
                    ui->tableWidget_content->setCellWidget(i, 1, pb1);

                    QPushButton * pb = new QPushButton(this);
                    pb->setText("Удалить");
                    pb->setProperty("id",qw.value(0).toInt());
                    connect(pb,SIGNAL(clicked()),this,SLOT(delete_content()));
                    ui->tableWidget_content->setCellWidget(i, 2, pb);

                    while (qw.next())
                    {
                        i++;

                        QTableWidgetItem *item =   new QTableWidgetItem(qw.value(1).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(i, 0, item);

                        QPushButton * pb1 = new QPushButton(this);
                        pb1->setText("Выбрать");
                        pb1->setProperty("id_content",qw.value(0).toInt());
                        pb1->setProperty("name_content",qw.value(1).toString());
                        connect(pb1,SIGNAL(clicked()),this,SLOT(content_chose()));
                        ui->tableWidget_content->setCellWidget(i, 1, pb1);

                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Удалить");
                        pb->setProperty("id",qw.value(0).toInt());
                        connect(pb,SIGNAL(clicked()),this,SLOT(delete_content()));
                        ui->tableWidget_content->setCellWidget(i, 2, pb);
                    }

                    ui->tableWidget_content->setSpan(i+1,0,1,3);
                    QPushButton * pb2 = new QPushButton(this);
                    pb2->setText("Добавить новый фильм");
                    qw.last();
                    pb2->setProperty("id",qw.value(0).toInt()+1);

                    connect(pb2,SIGNAL(clicked()),this,SLOT(add_content()));

                    ui->tableWidget_content->setCellWidget(i+1, 0, pb2);

                }
                else
                {
                    ui->tableWidget_content->setSpan(0,0,1,3);
                    QPushButton * pb2 = new QPushButton(this);
                    pb2->setText("Добавить новый фильм");
                    connect(pb2,SIGNAL(clicked()),this,SLOT(add_content()));
                    ui->tableWidget_content->setCellWidget(0, 0, pb2);
                }
            }
        }
        else
        {
            ui->pushButton_add_rr_to_usr->setEnabled(true);
            ui->pushButton_search->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);


            ui->tableWidget_content->hide();
            pb_delete_rr_user->hide();
            pb_cancel_fantom->hide();

            ui->tableWidget_content->clear();
            ui->tableWidget_content->setRowCount(1);
            ui->tableWidget_content->setColumnCount(2);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(1,300);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(0,300);
            ui->groupBox_rating_review->hide();
            ui->groupBox_content->hide();
        }
    }
}

void MainWindow::content_chose() // Выбираем фильм для просмотра оценок / отзывов
{
    if(id_log_in == 6)
    {
        ui->groupBox_rating_review->show();
        ui->groupBox_rating_review->setTitle("Добавление/Просмотр оценки/рецензии");
        int id_content = -1;
        QString name_content= "NULL";

        QPushButton * pb = qobject_cast<QPushButton *>(sender());
        if (pb!=nullptr)
        {
            on_radioButton_rating_clicked();
            id_content = pb->property("id_content").toInt();
            name_content = pb->property("name_content").toString();
            ui->textEdit_name_content->setText(name_content);

            id_content_table = id_content;

        }
        else {
            return;
        }
        proverka_r_r();

    }
    if(id_log_in == 7)
    {
        QPushButton * pb = qobject_cast<QPushButton *>(sender());
        if (pb!=nullptr)
        {
            id_content_table = pb->property("id_content").toInt();
            ui->textEdit_name_content->setText(pb->property("name_content").toString());
        }

        //Подгрузка кнопок отмены и сохранения
        {
            ui->groupBox_rating_review->setTitle("");
            ui->groupBox_rating_review->show();
            ui->radioButton_rating->hide();
            ui->radioButton_review->hide();
            ui->spinBox_rating->hide();
            ui->label_rating_review->hide();
            ui->label_rec->hide();
            ui->textEdit_review->hide();
            ui->textEdit_name_content->show();
            ui->textEdit_name_content->setReadOnly(false);
            ui->label_name_content->show();
            ui->comboBox_rec->hide();
            ui->pushButton_save->show();
        }

        //Подгрузка описания, оценок и отызвов
        {
            ui->groupBox_content->show();
            ui->groupBox_content->setTitle("Просмотр информации про кинопродукт");
            qw.prepare("SELECT description, awards FROM \"content\" WHERE id_content = :id");
            qw.bindValue(":id", id_content_table);
            qw.exec();

            qw.first();

            ui->textEdit_description->setReadOnly(false);
            ui->textEdit_description->setText(qw.value(0).toString());
            ui->spinBox_awards->setReadOnly(false);
            ui->spinBox_awards->setValue(qw.value(1).toInt());
            if(ui->radioButton_usr->isChecked())
            {
                ui->textEdit_description->setReadOnly(true);
                ui->spinBox_awards->setReadOnly(true);
            }
        }

        //Подгрузка количества оценок, и их среднего значения
        {
            qw.prepare("SELECT COUNT(rating), AVG(rating) FROM \"rating\" WHERE id_content = :id");
            qw.bindValue(":id", id_content_table);
            qw.exec();

            qw.first();

            ui->lineEdit_count_rating->setText(qw.value(0).toString());
            double i = round(qw.value(1).toDouble()*100)/100;
            ui->lineEdit_averege_rating->setText(QString::number(i));

            qw.prepare("SELECT COUNT(text_review), AVG(recomendation::int)*100 FROM \"review\" WHERE id_content = :id");
            qw.bindValue(":id", id_content_table);
            qw.exec();

            qw.first();

            ui->lineEdit_count_review->setText(qw.value(0).toString());
            ui->lineEdit_averege_review->setText(qw.value(1).toString());
        }
    }
}

void MainWindow::on_pushButton_search_clicked()// Кнопка №3 Кнопка для ознакомления с информацией о фильме
{
    if(id_log_in == 6) // Работа в качестве пользователя
    {
        if(ui->pushButton_search->isChecked())
        {
            if (db->isOpen())
            {
                ui->pushButton_show_r_r->setEnabled(false);
                ui->pushButton_add_rr_to_usr->setEnabled(false);
                ui->pushButton_log_in->setEnabled(false);

                ui->tableWidget_content->show();

                qw = db->exec("SELECT name, id_content FROM \"content\" ORDER BY id_content");//В ковычках пишем наш запрос
                if(qw.size()>0)
                {
                    QStringList title;
                    title.append("Название");
                    title.append("Действие");
                    ui->tableWidget_content->setHorizontalHeaderLabels(title);
                    ui->tableWidget_content->setColumnCount(2);
                    ui->tableWidget_content->setRowCount(qw.size());
                    int num_row = 0;
                    QString str="ВЫБРАТЬ";
                    while (qw.next())
                    {

                        QTableWidgetItem *item =   new QTableWidgetItem(qw.value(0).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(num_row, 0, item);
                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Выбрать");
                        pb->setProperty("id_content",qw.value(1).toInt());
                        pb->setProperty("name_content",qw.value(0).toString());
                        connect(pb,SIGNAL(clicked()),this,SLOT(content_chic()));
                        ui->tableWidget_content->setCellWidget(num_row, 1, pb);

                        num_row++;
                    }
                }
                else
                {
                    ui->pushButton_search->click();
                    QMessageBox::information(this,"Внимание","В БД отсутсвуют фильмы)");
                }

            }
        }
        else
        {
            ui->tableWidget_content->clear();
            ui->tableWidget_content->setRowCount(1);

            ui->textEdit_name_content->setText("");
            ui->groupBox_rating_review->hide();
            ui->groupBox_content->hide();

            ui->tableWidget_content->hide();

            ui->pushButton_show_r_r->setEnabled(true);
            ui->pushButton_add_rr_to_usr->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);
        }
    }
    if(id_log_in == 7) // Работа в качестве админа
    {
        if(ui->pushButton_search->isChecked())
        {
            id_create = 0;
            ui->tableWidget_content->clear();

            ui->tableWidget_content->show();

            ui->tableWidget_content->setRowCount(1);
            ui->tableWidget_content->setColumnCount(2);
            ui->pushButton_add_rr_to_usr->setEnabled(false);
            ui->pushButton_show_r_r->setEnabled(false);
            ui->pushButton_log_in->setEnabled(false);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            if (db->isOpen())
            {
                ui->pushButton_show_r_r->setEnabled(false);
                ui->pushButton_add_rr_to_usr->setEnabled(false);
                ui->pushButton_log_in->setEnabled(false);

                qw = db->exec("SELECT name, id_content FROM \"content\" ORDER BY id_content");//В ковычках пишем наш запрос

                if(qw.size()>0)
                {
                    QStringList title;
                    title.append("Название");
                    title.append("Действие");
                    ui->tableWidget_content->setHorizontalHeaderLabels(title);

                    ui->tableWidget_content->setColumnCount(2);
                    ui->tableWidget_content->setRowCount(qw.size());
                    int num_row = 0;
                    QString str="ВЫБРАТЬ";
                    while (qw.next())
                    {

                        QTableWidgetItem *item =    new QTableWidgetItem(qw.value(0).toString());
                        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                        ui->tableWidget_content->setItem(num_row, 0, item);
                        QPushButton * pb = new QPushButton(this);
                        pb->setText("Выбрать");
                        pb->setProperty("id_content",qw.value(1).toInt());
                        pb->setProperty("name_content",qw.value(0).toString());
                        connect(pb,SIGNAL(clicked()),this,SLOT(edit_team()));
                        ui->tableWidget_content->setCellWidget(num_row, 1, pb);

                        num_row++;
                    }
                }
                else
                {
                    ui->pushButton_search->click();
                    QMessageBox::information(this,"Внимание","В БД отсутсвуют фильмы)");
                }
            }
        }
        else
        {
            ui->pushButton_add_rr_to_usr->setEnabled(true);
            ui->pushButton_show_r_r->setEnabled(true);
            ui->pushButton_log_in->setEnabled(true);

            ui->tableWidget_content->hide();

            pb_cancel_fantom->hide();

            ui->tableWidget_content->clear();
            ui->tableWidget_content->setRowCount(1);
            ui->tableWidget_content->setColumnCount(2);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(1,300);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
            ui->tableWidget_content->setColumnWidth(0,300);
            ui->groupBox_rating_review->hide();
        }
    }
}

void MainWindow::content_chic()// Функция для отображения контента
{
    ui->groupBox_rating_review->show();
    ui->groupBox_rating_review->setTitle("Добавление/Просмотр оценки/рецензии");
    int id_content = -1;
    QString name_content= "NULL";

    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    if (pb!=nullptr)
    {
        on_radioButton_rating_clicked();
        id_content = pb->property("id_content").toInt();
        name_content = pb->property("name_content").toString();
        ui->textEdit_name_content->setText(name_content);

        id_content_table = id_content;

    }
    else {
        return;
    }
    proverka_r_r();
    pb_delete_rr_user->hide();

    //Подгрузка описания, оценок и отызвов
    {
        ui->groupBox_content->show();

        ui->label_count_rating->show();
        ui->label_count_review->show();
        ui->label_averege_rating->show();
        ui->label_averege_review->show();

        ui->lineEdit_count_rating->show();
        ui->lineEdit_count_review->show();
        ui->lineEdit_averege_rating->show();
        ui->lineEdit_averege_review->show();

        ui->pushButton_team->show();

        ui->label_name_content->setText("Название фильма:");

        ui->groupBox_content->setTitle("Просмотр информации про кинопродукт");
        qw.prepare("SELECT description, awards FROM \"content\" WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();

        qw.first();

        ui->textEdit_description->setReadOnly(true);
        ui->textEdit_description->setText(qw.value(0).toString());
        ui->spinBox_awards->setReadOnly(true);
        ui->spinBox_awards->setValue(qw.value(1).toInt());
        if(ui->radioButton_usr->isChecked())
        {
            ui->textEdit_description->setReadOnly(true);
            ui->spinBox_awards->setReadOnly(true);
        }
    }
    //Подгрузка количества оценок, и их среднего значения
    {
        qw.prepare("SELECT COUNT(rating), AVG(rating) FROM \"rating\" WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();

        qw.first();

        ui->lineEdit_count_rating->setText(qw.value(0).toString());
        double i = round(qw.value(1).toDouble()*100)/100;
        ui->lineEdit_averege_rating->setText(QString::number(i));

        qw.prepare("SELECT COUNT(text_review), AVG(recomendation::int)*100 FROM \"review\" WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();

        qw.first();


        ui->lineEdit_count_review->setText(qw.value(0).toString());
        ui->lineEdit_averege_review->setText(qw.value(1).toString());
    }
}

void MainWindow::edit_team() // Функция с помощью которой будем добавлять новых режисеров/продюсеров/сценаристов, привязана к 3 кнопке
{
    {//Отображаем в табличке действующих лиц

        QPushButton * pb = qobject_cast<QPushButton *>(sender());
        int idd = id_content_table;
        if (pb!=nullptr)
        {
            on_radioButton_rating_clicked();
            id_content_table = pb->property("id_content").toInt();
        }
        else
            return;
        if(id_create != 0)
            id_content_table = idd;
        id_create = 0;
        ui->tableWidget_content->clear();

        ui->tableWidget_content->setColumnCount(1);
        ui->tableWidget_content->setRowCount(1);

        qw.prepare("SELECT member.name_member, type_member.type_member, member.awards_member, part_member.id_part FROM part_member JOIN member ON member.id_member = part_member.id_member "
                   "JOIN type_member ON type_member.id_type = part_member.id_type WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        if(qw.size()>0)
        {
            ui->tableWidget_content->setColumnCount(4);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            ui->tableWidget_content->setRowCount(qw.size()+1);

            QStringList title;
            title.append("Имя");
            title.append("Роль");
            title.append("Кол-во наград");
            title.append("Действие: удаления");
            ui->tableWidget_content->setHorizontalHeaderLabels(title);

            for (int i=0; qw.next(); i++)
            {
                QPushButton * pb = new QPushButton(this);

                QTableWidgetItem *item =    new QTableWidgetItem(qw.value(0).toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                ui->tableWidget_content->setItem(i, 0, item);

                item = new QTableWidgetItem(qw.value(1).toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                ui->tableWidget_content->setItem(i, 1, item);

                item = new QTableWidgetItem(qw.value(2).toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                ui->tableWidget_content->setItem(i, 2, item);

                pb->setText("Удалить");
                pb->setProperty("id_content",id_content_table);
                pb->setProperty("type_member", qw.value(1).toInt());
                pb->setProperty("id_part",qw.value(3));
                connect(pb,SIGNAL(clicked()),this,SLOT(delete_person()));

                ui->tableWidget_content->setCellWidget(i, 3, pb);
            }
            ui->tableWidget_content->setSpan(ui->tableWidget_content->rowCount()-1,0,1,4);
            QPushButton * pb1 = new QPushButton(this);
            pb1->setText("Добавить новое лицо");
            pb1->setProperty("id",ui->tableWidget_content->rowCount()-1);
            connect(pb1,SIGNAL(clicked()),this,SLOT(add_person()));
            ui->tableWidget_content->setCellWidget(ui->tableWidget_content->rowCount()-1, 0, pb1);
        }
        else
        {
            QStringList title;
            title.append("Действие");
            ui->tableWidget_content->setHorizontalHeaderLabels(title);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            QPushButton * pb1 = new QPushButton(this);
            pb1->setText("Добавить новое лицо");
            pb1->setProperty("id",0);

            connect(pb1,SIGNAL(clicked()),this,SLOT(add_person()));

            ui->tableWidget_content->setCellWidget(0, 0, pb1);
            QMessageBox::information(this,"Внимание","Нет актерского состава");
        }
    }
    pb_cancel_fantom->show();
}

void MainWindow::delete_person() // Удаляем человека из режисерского состава/
{
    switch (id_create)
    {
    case 0:
    {
        if(QMessageBox::question(this,"Внимание","Вы действительно хотите удалить его?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            QPushButton * pb = qobject_cast<QPushButton *>(sender());
            int id = pb->property("id_part").toInt();
            qw.prepare("DELETE FROM public.part_member WHERE id_part = :id");
            qw.bindValue(":id",id);
            if(qw.exec())
                QMessageBox::information(this,"Yes","Yes");
            else
                QMessageBox::information(this,"No","No");

            //Нужно запустить функцию чтобы отобразить текущий состав
            edit_team();
        }
        break;
    }
    case 1:
    {
        if(QMessageBox::question(this,"Внимание","Вы действительно хотите удалить его вместе с его используемыми ролями?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            QPushButton * pb1 = qobject_cast<QPushButton *>(sender());
            int id = pb1->property("id").toInt();
            qw.prepare("DELETE FROM public.part_member WHERE id_member = :id");
            qw.bindValue(":id",id);
            qw.exec();
            qw.prepare("DELETE FROM public.member WHERE id_member = :id");
            qw.bindValue(":id",id);
            if(qw.exec())
                QMessageBox::information(this,"Успешно","Удалили его");
            else
                QMessageBox::information(this,"Внимание","Не удалили его");
            add_person();
        }
        break;
    }
    default:
        QMessageBox::warning(this,"","ERROR");
    }

}

void MainWindow::add_person()//Функция чтобы добавлять к фильму новых режисеров/продюсеров/сценаристов
{
    //Перересовываем таблицу вставляя в неё уже существующих персон и добвляем кнопку выбрать одного из них (после выбора открываем 1-й групп бокс
    //где завершаем добавление
    //Дополнительно добавляем кноку создать новую персону
    id_create = 1;

    ui->tableWidget_content->clear();
    ui->tableWidget_content->show();
    ui->tableWidget_content->setRowCount(0);
    ui->tableWidget_content->setColumnCount(0);

    ui->groupBox_rating_review->hide();
    pb_cancel_fantom->show();

    qw.exec("SELECT id_member, name_member, awards_member FROM \"member\" ORDER BY id_member");

    ui->tableWidget_content->setRowCount(qw.size()+1);
    ui->tableWidget_content->setColumnCount(1);
    if(qw.size()>0)
    {
        ui->tableWidget_content->setColumnCount(4);
        ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
        ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
        ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
        ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
        qw.first();

        QStringList title;
        title.append("Имя");
        title.append("Кол-во наград");
        title.append("Действие");
        title.append("Действие удаления");
        ui->tableWidget_content->setHorizontalHeaderLabels(title);

        QTableWidgetItem *item =  new QTableWidgetItem(qw.value(1).toString());
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget_content->setItem(0, 0, item);

        item =  new QTableWidgetItem(qw.value(2).toString());
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget_content->setItem(0, 1, item);

        int i = 0;

        QPushButton * pb = new QPushButton(this);
        pb->setText("Выбрать");
        pb->setProperty("id",qw.value(0).toInt());
        pb->setProperty("name",qw.value(1).toString());
        connect(pb,SIGNAL(clicked()),this,SLOT(chose_person()));

        ui->tableWidget_content->setCellWidget(i, 2, pb);

        QPushButton * pb1 = new QPushButton(this);
        pb1->setText("Удалить");
        pb1->setProperty("id",qw.value(0).toInt());
        connect(pb1,SIGNAL(clicked()),this,SLOT(delete_person()));

        ui->tableWidget_content->setCellWidget(i, 3, pb1);

        while (qw.next())
        {
            i++;
            QTableWidgetItem *item =  new QTableWidgetItem(qw.value(1).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);

            ui->tableWidget_content->setItem(i, 0, item);

            item =  new QTableWidgetItem(qw.value(2).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);

            ui->tableWidget_content->setItem(i, 1, item);
            QPushButton * pb = new QPushButton(this);
            pb->setText("Выбрать");
            pb->setProperty("id",qw.value(0).toInt());
            pb->setProperty("name",qw.value(1).toString());
            connect(pb,SIGNAL(clicked()),this,SLOT(chose_person()));

            ui->tableWidget_content->setCellWidget(i, 2, pb);

            QPushButton * pb1 = new QPushButton(this);
            pb1->setText("Удалить");
            pb1->setProperty("id",qw.value(0).toInt());
            connect(pb1,SIGNAL(clicked()),this,SLOT(delete_person()));

            ui->tableWidget_content->setCellWidget(i, 3, pb1);
        }
        ui->tableWidget_content->setSpan(ui->tableWidget_content->rowCount()-1,0,1,4);
    }
    QPushButton * pb2 = new QPushButton(this);
    pb2->setText("Создать новое лицо");
    connect(pb2,SIGNAL(clicked()),this,SLOT(new_person()));
    ui->tableWidget_content->setCellWidget(ui->tableWidget_content->rowCount()-1, 0, pb2);
}

void MainWindow::chose_person()// добавление в фильм уже созданного человека со своей ролью
{
    pb_cancel_fantom->hide();
    id_create = 2;
    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    QString name = pb->property("name").toString();
    id_member = pb->property("id").toInt();

    ui->tableWidget_content->hide(); //Прячем таблицу чтобы не разрушить логику програмы
    ui->groupBox_rating_review->show();

    ui->radioButton_rating->hide();
    ui->radioButton_review->hide();

    ui->tableWidget_content->show();

    ui->label_rating_review->hide();

    ui->label_rec->show();
    ui->label_rec->setText("Профессия:");
    ui->comboBox_rec->show();
    ui->comboBox_rec->clear();

    qw.exec("SELECT * FROM type_member");
    QVector<QPair<QString,int>> items;
    while(qw.next())
    {
        QPair<QString,int> test;
        test.first = qw.value(1).toString();
        test.second = qw.value(0).toInt();
        items.append(test);
        ui->comboBox_rec->addItem(test.first,test.second);
    }
    ui->comboBox_rec->setEnabled(true);
    ui->label_name_content->setText("Имя человека: ");
    ui->textEdit_name_content->setReadOnly(true);
    ui->textEdit_name_content->setText(name);
    ui->pushButton_save->show();
    ui->spinBox_rating->hide();
}

void MainWindow::new_person() // Создание нового человека:)
{
    id_create = 3;
    pb_cancel_fantom->hide();
    ui->tableWidget_content->hide(); //Прячем таблицу чтобы не разрушить логику програмы
    ui->groupBox_rating_review->show();

    ui->radioButton_rating->hide();
    ui->radioButton_review->hide();

    ui->label_rating_review->hide();

    ui->comboBox_rec->hide();
    ui->label_rec->hide();

    ui->textEdit_name_content->setReadOnly(false);
    ui->textEdit_name_content->clear();
    ui->label_name_content->setText("Имя человека: ");

    ui->label_rating_review->show();
    ui->label_rating_review->setText("Количество наград: ");
    ui->spinBox_rating->show();
    ui->spinBox_rating->setMaximum(99);
}

void MainWindow::buuton_cancel() // Фантомная кнопка назад
{
    if(ui->pushButton_search->isChecked())
    {
        pb_cancel_fantom->setVisible(false);
        switch (id_create)
        {
        case 0:
        {
            pb_cancel_fantom->hide();
            on_pushButton_search_clicked();
            on_pushButton_search_clicked();
            break;
        }
        case 1:
        {
            pb_cancel_fantom->hide();
            edit_team();
            break;
        }
        case 2:
        {
            pb_cancel_fantom->hide();
            edit_team();
            break;
        }
        }
    }
    if(ui->pushButton_add_rr_to_usr->isChecked())
    {
        pb_cancel_fantom->setVisible(false);
        pb_cancel_fantom->hide();
        on_pushButton_add_rr_to_usr_clicked();
        on_pushButton_add_rr_to_usr_clicked();
    }
}

void MainWindow::delete_rr_user() // Фантомная функция для удаления поставленной пользователем оценки
{
    if(id_log_in == 6)
    {
        if (flag_rating)
        {
            if(QMessageBox::question(this,"Внимание","Вы хотите удалить свою оценку?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                // Сначала удаляем оценку в журнале
                // Проверка на то есть ли отзыв, если есть то не убираем запись, а обновляем
                if(flag_review)//если есть отзыв
                {
                    qw.prepare("UPDATE public.journal SET id_rating = NULL WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    qw.exec();

                }
                else
                {
                    qw.prepare("DELETE FROM public.journal WHERE id_user = :id_user AND id_content = :id_content");
                    qw.bindValue(":id_user",id_user_global);
                    qw.bindValue(":id_content",id_content_table);
                    qw.exec();
                }
                // Далее удаляем запись об оценке
                qw.prepare("DELETE FROM public.rating WHERE id_user = :id_user AND id_content = :id_content");
                qw.bindValue(":id_user",id_user_global);
                qw.bindValue(":id_content",id_content_table);
                if(qw.exec())
                {
                    QMessageBox::information(this,"Внимание","Получилось удалить оценку в бд 'оценка'");
                }
                flag_rating = false;

                // Потом все прячем
                {
                    if(!flag_review)
                    {
                        pb_delete_rr_user->hide();
                        ui->groupBox_rating_review->hide();
                        //перерисовываем таблицу
                        ui->pushButton_show_r_r->click();
                        ui->pushButton_show_r_r->click();
                    }
                }
            }
        }
        if (flag_review)
        {
            if(QMessageBox::question(this,"Внимание","Вы хотите удалить свой отзыв?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                // Сначала удаляем отзыв в журнале

                // Проверка на то есть ли отзыв, если есть то не убираем запись, а обновляем
                if(flag_rating)//если есть отзыв
                {
                    qw.prepare("UPDATE public.journal SET id_review = NULL WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    qw.exec();

                }
                else
                {
                    qw.prepare("DELETE FROM public.journal WHERE id_user = :id_user AND id_content = :id_content");
                    qw.bindValue(":id_user",id_user_global);
                    qw.bindValue(":id_content",id_content_table);
                    qw.exec();
                }

                // Далее удаляем запись об оценке
                qw.prepare("DELETE FROM public.review WHERE id_user = :id_user AND id_content = :id_content");
                qw.bindValue(":id_user",id_user_global);
                qw.bindValue(":id_content",id_content_table);
                if(qw.exec())
                {
                    QMessageBox::information(this,"Внимание","Получилось удалить отзыв в бд 'отзыв'");
                }

                flag_review = false;

                // Потом все прячем
                {
                    if(!flag_rating)
                    {
                        pb_delete_rr_user->hide();
                        ui->groupBox_rating_review->hide();
                        //перерисовываем таблицу
                        ui->pushButton_show_r_r->click();
                        ui->pushButton_show_r_r->click();
                    }
                }
            }
        }

    }
    if(id_log_in == 7)
    {
        if (flag_rating)
        {
            if(QMessageBox::question(this,"Внимание","Вы хотите удалить чужую оценку?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                // Сначала удаляем оценку в журнале
                // Проверка на то есть ли отзыв, если есть то не убираем запись, а обновляем
                if(flag_review)//если есть отзыв
                {
                    qw.prepare("UPDATE public.journal SET id_rating = NULL WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    if(qw.exec())
                    {
                        QMessageBox::information(this,"Внимание","Получилось обновить запись в журнале и убрать информацию об оценке");
                    }
                }
                else
                {
                    qw.prepare("DELETE FROM public.journal WHERE id_user = :id_user AND id_content = :id_content");
                    qw.bindValue(":id_user",id_user_global);
                    qw.bindValue(":id_content",id_content_table);
                    if(qw.exec())
                    {
                        QMessageBox::information(this,"Внимание","Получилось удалить запись в журнале");
                    }
                }
                // Далее удаляем запись об оценке
                qw.prepare("DELETE FROM public.rating WHERE id_user = :id_user AND id_content = :id_content");
                qw.bindValue(":id_user",id_user_global);
                qw.bindValue(":id_content",id_content_table);
                if(qw.exec())
                {
                    QMessageBox::information(this,"Внимание","Получилось удалить оценку в бд 'оценка'");
                }
                flag_rating = false;

                // Потом все прячем
                {
                    if(!flag_review)
                    {
                        pb_delete_rr_user->hide();
                        ui->groupBox_rating_review->hide();
                        //перерисовываем таблицу
                        id_create = 1;
                        content_censor();
                    }
                }
            }
        }
        if (flag_review)
        {
            if(QMessageBox::question(this,"Внимание","Вы хотите удалить чужой отзыв?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                // Сначала удаляем отзыв в журнале

                // Проверка на то есть ли отзыв, если есть то не убираем запись, а обновляем
                if(flag_rating)//если есть отзыв
                {
                    qw.prepare("UPDATE public.journal SET id_review = NULL WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    if(qw.exec())
                    {
                        QMessageBox::information(this,"Внимание","Получилось обновить запись в журнале и убрать информацию об отзыве");
                    }

                }
                else
                {
                    qw.prepare("DELETE FROM public.journal WHERE id_user = :id_user AND id_content = :id_content");
                    qw.bindValue(":id_user",id_user_global);
                    qw.bindValue(":id_content",id_content_table);
                    if(qw.exec())
                    {
                        QMessageBox::information(this,"Внимание","Получилось удалить запись в журнале");
                    }
                }

                // Далее удаляем запись об оценке
                qw.prepare("DELETE FROM public.review WHERE id_user = :id_user AND id_content = :id_content");
                qw.bindValue(":id_user",id_user_global);
                qw.bindValue(":id_content",id_content_table);
                if(qw.exec())
                {
                    QMessageBox::information(this,"Внимание","Получилось удалить отзыв в бд 'отзыв'");
                }
                flag_review = false;
                // Потом все прячем
                {
                    if(!flag_rating)
                    {
                        pb_delete_rr_user->hide();
                        ui->groupBox_rating_review->hide();
                        //перерисовываем таблицу
                        id_create = 1;
                        content_censor();
                    }
                }
            }
        }
    }
}

void MainWindow::add_content() //Функция для создания новых фильмов
{
    QPushButton * pb2 = qobject_cast<QPushButton *>(sender());
    QString name = pb2->property("name").toString();
    id_content_table = pb2->property("id").toInt();
    flag_globb = true;

    //открываем групп боксы для создание данных о фильмах
    ui->groupBox_rating_review->show();
    ui->groupBox_rating_review->setTitle("");
    ui->textEdit_name_content->clear();
    ui->textEdit_name_content->setReadOnly(false);
    ui->radioButton_rating->hide();
    ui->radioButton_review->hide();
    ui->textEdit_review->hide();
    ui->label_rec->hide();
    ui->comboBox_rec->hide();
    ui->label_rating_review->hide();
    ui->spinBox_rating->hide();
    ui->groupBox_content->show();
    ui->lineEdit_count_rating->hide();
    ui->lineEdit_count_review->hide();
    ui->lineEdit_averege_rating->hide();
    ui->lineEdit_averege_review->hide();
    ui->label_count_rating->hide();
    ui->label_averege_rating->hide();
    ui->label_count_review->hide();
    ui->label_averege_review->hide();
    ui->textEdit_description->clear();
    ui->textEdit_description->setReadOnly(false);
    ui->spinBox_awards->setReadOnly(false);
    ui->spinBox_awards->setValue(0);
    ui->pushButton_team->hide();
    ui->pushButton_save->show();
}

void MainWindow::delete_content() // Функция которая удаляет фильмы
{
    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    id_content_table = pb->property("id").toInt();

    if(QMessageBox::question(this,"Внимание","Вы хотите удалить фильм вместе с его оценками/отзывами и съемочной командой?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        db->transaction();
        qw.prepare("DELETE FROM public.part_member WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.prepare("DELETE FROM public.journal WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.prepare("DELETE FROM public.rating WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.prepare("DELETE FROM public.review WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.prepare("DELETE FROM public.content WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        if(db->commit())
        {
            QMessageBox::information(this,"Yes","Получилось удалить");
            ui->pushButton_show_r_r->click();
            ui->pushButton_show_r_r->click();
        }
        else
        {
            QMessageBox::information(this,"No","Не получилось удалить");
        }
    }
}

void MainWindow::content_censor() // Функция с помощью которой подгружаем все оценки/отзывы на выбранный фильм
{
    ui->groupBox_rating_review->hide();
    {//Отображаем в табличке журнал на выбранный фильм

        QPushButton * pb = qobject_cast<QPushButton *>(sender());
        if (pb!=nullptr)
        {
            on_radioButton_rating_clicked();
            if(id_create==0)
                id_content_table = pb->property("id_content").toInt();
        }
        else
            return;
        ui->tableWidget_content->clear();

        ui->tableWidget_content->setColumnCount(1);
        ui->tableWidget_content->setRowCount(1);

        qw.prepare("SELECT id_user FROM \"journal\" WHERE id_content = :id ORDER BY id_user");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.first();

        if(qw.size() > 0)
        {
            ui->tableWidget_content->setRowCount(qw.size());
            ui->tableWidget_content->setColumnCount(2);

            QStringList title;
            title.append("login");
            title.append("Действие");
            ui->tableWidget_content->setHorizontalHeaderLabels(title);

            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

            int i = 0;

            QTableWidgetItem *item =  new QTableWidgetItem(qw.value(0).toString());
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);

            ui->tableWidget_content->setItem(i, 0, item);

            QPushButton * pb = new QPushButton(this);
            pb->setText("Выбрать");
            pb->setProperty("id",qw.value(0).toInt());
            connect(pb,SIGNAL(clicked()),this,SLOT(content_censor_user()));
            ui->tableWidget_content->setCellWidget(i, 1, pb);
            while (qw.next())
            {
                i++;

                QTableWidgetItem *item =  new QTableWidgetItem(qw.value(0).toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                ui->tableWidget_content->setItem(i, 0, item);

                QPushButton * pb = new QPushButton(this);
                pb->setText("Выбрать");
                pb->setProperty("id",qw.value(0).toInt());
                connect(pb,SIGNAL(clicked()),this,SLOT(content_censor_user()));
                ui->tableWidget_content->setCellWidget(i, 1, pb);
            }
            for (int j=0;j<i+1;j++)
            {
                qw.prepare("SELECT login FROM \"user\" WHERE id_user = :id");
                qw.bindValue(":id", ui->tableWidget_content->item(j,0)->text().toInt());
                qw.exec();
                qw.first();

                QTableWidgetItem *item = new QTableWidgetItem(qw.value(0).toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                ui->tableWidget_content->setItem(j, 0, item);
            }
        }
        else
        {
            ui->tableWidget_content->setRowCount(0);
            ui->tableWidget_content->setColumnCount(0);
            QMessageBox::information(this,"Внимание","Нет оценок/отзывов на этот фильм");
        }
        id_create = 0;
    }
    pb_cancel_fantom->show();
    pb_delete_rr_user->hide();
    ui->groupBox_rating_review->hide();
}

void MainWindow::content_censor_user() //Функция с помощью которой подгружаем пользователей и их оценки/отзывы
{
    pb_cancel_fantom->hide();

    int id_user = 0;
    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    if (pb!=nullptr)
    {
        id_user = pb->property("id").toInt();
    }
    else
        return;
    id_user_global = id_user;
    //Открываем оценки и отзывы пользователя
    {//Включаем группбокс
        ui->groupBox_rating_review->show();
        qw.prepare("SELECT login FROM \"user\" WHERE id_user = :id");
        qw.bindValue(":id", id_user);
        qw.exec();
        qw.first();
        QString str = qw.value(0).toString();
        ui->groupBox_rating_review->setTitle("Удаленние реакции пользователя: " + str);
        qw.prepare("SELECT name FROM \"content\" WHERE id_content = :id");
        qw.bindValue(":id", id_content_table);
        qw.exec();
        qw.first();
        str = qw.value(0).toString();
        ui->textEdit_name_content->setText(str);
        ui->textEdit_name_content->setReadOnly(true);
        ui->textEdit_description->setReadOnly(true);
        ui->textEdit_review->setReadOnly(true);
    }

    int rating = 0;
    QString text_rec = "";
    int rec = 0;

    {//Подгружаем поставленные оценки и отзывы
        qw.prepare("SELECT rating FROM \"rating\" WHERE id_user = :id AND id_content = :id2");
        qw.bindValue(":id", id_user);
        qw.bindValue(":id2", id_content_table);
        qw.exec();
        qw.first();
        if(qw.size()>0)
        {
            flag_rating=true;
            rating = qw.value(0).toInt();
            pb_delete_rr_user->show();
        }
        else
            flag_rating=false;

        qw.prepare("SELECT text_review, recomendation FROM \"review\" WHERE id_user = :id AND id_content = :id2");
        qw.bindValue(":id", id_user);
        qw.bindValue(":id2", id_content_table);
        qw.exec();
        qw.first();
        if(qw.size()>0)
        {
            flag_review=true;
            text_rec = qw.value(0).toString();
            rec = qw.value(1).toInt();
            pb_delete_rr_user->show();
        }
        else
            flag_review=false;
    }

    {//проставляем полученные оценки
        if(flag_rating)
        {
            ui->radioButton_rating->click();
            ui->spinBox_rating->setValue(rating);

            pb_delete_rr_user->show();
        }
        if(flag_review)
        {
            if(!flag_rating)
                ui->radioButton_review->click();
            ui->textEdit_review->setText(text_rec);
            ui->comboBox_rec->setCurrentIndex(rec);
        }
    }
    ui->pushButton_save->hide();
}

void MainWindow::on_pushButton_save_clicked() // Кнопка для сохранение данных в базу ;)
{
    if(id_log_in == 6) // Работаем как стандартный пользователь
    {
        int rating = ui->spinBox_rating->value();
        if(rating == 0 and ui->radioButton_rating->isChecked())
        {
            QMessageBox::information(this,"Пожалуйста","Оценка должна быть от 1 до 10");
            return;
        }
        QString review = "DMITRII";

        if(ui->radioButton_rating->isChecked())
        {
            if(!flag_rating)
            {
                int id_rating_int;
                //Добавление в БД "оценка"
                {
                    qw.prepare("INSERT INTO public.rating (id_user, id_content, rating) VALUES (:id_user, :id_content, :rating) RETURNING id_rating");
                    qw.bindValue(":id_user", id_user_global);
                    qw.bindValue(":id_content", id_content_table);
                    qw.bindValue(":rating", rating);
                    qw.exec();
                    qw.next();
                    id_rating_int = qw.value(0).toInt();
                }
                //Добавление в БД "журнал"
                {
                    if(!flag_review)//Если нет никакой записи в журнале(нет рецензии) то создаем новую
                    {
                        qw.prepare("INSERT INTO public.journal (id_user, id_content, id_rating, id_review) "
                                   "VALUES (:id_user, :id_content, :id_rating, :id_review)");
                        qw.bindValue(":id_user", id_user_global);
                        qw.bindValue(":id_content", id_content_table);
                        qw.bindValue(":id_rating", id_rating_int);
                        qw.exec();
                        flag_rating=true;
                        QMessageBox::information(this,"Да","Получилось добавить запись в журнал");
                    }
                    else
                    {
                        qw.prepare("UPDATE public.journal SET id_rating = :rating WHERE id_user = :id AND id_content = :content");
                        qw.bindValue(":rating",id_rating_int);
                        qw.bindValue(":id", id_user_global);
                        qw.bindValue(":content", id_content_table);
                        qw.exec();
                        QMessageBox::information(this,"Да","Получилось обновить оценку");
                    }
                }
                flag_rating = true;
                pb_delete_rr_user->show();
            }
            else
            {
                qw.prepare("UPDATE public.rating SET rating = :rating WHERE id_user = :id AND id_content = :content");
                qw.bindValue(":rating", ui->spinBox_rating->value());
                qw.bindValue(":id", id_user_global);
                qw.bindValue(":content", id_content_table);
                qw.exec();
                QMessageBox::information(this,"Да","Получилось обновить оценку");
            }
        }
        else
        {
            if(ui->radioButton_review->isChecked())
            {
                if(!flag_review) //Создаем новую запись в журнале
                {
                    int id_review_int;

                    { // Добавления в БД "Рецензия"

                        int rec_int = ui->comboBox_rec->currentIndex();
                        bool rec_bool = rec_int == 1;
                        review = ui->textEdit_review->toPlainText();

                        qw.prepare("INSERT INTO public.review (id_content, id_user, text_review, recomendation) "
                                   "VALUES (:id_content, :id_user, :text_review, :recomendation) RETURNING id_review");
                        qw.bindValue(":id_content", id_content_table);
                        qw.bindValue(":id_user", id_user_global);
                        qw.bindValue(":text_review", review);
                        if(rec_int != 0)
                        {
                            qw.bindValue(":recomendation", rec_bool);
                        }
                        qw.exec();

                        qw.next();
                        id_review_int = qw.value(0).toInt();
                        flag_review = true;
                        pb_delete_rr_user->show();
                    }

                    //Добавление в БД "журнал"
                    {
                        if(!flag_rating)//Если нет никакой записи в журнале(нет оценки) то создаем новую
                        {
                            qw.prepare("INSERT INTO public.journal (id_user, id_content, id_rating, id_review) "
                                       "VALUES (:id_user, :id_content, :id_rating, :id_review)");
                            qw.bindValue(":id_user", id_user_global);
                            qw.bindValue(":id_content", id_content_table);
                            qw.bindValue(":id_review", id_review_int);
                            qw.exec();
                            flag_review=true;
                            QMessageBox::information(this,"Да","Получилось добавить запись в журнал");
                        }
                        else
                        {
                            qw.prepare("UPDATE public.journal SET id_review = :review WHERE id_user = :id AND id_content = :content");
                            qw.bindValue(":review",id_review_int);
                            qw.bindValue(":id", id_user_global);
                            qw.bindValue(":content", id_content_table);
                            qw.exec();
                            QMessageBox::information(this,"Да","Получилось обновить запись в журнал");
                        }
                    }
                }
                else // Изменяем рецензию
                {
                    review = ui->textEdit_review->toPlainText();
                    qw.prepare("UPDATE public.review SET text_review = :review WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":review", review);
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    qw.exec();

                    int rec_int = ui->comboBox_rec->currentIndex();
                    bool rec_bool= rec_int == 1;
                    qw.prepare("UPDATE public.review SET recomendation = :recomendation WHERE id_user = :id AND id_content = :content");
                    qw.bindValue(":id", id_user_global);
                    qw.bindValue(":content", id_content_table);
                    if(rec_int == 0)
                        qw.bindValue(":recomendation", rec_bool);
                    qw.exec();
                    QMessageBox::information(this,"Да","Получилось обновить отзыв в бд");
                }
            }
            else
                QMessageBox::information(this,"Ошибка","ОШИБКА(БЛИЗКА К КРЕТИЧЕСКОЙ)",QMessageBox::Ok);
        }
    }
    if(id_log_in == 7) // Работаем как стандартный админ
    {
        if(id_create>0)
        {
            switch (id_create)
            {
            case 2:
            {
                //Новая запись человек + роль + фильм
                QVariant indexData = ui->comboBox_rec->currentData();
                qw.prepare("INSERT INTO public.part_member (id_member, id_type, id_content) "
                           "VALUES (:id_member, :id_type, :id_content)");
                qw.bindValue(":id_member", id_member);
                qw.bindValue(":id_type", indexData.toInt());
                qw.bindValue(":id_content", id_content_table);

                if(qw.exec())
                {
                    QMessageBox::information(this,"Успех","Получилось добавить");
                    add_person();
                    ui->groupBox_rating_review->hide();
                    ui->tableWidget_content->show();
                    pb_cancel_fantom->show();
                }
                else
                    QMessageBox::warning(this,"Ошибка",qw.lastError().text());

                break;
            }
            case 3:
            {
                QString name = ui->textEdit_name_content->toPlainText().trimmed();
                int count    = ui->spinBox_rating->value();
                if(name.isEmpty())
                {
                    QMessageBox::warning(this,"Ошибка","Некорректное имя");
                    return;
                }
                //добавляем проверку на то чтобы человек с таким именем отсутсвовал
                {
                    qw.exec("SELECT name_member FROM \"member\"");
                    while (qw.next())
                    {
                        if(qw.value(0).toString() == name)
                        {
                            QMessageBox::warning(this,"Уважаемый","Такое имя уже есть в таблице");
                            return;
                        }
                    }
                }
                qw.prepare("INSERT INTO public.member (name_member, awards_member) "
                           "VALUES (:name_member, :awards_member)");
                qw.bindValue(":name_member", name);
                qw.bindValue(":awards_member", count);
                if(qw.exec())
                {
                    QMessageBox::information(this,"Уважаемый","Удалось к счастью");
                    add_person();
                }
                else
                    QMessageBox::warning(this,"Уважаемый","Не удалось к сожалению");
                break;
            }
            }
        }
        if(ui->pushButton_show_r_r->isChecked())
        {
            if(QMessageBox::question(this,"Вопрос","Сохраняем изменения?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            {
                qw.prepare("SELECT id_content FROM \"content\" WHERE name = :name_content");
                qw.bindValue(":name_content", ui->textEdit_name_content->toPlainText().trimmed());
                qw.exec();
                if(qw.size()>0)
                {
                    flag_globb = false;
                }
                if(!flag_globb)
                {
                    qw.prepare("UPDATE public.content SET name = :name WHERE id_content = :id");
                    qw.bindValue(":id", id_content_table);
                    qw.bindValue(":name", ui->textEdit_name_content->toPlainText().trimmed());
                    qw.exec();
                    qw.prepare("UPDATE public.content SET description = :description WHERE id_content = :id");
                    qw.bindValue(":id", id_content_table);
                    qw.bindValue(":description", ui->textEdit_description->toPlainText().trimmed());
                    qw.exec();
                    qw.prepare("UPDATE public.content SET awards = :awards WHERE id_content = :id");
                    qw.bindValue(":id", id_content_table);
                    qw.bindValue(":awards", ui->spinBox_awards->value());
                    qw.exec();
                    //Отгрузка кнопок отмены и сохранения
                    {
                        ui->groupBox_rating_review->setTitle("Добавление/Просмотр оценки/рецензии");
                        ui->groupBox_rating_review->hide();
                        ui->radioButton_rating->show();
                        ui->radioButton_review->show();
                        ui->spinBox_rating->show();
                        ui->label_rating_review->show();
                        ui->label_rec->show();
                        ui->textEdit_review->show();
                        ui->textEdit_name_content->show();
                        ui->textEdit_name_content->setReadOnly(true);
                        ui->label_name_content->show();
                        ui->comboBox_rec->show();
                    }
                    //Отгрузка группбокса с инфой
                    ui->groupBox_content->hide();
                    ui->pushButton_show_r_r->click();
                    ui->pushButton_show_r_r->click();
                }
                else
                {
                    qw.prepare("SELECT id_content FROM \"content\" WHERE name = :name_content");
                    qw.bindValue(":name_content", ui->textEdit_name_content->toPlainText().trimmed());
                    qw.exec();
                    if(qw.size()>0)
                    {
                        QMessageBox::warning(this,"Ошибка","Фильм с таким именем уже существует");
                        return;
                    }

                    qw.prepare("INSERT INTO public.content (name, description, awards) "
                               "VALUES (:name, :description, :awards)");
                    qw.bindValue(":name", ui->textEdit_name_content->toPlainText().trimmed());
                    qw.bindValue(":description", ui->textEdit_description->toPlainText().trimmed());
                    qw.bindValue(":awards", ui->spinBox_awards->value());
                    if(qw.exec())
                    {
                        QMessageBox::information(this,"Внимание","Получилось добавить");
                    }
                    else
                    {
                        QMessageBox::warning(this,"Ошибка","Не получилось добавить");
                    }
                    ui->pushButton_show_r_r->click();
                    ui->pushButton_show_r_r->click();
                }
            }
        }
    }
}

void MainWindow::on_radioButton_rating_clicked() // Выбираем радиобаттон чтобы написать/удалить/посмотреть свою оценку
{
    if(ui->pushButton_add_rr_to_usr->isChecked())
    {
        ui->textEdit_review->hide();
        ui->label_rating_review->setText("Оценка :");
        ui->spinBox_rating->show();
        ui->spinBox_rating->setReadOnly(false);

        ui->pushButton_save->setVisible(true);

        ui->label_rec->hide();
        ui->comboBox_rec->hide();
        if(flag_rating)
            pb_delete_rr_user->show();
    }
    if(ui->pushButton_show_r_r->isChecked() or ui->pushButton_search->isChecked())
    {
        if((!flag_rating and ui->pushButton_search->isChecked()) or (!flag_rating and ui->pushButton_show_r_r->isChecked()))
        {
            ui->spinBox_rating->hide();
            ui->textEdit_review->hide();
            ui->label_rating_review->setText("Нет вашей оценки");
            ui->comboBox_rec->hide();
            ui->label_rec->hide();
        }
        else
        {
            ui->textEdit_review->hide();
            ui->label_rating_review->setText("Оценка :");
            ui->spinBox_rating->show();
            ui->spinBox_rating->setReadOnly(true);

            ui->pushButton_save->setVisible(false);

            ui->label_rec->hide();
            ui->comboBox_rec->hide();
        }
    }
    if(flag_rating)
    {
        if(!ui->pushButton_search->isChecked())
            pb_delete_rr_user->show();
    }
    if(flag_rating and id_user_global == 7)
    {
        pb_delete_rr_user->show();
        if(id_log_in == 7 and ui->pushButton_add_rr_to_usr->isChecked())
        {

            ui->pushButton_save->hide();
        }
        content_censor_user();
    }
    if(flag_review and id_user_global == 7)
    {
        pb_delete_rr_user->hide();
        if(id_log_in == 7 and ui->pushButton_add_rr_to_usr->isChecked())
        {
            ui->label_rating_review->setText("Нет оценки");
            ui->comboBox_rec->hide();
            ui->textEdit_review->hide();
            ui->label_rec->hide();
            ui->spinBox_rating->hide();
            ui->pushButton_save->hide();
        }
    }
}

void MainWindow::on_radioButton_review_clicked() // Выбираем радиобаттон чтобы написать/удалить/посмотреть свой отзыв
{
    if(ui->pushButton_add_rr_to_usr->isChecked())
    {
        ui->spinBox_rating->hide();
        ui->label_rating_review->setText("Текст рецензии :");
        ui->textEdit_review->show();
        ui->textEdit_review->setReadOnly(false);

        ui->label_rec->show();
        ui->comboBox_rec->show();
        ui->comboBox_rec->setEnabled(true);
        ui->comboBox_rec->clear();
        ui->comboBox_rec->addItem("Нейтральная");
        ui->comboBox_rec->addItem("Положительная");
        ui->comboBox_rec->addItem("Отрицательная");

        ui->label_rec->setText("Тип рецензии:");

        if(!flag_review)
        {
            ui->textEdit_review->clear();
            ui->comboBox_rec->setCurrentIndex(0);
        }
    }
    if(ui->pushButton_show_r_r->isChecked() or ui->pushButton_search->isChecked())
    {
        if((!flag_review and ui->pushButton_search->isChecked()) or (!flag_review and ui->pushButton_show_r_r->isChecked()))
        {
            ui->spinBox_rating->hide();
            ui->textEdit_review->hide();
            ui->label_rating_review->setText("Нет вашей рецензии");
        }
        else
        {
            ui->spinBox_rating->hide();
            ui->label_rating_review->setText("Текст рецензии :");
            ui->textEdit_review->show();
            ui->textEdit_review->setReadOnly(true);

            ui->label_rec->show();
            ui->comboBox_rec->show();
            ui->comboBox_rec->setEnabled(false);
        }
    }
    if(flag_review)
    {
        if(!ui->pushButton_search->isChecked())
            pb_delete_rr_user->show();
        content_censor_user();
    }
    else {
        pb_delete_rr_user->hide();
        if(id_log_in == 7 and ui->pushButton_add_rr_to_usr->isChecked())
        {
            ui->label_rating_review->setText("Нет рецензии");
            ui->comboBox_rec->hide();
            ui->textEdit_review->hide();
            ui->label_rec->hide();
            ui->spinBox_rating->hide();
        }
    }
}

void MainWindow::on_pushButton_cancel_clicked() // кнопка отмены расположенная в первом группбоксе
{
    switch (id_create)
    {
    case 0:
    {
        ui->textEdit_review->clear();
        ui->groupBox_rating_review->hide();
        ui->groupBox_content->hide();
        break;
    }
    case 1:
    {
        edit_team();
        break;
    }
    case 2:
    {
        add_person();
        break;
    }
    case 3:
    {
        add_person();
        break;
    }
    }
    pb_delete_rr_user->hide();
    if(id_log_in == 7 and ui->pushButton_add_rr_to_usr->isChecked())
        pb_cancel_fantom->show();
}

void MainWindow::on_pushButton_team_clicked()//Кнопка просмотра актерского состава со стороны пользователя
{
    if(id_log_in == 6)
    {
        if(ui->pushButton_team->isChecked())
        {
            ui->tableWidget_content->clear();
            ui->tableWidget_content->setColumnCount(3);

            QStringList title;
            title.append("Имя");
            title.append("Роль");
            title.append("Кол-во наград");
            ui->tableWidget_content->setHorizontalHeaderLabels(title);

            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            ui->pushButton_search->setEnabled(false);
            ui->pushButton_cancel->hide();
            ui->pushButton_team->setText("Назад");

            //Другой способ загружать команду
            {
                qw.prepare("SELECT member.name_member, type_member.type_member, member.awards_member FROM public.part_member JOIN public.type_member ON part_member.id_type = type_member.id_type JOIN public.member ON part_member.id_member = public.member.id_member WHERE id_content = :id");
                qw.bindValue(":id", id_content_table);
                qw.exec();

                ui->tableWidget_content->setColumnCount(3);
                ui->tableWidget_content->resizeColumnToContents(3);
                ui->tableWidget_content->setRowCount(qw.size());

                int j=0;
                while (qw.next())
                {
                    QTableWidgetItem *item =  new QTableWidgetItem(qw.value(0).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 0,item);

                    item =  new QTableWidgetItem(qw.value(1).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 1, item);

                    item =  new QTableWidgetItem(qw.value(2).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 2, item);
                    j++;
                }
            }
        }
        else
        {
            pb_delete_rr_user->hide();
            ui->pushButton_search->setEnabled(true);
            ui->pushButton_cancel->show();
            ui->pushButton_team->setText("Съемочная команда");
            ui->pushButton_search->click();
            ui->pushButton_search->click();
        }
    }
    if(id_log_in == 7)
    {
        if(ui->pushButton_team->isChecked())
        {
            ui->tableWidget_content->setColumnCount(3);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
            ui->tableWidget_content->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
            ui->tableWidget_content->clear();
            ui->pushButton_search->setEnabled(false);
            ui->pushButton_cancel->hide();
            ui->pushButton_team->setText("Назад (перейти к выбору контента)");

            //Другой способ загружать команду
            {
                qw.prepare("SELECT member.name_member, type_member.type_member, member.awards_member FROM public.part_member JOIN public.type_member ON part_member.id_type = type_member.id_type JOIN public.member ON part_member.id_member = public.member.id_member WHERE id_content = :id");
                qw.bindValue(":id", id_content_table);
                qw.exec();

                ui->tableWidget_content->setColumnCount(3);
                ui->tableWidget_content->resizeColumnToContents(3);
                ui->tableWidget_content->setRowCount(qw.size());

                int j=0;
                while (qw.next())
                {
                    QTableWidgetItem *item =  new QTableWidgetItem(qw.value(0).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 0,item);

                    item =  new QTableWidgetItem(qw.value(1).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 1, item);

                    item =  new QTableWidgetItem(qw.value(2).toString());
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);

                    ui->tableWidget_content->setItem(j, 2, item);
                    j++;
                }
            }
        }
        else
        {
            ui->pushButton_show_r_r->click();
            ui->pushButton_show_r_r->click();
            ui->groupBox_rating_review->show();
            ui->groupBox_content->show();
            ui->pushButton_team->setText("Съемочная команда");
        }
    }
}
