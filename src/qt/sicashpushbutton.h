#ifndef NIRVANAPUSHBUTTON_H
#define NIRVANAPUSHBUTTON_H
#include <QPushButton>
#include <QStyleOptionButton>
#include <QIcon>

class NirvanaPushButton : public QPushButton
{
public:
    explicit NirvanaPushButton(QWidget * parent = Q_NULLPTR);
    explicit NirvanaPushButton(const QString &text, QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void updateIcon(QStyleOptionButton &pushbutton);

private:
    bool m_iconCached;
    QIcon m_downIcon;
};

#endif // NIRVANAPUSHBUTTON_H
