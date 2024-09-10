// CustomItemWidget.h
#ifndef CUSTOMITEMWIDGET_H
#define CUSTOMITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CustomItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomItemWidget(QWidget *parent = nullptr);

    // Métodos para acceder a la viñeta y los botones si es necesario
    QLabel* vignette() const { return m_vignette; }
    QPushButton* button1() const { return m_button1; }
    // Añade aquí métodos para los otros botones si es necesario

private:
    // Widgets privados
    QLabel* m_vignette;
    QPushButton* m_button1;
    // Añade aquí los otros botones si es necesario
};

#endif // CUSTOMITEMWIDGET_H
