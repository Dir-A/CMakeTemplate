#include <QApplication>
#include <QMainWindow>


class MyMainWindow : public QMainWindow
{
public:
    explicit MyMainWindow(QWidget* parent = nullptr) :
        QMainWindow(parent)
    {
        
    }
};

auto main(int argc, char** argv) -> int
{
    QApplication app(argc, argv);
    MyMainWindow main_win;
    main_win.show();
    return app.exec();
}
