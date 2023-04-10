#ifndef PATHBAR_H
#define PATHBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QButtonGroup>
#include <QLabel>

class PathBar : public QWidget
{
    Q_OBJECT
public:
    explicit PathBar(QWidget *parent = nullptr);
    void setPath(const QString &path);

signals:
    void pathChanged(const QString &path);

private:
    QButtonGroup *m_group;
    QHBoxLayout *m_layout;
    QStringList m_pathList;

private slots:
    void onButtonClicked();
};

#endif // PATHBAR_H
