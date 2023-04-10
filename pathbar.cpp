#include "pathbar.h"
#include <QDebug>

PathBar::PathBar(QWidget *parent) : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_group = new QButtonGroup(this);
    m_group->setExclusive(true);
    m_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setLayout(m_layout);
}

void PathBar::setPath(const QString &path)
{
    m_pathList = path.split("\\");  // 分割路径
    if (m_pathList.isEmpty()) {
        return;
    }

    // 清空布局
    QLayoutItem *item;
    while ((item = m_layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // 添加第一个按钮
    QPushButton *firstButton = new QPushButton(m_pathList.first(), this);
    firstButton->setFlat(true);
    firstButton->setCheckable(true);
    firstButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    connect(firstButton, &QPushButton::clicked, this, &PathBar::onButtonClicked);
    m_group->addButton(firstButton);
    m_layout->addWidget(firstButton);

    // 添加剩余的按钮和QLabel
    QStringList::const_iterator iter;
    for (iter = m_pathList.constBegin() + 1; iter != m_pathList.constEnd(); ++iter) {
        QLabel *label = new QLabel(">", this);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        m_layout->addWidget(label);

        QPushButton *button = new QPushButton(*iter, this);
        button->setFlat(true);
        button->setCheckable(true);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        connect(button, &QPushButton::clicked, this, &PathBar::onButtonClicked);
        m_group->addButton(button);
        m_layout->addWidget(button);
    }

    // 设置第一个按钮为选中状态
    firstButton->click();

    // 设置布局长度随按钮改变而改变
    m_layout->addStretch();
}

void PathBar::onButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == nullptr) {
        return;
    }

    int index = m_pathList.indexOf(button->text());  // 获取按钮所在的索引
    if (index == -1) {
        return;
    }
    button->setChecked(true);
    // 移除按钮和QLabel之后的控件
    QLayoutItem *item;
    while ((item = m_layout->takeAt(index*2+1)) != nullptr) {
        delete item->widget();
        delete item;
    }
    // 添加按钮和QLabel
    QStringList::const_iterator iter;
    if(index + 1 < m_pathList.size())
    {
        QLabel *label = new QLabel(">", this);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        m_layout->insertWidget(index*2+1, label);

        QPushButton *button = new QPushButton(m_pathList[index+1], this);
        button->setFlat(true);
        button->setCheckable(true);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        connect(button, &QPushButton::clicked, this, &PathBar::onButtonClicked);
        m_group->addButton(button);
        m_layout->insertWidget(index*2+2, button);
    }

    // 发送路径改变信号
//    QString path = m_pathList.mid(0, index + 1).join("\\");
//    qDebug()<<path;

    m_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    emit pathChanged(button->text());
}
